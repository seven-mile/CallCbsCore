#include "pch.h"
#include "CCbsUIHandlerImpl.h"

HRESULT RunApplication()
{
	BEGIN_ERROR_HANDLING();

	// todo: do your own work

	// A callback handler interface implemented by user
	ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl;
	CHECK(pCbsSession->RegisterCbsUIHandler(pUiHandler),
		"Failed to register ICbsUIHandler into current session.");

	ComPtr<ICbsPackage> pPkg;
	CHECK(pCbsSession->CreatePackage(NULL, CbsPackageTypeCabinet,
		_T("C:\\Users\\HigHwind\\Desktop\\tmp\\cab\\nt10\\windows10.0-kb3186578-x86_fffce1ba07ca03ebd00979ed5fa1c604a541d6fa.cab"),
		_T("C:\\Users\\HigHwind\\Desktop\\tmp\\cbstemp\\sandbox"), &pPkg),
		"Failed to create package from cabinet file.");

	{
		LPTSTR szProp;
		CHECK(pPkg->GetProperty(CbsPackagePropertyIdentityString, &szProp),
			"Failed to get identity string for the cab package.");
		_tprintf(_T("Pack: %s\n"), szProp);

		CHECK(pPkg->GetProperty(CbsPackagePropertyDescription, &szProp),
			"Failed to get description for the cab package.");
		_tprintf(_T("\t%s\n"), szProp);

		CHECK(pPkg->GetProperty(CbsPackagePropertyReleaseType, &szProp),
			"Failed to get release type for the cab package.");
		_tprintf(_T("\t%s"), szProp);

		CHECK(pPkg->GetProperty(CbsPackagePropertySupportInformation, &szProp),
			"Failed to get support info for the cab package.");
		_tprintf(_T("\t%s\n"), szProp);

		CHECK(pPkg->GetProperty(CbsPackagePropertyKeyword, &szProp),
			"Failed to get release type for the cab package.");
		_tprintf(_T("\t%s\n"), szProp);
	}

	_CbsInstallState appl, curr;
	CHECK(pPkg->EvaluateApplicability(NULL, &appl, &curr),
		"Failed to evaluate applicability for the cab package.");

	LPTSTR szApplState, szCurrState;
	TextizeCbsInstallState(appl, &szApplState);
	TextizeCbsInstallState(curr, &szCurrState);

	_tprintf(_T("Applicable State: %s Current State: %s.\n"), szApplState, szCurrState);

	// Install package (the operation will be append to the PoQ)
	//CHECK(pPkg->InitiateChanges(NULL, CbsInstallStateInstalled, pUiHandler),
	//	"Failed to initiate a installing change for the cab package.");

	ComPtr<IEnumCbsUpdate> pUpdEnum;
	CHECK(pPkg->EnumerateUpdates(CbsApplicabilityNotApplicable,
		CbsSelectabilityClass1, &pUpdEnum),
		"Failed to enumerate updates from package.");

	InsertLine(LineSizeLong);

	for (auto pUpd : GetIEnumVector<ICbsUpdate, IEnumCbsUpdate>(pUpdEnum))
	{
		LPTSTR szProp = NULL;
		CHECK(pUpd->GetProperty(CbsUpdatePropertyDisplayName, &szProp),
			"Failed to get display name for update.");
		_tprintf(_T("Detect update: %s\n"), szProp);

		CHECK(pUpd->GetProperty(CbsUpdatePropertyName, &szProp),
			"Failed to get alias name for update.");
		_tprintf(_T("\t{Alias - %s}\n"), szProp);

		InsertLine(LineSizeLong);
	}

	return S_OK;
}

int main()
{
	BEGIN_ERROR_HANDLING();

	CHECK(CoInitialize(NULL), "Failed to initialize COM.");
	
	// I18n console host
	setlocale(LC_ALL, "");

	// The env var of app is used to config CBS's logging setting.
	SetEnvironmentVariable(_T("COMPONENT_BASED_SERVICING_LOGFILE"), CBS_LOG_FILE);

	CHECK(FindStackByReg(), "Failed to find stack by read registry.");
	CHECK(LoadCbsCore(), "Failed to load cbscore.dll library.");
	CHECK(LoadSxSStore(), "Failed to load sxsstore.dll library.");
	CHECK(InitCbsCoreAndSxSStore(), "Failed to create cbscore and sxsstore class factory.");
	CHECK(LoadWdsCore(), "Failed to catch wdscore.dll, logging of the application in file won't be initialized.");

	// You can open an online session when you have access to TrustedInstaller using function OpenOnlineSession
	// Specify the path to a offline windows image. For example: L"D:\\", L"D:\\Windows".
	CHECK(OpenOnlineSession(/*_T("E:\\MyCache\\Vx"), _T("E:\\MyCache\\Vx\\Windows"), */CbsSessionOptionNone),
		"Failed to open an session for CBS operation.");

	CHECK(RunApplication(), "User-defined operations have NOT been performed fully.");

	// Make the changes come into effects.
	CHECK(CloseSessionAndFinalizeCbsCore(), "Failed to close current session or to finalize cbscore.");

	CoUninitialize();

	return S_OK;

#ifndef _DEBUG
	system("pause");
#endif
}
