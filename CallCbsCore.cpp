#include "pch.h"
#include "CCbsUIHandlerImpl.h"

HRESULT DoTask_InstallPackage()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  // todo: do your own work

  // A callback handler interface implemented by user
  ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("PkgInstaller");
  // CHECK(g_sess->RegisterCbsUIHandler(pUiHandler),
  //	"Failed to register ICbsUIHandler into current session.");

  ComPtr<ICbsPackage> pPkg;

  CHECK(g_sess->CreatePackage(0, CbsPackageTypeCabinet,
    _T("C:\\Users\\HigHwind\\Desktop\\tmp\\cab\\nt10\\Windows10.0-KB4517245-x64.cab"),
    _T("C:\\Users\\HigHwind\\Desktop\\tmp\\cbstemp\\sandbox"),
    &pPkg), "Failed to create package from cab file.");

  _CbsInstallState stCur, stApp;
  CHECK(pPkg->EvaluateApplicability(0, &stApp, &stCur), "Failed to evaluate the package, maybe it's invalid.");

  if (stCur == CbsInstallStateInstalled) {
    LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "You have installed this package! The program is to exit.");
    return S_OK;
  }

  if (stApp != CbsInstallStateInstalled)
    RET_HR_LOG(E_NOT_VALID_STATE, "The package is not applicable for your OS.");

  CHECK(PrintPackageInfo(pPkg, stCur == CbsInstallStateInstalled), "Failed to print package info.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Ready. The package is to be installed.");

  // Guaranteed success
  pPkg->InitiateChanges(0, CbsInstallStateInstalled, pUiHandler);

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

HRESULT DoTask_EnumeratePkgs()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("PkgLister");
  ComPtr<IEnumCbsIdentity> pIdents;
  CHECK(g_sess->EnumeratePackages(0x1b0, &pIdents), "Failed to enum pkgs.");

  for (auto& x : GetIEnumVector<ICbsIdentity, IEnumCbsIdentity>(pIdents)) {
    ComPtr<ICbsPackage> pPkg;
    CHECK(g_sess->OpenPackage(0, x, nullptr, &pPkg), "Failed to open pkg with identity.");
    CHECK(PrintPackageInfo(pPkg), "Failed to print package info.");
  }

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

HRESULT DoTask_EnableHyperV()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("Hyper-V Enabler");

  auto pFound = GetFoundationPackage();
  assert(pFound);

  PrintPackageInfo(pFound);

  ComPtr<ICbsUpdate> pUpd;
  CHECK(pFound->GetUpdate(L"Microsoft-Windows-HyperV-OptionalFeature-HypervisorPlatform-Disabled-Package", &pUpd), "..");

  pUpd->SetInstallState(0, CbsInstallStateInstalled);

  pFound->InitiateChanges(0, CbsInstallStateInstalled, pUiHandler);

  /*ComPtr<IEnumCbsUpdate> pUpds;
  CHECK(pFound->EnumerateUpdates(CbsApplicabilityApplicable, CbsSelectabilityClass1, &pUpds),
    "Failed to enumerate updates from foundation package.");

  for (auto& pUpd : GetIEnumVector<ICbsUpdate, IEnumCbsUpdate>(pUpds))
    PrintUpdateInfo(pUpd);*/

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

int main()
{
  BEGIN_ERROR_HANDLING();

  g_conf.output_log = true;
  CHECK(CheckSudoSelf(), "Failed to check permission and sudo self.");

  if (hr == S_ASYNCHRONOUS)
  {
    printf("OK, preparing to restart and sudo...\n");
    return 0;
  }

  printf("OK, now we have TI permission.\n");

  CHECK(CoInitialize(nullptr), "Failed to initialize COM.");

  // I18n console host
  setlocale(LC_ALL, "");

  // The env var of app is used to config CBS's logging setting.
  //g_conf.SetLogFile(CBS_LOG_FILE);
  g_conf.mode = CCbsConfig::SessMode::Online;
  g_conf.stack_source = CCbsConfig::StackSource::Registry;

  CHECK(g_mgr.FindStack(), "Failed to find stack [StackSource = %s].",
    GetEnumName(g_conf.stack_source));
  CHECK(g_mgr.Load(), "Failed to automatically load stack.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "We are to execute the task!");
  system("pause");

  CHECK(DoTask_EnumeratePkgs(), "User-defined operations in the task have NOT been performed fully.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "We are to dispose the manager.");
  CHECK(g_mgr.Dispose(), "Failed to dispose StackManager.");

  CoUninitialize();

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "All resource cleared!");

  getchar();

  return S_OK;
}
