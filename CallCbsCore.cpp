#include "pch.h"
#include "CCbsUIHandlerImpl.h"

HRESULT RunApplication()
{
	BEGIN_ERROR_HANDLING();

	// todo: do your own work
	CComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl;
	CHECK(pCbsSession->RegisterCbsUIHandler(pUiHandler),
		"Failed to register ICbsUIHandler into current session.");

	CComPtr<ICbsPackage> pPkg;
	CHECK(pCbsSession->CreatePackage(NULL, CbsPackageTypeCabinet,
		_T("C:\\Users\\HigHwind\\Desktop\\windows10.0-kb4465065"
			"-v3-x64_be21f962bde9c80626b88439f90e1bbb4c97528f.cab"),
		_T("C:\\Users\\HigHwind\\Desktop\\sandbox"), &pPkg),
		"Failed to create package from cabinet file.");

	CComPtr<IEnumCbsUpdate> pUpdEnum;
	CHECK(pPkg->EnumerateUpdates(CbsApplicabilityNotApplicable,
		CbsSelectabilityClass1, &pUpdEnum),
		"Failed to enumerate updates from package.");

	ULONG nUpd = 128;
	std::unique_ptr<CComPtr<ICbsUpdate>[]> arrUpds(new CComPtr<ICbsUpdate>[ nUpd ]);

	CHECK(pUpdEnum->Next(nUpd, (ICbsUpdate**)arrUpds.get(), &nUpd),
		"Failed to get ICbsUpdate* s.");

	InsertLine(LineSizeLong);
	
	for (ULONG iUpd = 0; iUpd < nUpd; iUpd++)
	{
		auto& pUpd = arrUpds[iUpd];
		LPTSTR szProp = NULL, szProp2 = NULL;
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

	setlocale(LC_ALL, "");
	
	CHECK(FindStackByReg(), "Failed to find stack by read registry.");
	CHECK(LoadCbsCore(), "Failed to load cbscore.dll library.");
	CHECK(LoadSxSStore(), "Failed to load sxsstore.dll library.");
	CHECK(InitCbsCoreAndSxSStore(), "Failed to create cbscore and sxsstore class factory.");
	CHECK(LoadWdsCore(), "Failed to catch wdscore.dll, logging from cbs.log won't be initialized.");
	
	CHECK(OpenOfflineSession(_T("E:\\MyCache\\Vx"), _T("E:\\MyCache\\Vx\\Windows"), CbsSessionOptionDoSynchronousCleanup),
		"Failed to open an offline session for CBS operation.");

	CHECK(RunApplication(), "User-defined operations have NOT been performed fully.");

	CHECK(CloseSessionAndFinalizeCbsCore(), "Failed to close current session and to finalize cbscore.");
	
	CoUninitialize();

	return S_OK;

#ifndef _DEBUG
	system("pause");
#endif
}
