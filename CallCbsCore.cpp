#include "pch.h"
#include "CCbsUIHandlerImpl.h"

HRESULT RunApplication()
{
	BEGIN_ERROR_HANDLING();

	// todo: do your own work

	// A callback handler interface implemented by user
	ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("PkgInstaller");
	// CHECK(pCbsSession->RegisterCbsUIHandler(pUiHandler),
	//	"Failed to register ICbsUIHandler into current session.");

	ComPtr<ICbsPackage> pPkg;

	CHECK(pCbsSession->CreatePackage(NULL, CbsPackageTypeCabinet,
		_T("C:\\Users\\HigHwind\\Desktop\\tmp\\cab\\nt10\\Windows10.0-KB4517245-x64.cab"),
		_T("C:\\Users\\HigHwind\\Desktop\\tmp\\cbstemp\\sandbox"),
		&pPkg), "Failed to create package from cab file.");

	_CbsInstallState stCur, stApp;
	CHECK(pPkg->EvaluateApplicability(NULL, &stApp, &stCur), "Failed to evaluate the package, maybe it's invalid.");

	if (stCur == CbsInstallStateInstalled) {
		WdsLogHrInternalA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "You have installed this package! The program is to exit.");
		return S_OK;
	}
	
	if (stApp != CbsInstallStateInstalled)
		RET_HR_LOG(E_NOT_VALID_STATE, "The package is not applicable for your OS.");

	CHECK(PrintPackageInfo(pPkg, stCur == CbsInstallStateInstalled), "Failed to print package info.");

	WdsLogHrInternalA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Ready. The package is to be installed.");

	// Guaranteed success
	pPkg->InitiateChanges(NULL, CbsInstallStateInstalled, pUiHandler);

	return S_OK;
}

int main()
{
	BEGIN_ERROR_HANDLING();

	CHECK(CoInitialize(NULL), "Failed to initialize COM.");
	
	// I18n console host
	setlocale(LC_ALL, "");

	// The env var of app is used to config CBS's logging setting.
	g_conf.SetLogFile(CBS_LOG_FILE);
	g_conf.SetLogOutput(true);

	CHECK(FindStackByReg(), "Failed to find stack by read registry.");
	CHECK(LoadCbsCore(), "Failed to load cbscore.dll library.");
	CHECK(LoadSxSStore(), "Failed to load sxsstore.dll library.");
	CHECK(InitCbsCoreAndSxSStore(), "Failed to create cbscore and sxsstore class factory.");
	CHECK(LoadWdsCore(), "Failed to catch wdscore.dll, logging of the application in file won't be initialized.");

	// You can open an online session when you have access to TrustedInstaller using function OpenOnlineSession
	// Specify the path to a offline windows image. For example: _T("D:\\"), _T("D:\\Windows").
	CHECK(OpenOnlineSession(CbsSessionOptionNone),
		"Failed to open a session for CBS operation.");

	CHECK(RunApplication(), "User-defined operations have NOT been performed fully.");

	// Make the changes come into effects.
	CHECK(CloseSessionAndFinalizeCbsCore(), "Failed to close current session or to finalize cbscore.");

	CoUninitialize();

	return S_OK;

#ifndef _DEBUG
	system("pause");
#endif
}
