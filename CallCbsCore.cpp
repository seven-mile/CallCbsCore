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

	ComPtr<IEnumCbsCapability> pEnumCapa;
	CHECK(pCbsSession->EnumerateCapabilities(
		CbsOnDemandSourceEnumAllowCloud,
		L"Accessibility.Braille",
		NULL,
		NULL,
		NULL,
		NULL,
		&pEnumCapa),
		"Failed to enum capabilities from session.");

	for (auto& pCapa : GetIEnumVector<ICbsCapability, IEnumCbsCapability>(pEnumCapa))
	{
		LPTSTR szNamespace, szArch, szLang;
		ULONG dwMajor, dwMinor;
		
		CHECK(pCapa->GetCapability(&szNamespace, &szLang, &szArch, &dwMajor, &dwMinor),
			"Failed to get capability info.");

		_CbsInstallState appl = CbsInstallStateInvalid, ins = CbsInstallStateInvalid;
		CHECK(pCapa->EvaluateApplicability(NULL, &appl, &ins), "Failed to get capability status.");
		LPTSTR szStatus;
		CHECK(TextizeCbsInstallState(ins, &szStatus), "Failed to textize status.");

		ULONG cntDlBytes;
		CHECK(pCapa->GetDownloadSize(&cntDlBytes), "Failed to get download size in bytes");

		_tprintf(_T("%s.%s %lu.%lu\n\tDownload Size: %.2f MiB\nStatus: %s\n"),
			szNamespace, szLang, dwMajor, dwMinor, cntDlBytes/1024.0/1024, szStatus);

		system("pause");

		// will meet network problem unexpectedly...
		CHECK(pCapa->InitiateChanges(NULL, appl, pUiHandler), "Failed to init a change for this capa.");
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
