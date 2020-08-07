#include "pch.h"
#include "RegWrapper.hpp"
#include "StackManager.h"

HRESULT(WINAPI* vpfnCbsCoreInitialize)(IMalloc*, int(WINAPI*)(int), void (*)(), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
HRESULT(WINAPI* vpfnCbsCoreSetState)(int state, PROC value);
HRESULT(WINAPI* vpfnCbsCoreFinalize)();
HRESULT(WINAPI* vpfnSetTestMode)(int mode);

HRESULT(WINAPI* vpfnSxsStoreInitialize)(IMalloc*, int(WINAPI*)(int), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
HRESULT(*vpfnSxsStoreFinalize)();

HRESULT(WINAPI* vpfnWdsSetupLogMessageA)(LPVOID pMsg, enum WdsLogSource source, const char*, const char*, ULONG, const char* file, const char* func, void* CurIP, ULONG, void*, UINT);
LPVOID(WINAPI* vpfnConstructPartialMsgVA)(WdsLogLevel level, const char* fmtMsg, va_list va);
LPVOID(*vpfnCurrentIP)();

int(WINAPI* vpfnGetSystemStore)(UINT reserved, const struct _GUID&, struct IUnknown**);

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

int NilFunc1(int) { return S_OK; }
void NilFunc2() {  }

StackManager g_mgr;

HRESULT StackManager::FindStackByReg()
{
  BEGIN_ERROR_HANDLING();

  try {
    winreg::RegKey keyStack(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\Version"), KEY_QUERY_VALUE);
    auto vs = keyStack.EnumValues();
    if (vs.empty()) RET_WIN32ERR_LOG(ERROR_NOT_FOUND, "FATAL ERROR: No stack found in the registry!");
    LogA(S_OK, WdsLogSourceTOOL, WdsLogLevelInfo, "Connected to CBS Registry, %u stack(s) found.", vs.size());
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
    ServicingStack = keyStack.GetStringValue(vs[0].first);
    ServicingStack.resize(lstrlenW(ServicingStack.c_str()));
  }
  catch (winreg::RegException& e)
  {
    RET_WIN32ERR_LOG(e.ErrorCode(), e.what());
  }

  CHECK(FindStackByPath(ServicingStack),
    "Registry has been read successfully, but stack path [\n\t = %S \n] is unusable.",
    ServicingStack.c_str());

  return S_OK;
}

HRESULT StackManager::FindStackByPath(std::wstring path)
{
  ServicingStack = path;

  CbsCore = ServicingStack;
  CbsCore += _T("\\CbsCore.dll");

  SxSStore = _T("SxSStore.dll"); // default dir: %WINDIR%

  // verifying validation
  WIN32_FIND_DATA res_data;
  std::wcout << L"cbscore: " << CbsCore << std::endl;
  auto res = FindFirstFile(CbsCore.c_str(), &res_data);
  if (res == INVALID_HANDLE_VALUE)
    return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

  return S_OK;
}

HRESULT StackManager::FindStackBySSShim(std::wstring path) {
  return E_NOTIMPL;
}

HRESULT StackManager::FindStack() {
  BEGIN_ERROR_HANDLING();

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "FindStack! [Mode = %s, Source = %s].",
    GetEnumName(g_conf.mode).c_str(),
    GetEnumName(g_conf.stack_source).c_str());
  if (g_conf.mode == CCbsConfig::SessMode::Online) {
    if (g_conf.stack_source == CCbsConfig::StackSource::Registry) {
      CHECK(FindStackByReg(), "Failed to find stack by registry.");
    }
    else RET_HR_LOG(E_INVALIDARG,
      "Invalid global config! [Mode = %s] does not match [Source = %s].",
      GetEnumName(g_conf.mode).c_str(),
      GetEnumName(g_conf.stack_source).c_str());
  }
  else {
    if (g_conf.stack_source == CCbsConfig::StackSource::FolderPath) {
      CHECK(FindStackByPath(g_conf.arg_path), "Failed to find stack by path.");
    }
    else if (g_conf.stack_source == CCbsConfig::StackSource::SSShim) {
      CHECK(FindStackBySSShim(g_conf.arg_path), "Failed to find stack by SSShim.");
    }
    else RET_HR_LOG(E_INVALIDARG,
      "Invalid global config! [Mode = %s] does not match [Source = %s].",
      GetEnumName(g_conf.mode).c_str(),
      GetEnumName(g_conf.stack_source).c_str());
  }
  bStackFound = true;
  return S_OK;
}

HRESULT StackManager::LoadCbsCore() {
  BEGIN_ERROR_HANDLING();

  if (bCbsCoreLoaded) return S_OK;

  auto hCbsCore = LoadLibrary(CbsCore.c_str());
  if (!hCbsCore) RET_LASTERR_LOG("Failed to load dll %S.", CbsCore.c_str());

  vpfnCbsCoreInitialize =
    (HRESULT(WINAPI*)(struct IMalloc*, int(WINAPI*)(int),
      void (*)(void), void (*)(void), void (*)(void), void (*)(void),
      void (*)(void), struct IClassFactory**))
    GetProcAddress(hCbsCore, "CbsCoreInitialize");
  vpfnCbsCoreSetState = (HRESULT(WINAPI*)(int state, PROC value))
    GetProcAddress(hCbsCore, "CbsCoreSetState");
  vpfnCbsCoreFinalize = (HRESULT(WINAPI*)(void))
    GetProcAddress(hCbsCore, "CbsCoreFinalize");

  if (!vpfnCbsCoreInitialize || !vpfnCbsCoreSetState || !vpfnCbsCoreFinalize)
    RET_LASTERR_LOG("Failed to find proc in DLL CbsCore.dll.");
  else bCbsCoreLoaded = true;

  return S_OK;
}

HRESULT StackManager::LoadSxSStore()
{
  BEGIN_ERROR_HANDLING();

  if (bSxSStoreLoaded) return S_OK;

  auto hSxSStore = LoadLibrary(SxSStore.c_str());
  if (!hSxSStore) RET_LASTERR_LOG("Failed to load dll %s.", SxSStore.c_str());

  vpfnSxsStoreInitialize =
    (HRESULT(WINAPI*)(struct IMalloc*, int(WINAPI*)(int), void (*)(void), void (*)(void), void (*)(void), void (*)(void), struct IClassFactory**))
    GetProcAddress(hSxSStore, "SxsStoreInitialize");
  vpfnSxsStoreFinalize = (HRESULT(*)(void))GetProcAddress(hSxSStore, "SxsStoreFinalize");

  if (!vpfnSxsStoreInitialize || !vpfnSxsStoreFinalize)
    RET_LASTERR_LOG("Failed to find proc in DLL SxSStore.dll.");
  else bSxSStoreLoaded = true;

  return S_OK;
}

HRESULT StackManager::InitCbsCore() {
  BEGIN_ERROR_HANDLING();

  if (bCbsCoreInited) return S_OK;
  if (!bCbsCoreLoaded) return E_NOT_VALID_STATE;

  LPMALLOC pMalloc = nullptr;

  CHECK(CoGetMalloc(1u, &pMalloc), "CoGetMalloc failed.");

  if (!CoreEvent.LockProcCB) CoreEvent.LockProcCB = NilFunc1;
#define CHK_FUN2(x) if(!(x)) (x) = NilFunc2
  CHK_FUN2(CoreEvent.UnlockProcCB);
  CHK_FUN2(CoreEvent.UnlockProcCB);
  CHK_FUN2(CoreEvent.InstCreatedCB);
  CHK_FUN2(CoreEvent.InstDestroyedCB);
  CHK_FUN2(CoreEvent.ReqShutdownNowCB);
  CHK_FUN2(CoreEvent.ReqShutdownProcessingCB);

  CHECK(vpfnCbsCoreInitialize(pMalloc, CoreEvent.LockProcCB, CoreEvent.UnlockProcCB, CoreEvent.InstCreatedCB, CoreEvent.InstDestroyedCB,
    CoreEvent.ReqShutdownNowCB, CoreEvent.ReqShutdownProcessingCB, &pCbsCoreFactory), "Failed to initialize CbsCore.");

  bCbsCoreInited = true;

  return S_OK;
}

HRESULT StackManager::InitSxSStore() {
  BEGIN_ERROR_HANDLING();

  if (!bCbsCoreInited || !bSxSStoreLoaded)
    return E_NOT_VALID_STATE;

  LPMALLOC pMalloc = nullptr;

  CHECK(CoGetMalloc(1u, &pMalloc), "CoGetMalloc failed.");

  if (!CoreEvent.LockProcCB) CoreEvent.LockProcCB = NilFunc1;
  CHK_FUN2(CoreEvent.UnlockProcCB);
  CHK_FUN2(CoreEvent.UnlockProcCB);
  CHK_FUN2(CoreEvent.InstCreatedCB);
  CHK_FUN2(CoreEvent.InstDestroyedCB);
  CHK_FUN2(CoreEvent.ReqShutdownProcessingCB);
#undef CHK_FUN2

  CHECK(vpfnSxsStoreInitialize(pMalloc, CoreEvent.LockProcCB, CoreEvent.UnlockProcCB, CoreEvent.InstCreatedCB, CoreEvent.InstDestroyedCB,
    CoreEvent.ReqShutdownProcessingCB, &pSxSFactory), "Failed to initialize SxSStore.");

  bSxSStoreInited = true;

  return S_OK;
}

HRESULT StackManager::InitWdsCore() {
  BEGIN_ERROR_HANDLING();

  if (bWdsCoreInited) return S_OK;
  if (!bCbsCoreInited) return E_NOT_VALID_STATE;

  auto hWdsCore = GetModuleHandle(_T("wdscore.dll"));
  if (!hWdsCore) RET_LASTERR_LOG("Failed to load WdsCore.dll.");

  vpfnWdsSetupLogMessageA =
    (HRESULT(WINAPI*)(void*, enum WdsLogSource, LPCSTR, LPCSTR, ULONG, LPCSTR, LPCSTR, void* CurIP, ULONG, void*, UINT))
    GetProcAddress(hWdsCore, "WdsSetupLogMessageA");
  vpfnConstructPartialMsgVA =
    (LPVOID(WINAPI*)(enum WdsLogLevel, LPCSTR, va_list va))
    GetProcAddress(hWdsCore, "ConstructPartialMsgVA");
  vpfnCurrentIP = (void* (*)(void))GetProcAddress(hWdsCore, "CurrentIP");

  if (!vpfnWdsSetupLogMessageA || !vpfnConstructPartialMsgVA || !vpfnCurrentIP)
    RET_LASTERR_LOG("Failed to find proc in DLL WdsCore.dll.");
  else bWdsCoreInited = true;

  LogA(S_OK, WdsLogSourceWDS, WdsLogLevelInfo, "The logging in file %S initialized successfully.", CBS_LOG_FILE);

  bWdsCoreInited = true;

  return S_OK;
}

HRESULT StackManager::InitWcp() {
  BEGIN_ERROR_HANDLING();

  auto hWcpDll = GetModuleHandle(_T("wcp.dll"));
  if (!hWcpDll) RET_LASTERR_LOG("Failed to load Wcp.dll.");

  vpfnGetSystemStore =
    (int(WINAPI*)(UINT, const _GUID&, IUnknown**))
    GetProcAddress(hWcpDll, "GetSystemStore");

  if (!vpfnGetSystemStore) RET_LASTERR_LOG("Failed to find proc in DLL wcp.dll.");

  bWcpInited = true;

  return S_OK;
}

HRESULT StackManager::Load() {
  BEGIN_ERROR_HANDLING();

  CHECK(LoadCbsCore(), "Failed to load CbsCore.");
  // LoadSxSStore
  CHECK(InitCbsCore(), "Failed to init CbsCore.");
  // InitSxSStore
  CHECK(InitWdsCore(), "Failed to init WdsCore Log Engine.");
  CHECK(InitWcp(), "Failed to init Windows Componentization Platform.");

  return S_OK;
}

HRESULT StackManager::Dispose()
{
  BEGIN_ERROR_HANDLING();

  if (pSess) {
    LogA(S_OK, WdsLogSourceUI, WdsLogLevelWarning,
      "A session is active, so the changes will be abandoned.");
    pSess->Release();
    pSess = nullptr;
  }

  // Memory

  if (bSxSStoreInited) {
    pSxSFactory->Release();
    pSxSFactory = nullptr;
    CHECK(vpfnSxsStoreFinalize(), "Failed to finalize SxSStore.");
  }

  if (bCbsCoreInited) {
    pCbsCoreFactory->Release();
    pCbsCoreFactory = nullptr;
    CHECK(vpfnCbsCoreFinalize(), "Failed to finalize CbsCore.");
  }

  // Configuration

  bStackFound = false;
  bCbsCoreLoaded = false;
  bSxSStoreLoaded = false;
  bCbsCoreInited = false;
  bSxSStoreInited = false;
  bWdsCoreInited = false;
  bWcpInited = false;

  vpfnCbsCoreInitialize = nullptr;
  vpfnCbsCoreSetState = nullptr;
  vpfnCbsCoreFinalize = nullptr;
  vpfnSetTestMode = nullptr;
  vpfnSxsStoreInitialize = nullptr;
  vpfnSxsStoreFinalize = nullptr;
  vpfnWdsSetupLogMessageA = nullptr;
  vpfnConstructPartialMsgVA = nullptr;
  vpfnCurrentIP = nullptr;
  vpfnGetSystemStore = nullptr;

  ServicingStack.clear();
  CbsCore.clear();
  SxSStore.clear();

  return S_OK;
}

HRESULT StackManager::ApplySess(_CbsSessionOption opt,
  const std::wstring strClientId,
  const std::wstring strBootDrive)
{
  BEGIN_ERROR_HANDLING();

  if (!bCbsCoreInited) return E_NOT_VALID_STATE;
  CHECK(pCbsCoreFactory->CreateInstance(nullptr, IID_ICbsSession, (void**)&pSess),
    "Failed to create ICbsSession instance.");

  if (strBootDrive.empty())
  {
    CHECK(pSess->Initialize(opt, strClientId.c_str(), nullptr, nullptr),
      "Failed to initialize online session with option = %u.", opt);
  }
  else
  {
    CHECK(pSess->Initialize(opt, strClientId.c_str(), strBootDrive.c_str(),
      (strBootDrive + L"\\Windows").c_str()),
      "Failed to initialize offline session with option = %u, location = %s.",
      opt, strBootDrive);
  }

  *(void**)(pSess + 5) = *((void**)pSess - 8 + 6);

  return S_OK;
}

HRESULT StackManager::SubmitSess() {
  BEGIN_ERROR_HANDLING();

  if (!GetActiveSess()) return E_NOT_VALID_STATE;

  _CbsRequiredAction ra;
  CHECK(pSess->FinalizeEx(0, &ra), "Failed to finalize cbs session.");

  if (ra == CbsRequiredActionReboot)
    LogA(S_OK, WdsLogSourceUI, WdsLogLevelWarning, "You may need to restart your computer to apply these changes.");

  pSess->Release();
  pSess = nullptr;

  return S_OK;
}

HRESULT StackManager::GetNewSess(_CbsSessionOption opt) {
  if (pSess) RET_HR_LOG(E_ACCESSDENIED, "Current session is still active! You should submit it first.");

  if (g_conf.mode == CCbsConfig::SessMode::Online && !g_conf.arg_bootdrive.empty())
    RET_HR_LOG(E_INVALIDARG, "Invalid global config! [Mode = %s] does not match [BootDrive = %s].",
      enum_name(g_conf.mode).data(), g_conf.arg_bootdrive.c_str());

  return ApplySess(opt, g_conf.strClientName, g_conf.arg_bootdrive);
}
