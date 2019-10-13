#include "pch.h"
#include "RegWrapper.hpp"
#include <sstream>
#include <filesystem>

std::filesystem::path pathServicingStack;
std::filesystem::path pathCbsCore;
std::filesystem::path pathSxSStore;

HRESULT(__stdcall* vpfnCbsCoreInitialize)(IMalloc*, int(__stdcall*)(int), void (*)(), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
HRESULT(__stdcall* vpfnCbsCoreSetState)(int state, PROC value);
HRESULT(__stdcall* vpfnCbsCoreFinalize)();
HRESULT(__stdcall* vpfnSetTestMode)(int mode);

HRESULT(__stdcall* vpfnSxsStoreInitialize)(IMalloc*, int(__stdcall*)(int), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
HRESULT(*vpfnSxsStoreFinalize)();

HRESULT(__stdcall* vpfnWdsSetupLogMessageA)(LPVOID pMsg, enum WdsLogSource source, const char*, const char*, ULONG, const char* file, const char* func, void* CurIP, ULONG, void*, UINT);
LPVOID(__stdcall* vpfnConstructPartialMsgVA)(WdsLogLevel level, const char* fmtMsg, va_list va);
LPVOID (*vpfnCurrentIP)();

int(__stdcall* vpfnGetSystemStore)(UINT reserved, const struct _GUID&, struct IUnknown**);

IClassFactory* pCbsCoreFactory;
IClassFactory* pSxSFactory;

ICbsSession* pCbsSession;
ICbsSessionInternal* pCbsSessionIn;

#pragma region GUID DEF
const GUID CLSID_CbsSession = { 0x752073a1, 0x23F2, 0x4396, { 0x85, 0xF0, 0x8F, 0xDB, 0x87, 0x9E, 0xD0, 0xED } };
const GUID IID_ICbsUIHandler = { 0x75207392, 0x23F2, 0x4396, { 0x85, 0xF0, 0x8F, 0xDB, 0x87, 0x9E, 0xD0, 0xED } };
const GUID IID_ICSIStore = { 0x465F1EC1, 0x7F1D, 0x4A85,{ 0xA3, 0x0B, 0xAE, 0x10, 0x90, 0xF2, 0x12, 0xDB } };
const GUID IID_ISxSStore = { 0x8601319A, 0xD7CF, 0x40F3, {0x90, 0x25, 0x7f, 0x77, 0x12, 0x54, 0x53, 0xC6 } };
const GUID IID_IServicingQuerier = { 0x6A0316CF, 0xC193, 0x4A6F, {0xB1, 0xBF, 0x73, 0x4A, 0xF6, 0x41, 0x27, 0x3F } };
const GUID IID_ICbsSession = { 0x75207391, 0x23F2, 0x4396, {0x85,0xF0,0x8F,0xDB,0x87,0x9E,0xD0,0xED} };
const GUID IID_ICbsSessionPrivate = { 0x0FC2C7E52, 0xDF06, 0x445E, { 0x8D, 0x83, 0xBA, 0x12, 0x58, 0x80, 0xEA, 0x0D } };
const GUID IID_ICbsSession9 = { 0x9C7E3CF3, 0x4C97, 0x4D36, { 0xBD, 0xEB, 0xE3, 0x09, 0x3C, 0x22, 0x8C, 0x22 } };
#pragma endregion

#pragma region CallbackFunctions

int __stdcall CoreEventLockProcCB(int unkArg)
{
	return S_OK;
}

void CoreEventUnlockProcCB() {

}

void CoreEventInstCreatedCB() {

}

void CoreEventInstDestroyedCB() {

}

void CoreEventReqShutdownNowCB() {

}

void CoreEventReqShutdownProcessingCB() {

}

#pragma endregion

HRESULT FindStackByReg()
{
	BEGIN_ERROR_HANDLING();

	try {
		winreg::RegKey keyStack(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\Version"));
		auto vs = keyStack.EnumValues();

		if (vs.empty()) RET_WIN32ERR_LOG(ERROR_NOT_FOUND, "FATAL ERROR: No stack found in the registry!");

		WdsLogA(S_OK, WdsLogSourceTOOL, WdsLogLevelInfo, "Connected to CBS Registry, %u stack(s) found.", vs.size());

		// sort the stack by version
		std::sort(vs.begin(), vs.end(), [](auto lhs, auto rhs) {
			std::wstringstream ls, rs;
			ls << lhs.first;
			rs << rhs.first;
			for (UINT iCnt = 0; iCnt < 4; iCnt++)
			{
				std::wstring curVerL, curVerR;
				std::getline(ls, curVerL, L'.');
				std::getline(rs, curVerR, L'.');

				if (curVerL == curVerR) continue;

				return curVerL.size() == curVerR.size() ? curVerL > curVerR : curVerL.size() > curVerR.size();
			}
			return false;
		});

		// use the newest stack defaultly
		pathServicingStack = ExpandEnvW(keyStack.GetStringValue(vs[0].first));
		
	} catch (winreg::RegException &e) {
		RET_WIN32ERR_LOG(e.ErrorCode(), e.what());
	}

	pathCbsCore = pathServicingStack;
	pathCbsCore += _T("\\CbsCore.dll");
	
	pathSxSStore = _T("SxSStore.dll"); // default dir: %WINDIR%

	return hr;
}

HRESULT LoadCbsCore()
{
	BEGIN_ERROR_HANDLING();

	auto hCbsCore = LoadLibrary(pathCbsCore.c_str());
	if (!hCbsCore) RET_LASTERR_LOG("Failed to load dll %s.", pathCbsCore.c_str());

	vpfnCbsCoreInitialize =
		(HRESULT(__stdcall*)(struct IMalloc*, int(__stdcall*)(int),
		void (*)(void), void (*)(void), void (*)(void), void (*)(void),
		void (*)(void), struct IClassFactory**))
		GetProcAddress(hCbsCore, "CbsCoreInitialize");
	vpfnCbsCoreSetState = (HRESULT(__stdcall*)(int state, PROC value))
		GetProcAddress(hCbsCore, "CbsCoreSetState");
	vpfnCbsCoreFinalize = (HRESULT(__stdcall*)(void))
		GetProcAddress(hCbsCore, "CbsCoreFinalize");

	if (!vpfnCbsCoreInitialize || !vpfnCbsCoreSetState || !vpfnCbsCoreFinalize)
		RET_LASTERR_LOG("Failed to find proc in DLL CbsCore.dll.");
	return hr;
}

HRESULT LoadSxSStore()
{
	BEGIN_ERROR_HANDLING();

	auto hSxSStore = LoadLibrary(pathSxSStore.c_str());
	if (!hSxSStore) RET_LASTERR_LOG("Failed to load dll %s.", pathSxSStore.c_str());

	vpfnSxsStoreInitialize =
		(HRESULT(__stdcall*)(struct IMalloc*, int(__stdcall*)(int), void (*)(void), void (*)(void), void (*)(void), void (*)(void), struct IClassFactory**))
		GetProcAddress(hSxSStore, "SxsStoreInitialize");
	vpfnSxsStoreFinalize = (HRESULT(*)(void))GetProcAddress(hSxSStore, "SxsStoreFinalize");

	if (!vpfnSxsStoreInitialize || !vpfnSxsStoreFinalize)
		RET_LASTERR_LOG("Failed to find proc in DLL SxSStore.dll.");
	return hr;
}

HRESULT LoadWdsCore()
{
	BEGIN_ERROR_HANDLING();

	auto hWdsCore = GetModuleHandle(_T("wdscore.dll"));
	if (!hWdsCore) RET_LASTERR_LOG("Failed to load WdsCore.dll.");

	vpfnWdsSetupLogMessageA =
		(HRESULT(__stdcall*)(LPVOID, enum WdsLogSource, char const*, char const*, ULONG, char const*, char const*, void* CurIP, ULONG, void*, UINT))
		GetProcAddress(hWdsCore, "WdsSetupLogMessageA");
	vpfnConstructPartialMsgVA =
		(LPVOID(__stdcall*)(enum WdsLogLevel, const char*, va_list va))
		GetProcAddress(hWdsCore, "ConstructPartialMsgVA");
	vpfnCurrentIP = (void* (*)(void))GetProcAddress(hWdsCore, "CurrentIP");

	if (!vpfnWdsSetupLogMessageA || !vpfnConstructPartialMsgVA || !vpfnCurrentIP)
		RET_LASTERR_LOG("Failed to find proc in DLL WdsCore.dll.");

	return hr;
}

HRESULT LoadWcp()
{
	BEGIN_ERROR_HANDLING();

	auto hWcpDll = GetModuleHandle(_T("wcp.dll"));
	if (!hWcpDll) RET_LASTERR_LOG("Failed to load Wcp.dll.");

	vpfnGetSystemStore =
		(int(__stdcall*)(unsigned int, const struct _GUID&, struct IUnknown**))
		GetProcAddress(hWcpDll, "GetSystemStore");

	if (!vpfnGetSystemStore) RET_LASTERR_LOG("Failed to find proc in DLL wcp.dll.");

	return hr;
}

HRESULT InitCbsCoreAndSxSStore()
{
	BEGIN_ERROR_HANDLING();

	LPMALLOC pMalloc = NULL;

	CHECK(CoGetMalloc(1u, &pMalloc), "CoGetMalloc failed.");

	CHECK(vpfnCbsCoreInitialize(pMalloc, CoreEventLockProcCB, CoreEventUnlockProcCB, CoreEventInstCreatedCB, CoreEventInstDestroyedCB,
		CoreEventReqShutdownNowCB, CoreEventReqShutdownProcessingCB, &pCbsCoreFactory), "Failed to initialize CbsCore.");
	CHECK(vpfnSxsStoreInitialize(pMalloc, CoreEventLockProcCB, CoreEventUnlockProcCB, CoreEventInstCreatedCB, CoreEventInstDestroyedCB,
		CoreEventReqShutdownProcessingCB, &pSxSFactory), "Failed to initialize SxSStore.");

	return hr;
}

HRESULT OpenOnlineSession(_CbsSessionOption option)
{
	BEGIN_ERROR_HANDLING();

	if (pCbsSession) RET_HR_LOG(E_INVALIDARG, "Cannot open session with an used object.");

	CHECK(pCbsCoreFactory->CreateInstance(NULL, IID_ICbsSession, (LPVOID*)&pCbsSession),
		"Failed to create ICbsSession object.");

	CHECK(pCbsSession->Initialize(option, _T("CallCbsCore"), NULL, NULL),
		"Failed to initialize online session with option = %u.", option);

	// This makes ExtendedQueryInterface method runs correctly
	pCbsSessionIn = *((ICbsSessionInternal * *)pCbsSession - 8 + 6);
	*(LPVOID*)(pCbsSession + 5) = pCbsSessionIn;

	return hr;
}

HRESULT OpenOfflineSession(LPCTSTR szBootDrive, LPCTSTR szWindir, _CbsSessionOption option)
{
	BEGIN_ERROR_HANDLING();

	if (pCbsSession) RET_HR_LOG(E_INVALIDARG, "Cannot open session with an used object.");

	CHECK(pCbsCoreFactory->CreateInstance(NULL, IID_ICbsSession, (LPVOID*)& pCbsSession), "Failed to create ICbsSession object.");

	CHECK(pCbsSession->Initialize(option, _T("CallCbsCore"), szBootDrive, szWindir),
		"Failed to initialize offline session with option = %u, location = %s.", option, szBootDrive);

	// This makes ExtendedQueryInterface method runs correctly
	pCbsSessionIn = *((ICbsSessionInternal * *)pCbsSession - 8 + 6);
	*(LPVOID*)(pCbsSession + 5) = pCbsSessionIn;

	return hr;
}

HRESULT CloseSessionAndFinalizeCbsCore()
{
	BEGIN_ERROR_HANDLING();

	_CbsRequiredAction ra;
	CHECK(pCbsSession->FinalizeEx(NULL, &ra), "Failed to finalize cbs session.");

	pCbsSession->Release();
	pSxSFactory->Release();
	pCbsCoreFactory->Release();
	
	CHECK(vpfnSxsStoreFinalize(), "Failed to finalize SxSStore.");
	CHECK(vpfnCbsCoreFinalize(), "Failed to finalize CbsCore.");
	
	return hr;
}
