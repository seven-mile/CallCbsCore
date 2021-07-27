#include "pch.h"
#include "CCbsUIHandlerImpl.h"
#include "magic_enum.hpp"

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

HRESULT DoTask_InstallPackage()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  // todo: do your own work

  // A callback handler interface implemented by user
  const ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("PkgInstaller");
  // CHECK(g_sess->RegisterCbsUIHandler(pUiHandler),
  //	"Failed to register ICbsUIHandler into current session.");

  ComPtr<ICbsPackage> pPkg;

  InitCommonControls();

  WCHAR file_path[MAX_PATH], file_title[MAX_PATH];
  OPENFILENAMEW ofn{ 0 };
  ofn.lStructSize = sizeof ofn;
  //ofn.hwndOwner = GetConsoleWindow();
  ofn.lpstrFilter = L"Cab files (*.cab)\0*.cab\0All files (*.*)\0*.*\0";
  ofn.lpstrFile = file_path;
  ofn.lpstrFileTitle = file_title;
  ofn.nMaxFile = ofn.nMaxFileTitle = MAX_PATH;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (!GetOpenFileNameW(&ofn)) RET_HR_LOG(S_OK, "The operation was canceled by user.");

  CHECK(g_sess->CreatePackage(0, CbsPackageTypeCabinet,
    file_path,
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

  for (auto& x : GetIEnumComPtrVector<ICbsIdentity, IEnumCbsIdentity>(pIdents)) {
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

  const ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("Hyper-V Enabler");

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

  for (auto& pUpd : GetIEnumComPtrVector<ICbsUpdate, IEnumCbsUpdate>(pUpds))
    PrintUpdateInfo(pUpd);*/

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

HRESULT DoTask_EnumerateUpdates()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  auto pFound = GetFoundationPackage();
  assert(pFound);

  ComPtr<IEnumCbsUpdate> pUpds;
  CHECK(pFound->EnumerateUpdates(CbsApplicabilityApplicable, CbsSelectabilityClass1, &pUpds),
    "Failed to enumerate updates from foundation package.");

  for (auto& pUpd : GetIEnumComPtrVector<ICbsUpdate, IEnumCbsUpdate>(pUpds))
    PrintUpdateInfo(pUpd);

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

HRESULT DoTask_Wcp()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  //LPWSTR str1, str2;

  //auto fnGetVersionStr
    //= g_mgr.GetFunction<HRESULT __stdcall(BOOL,wchar_t**,wchar_t**)>(L"wcp.dll", "GetVersionString");

  //CHECK(fnGetVersionStr(false, &str1, &str2), "Failed to Call GetVersionString");

  //printf("%S\n%S\n", str1, str2);

  ComPtr<IStore2> pStore;
  g_mgr.GetSystemStore(0, IID_IStore, (IUnknown**)&pStore);

  ComPtr<ICSIStore> pCSIStore;
  g_mgr.GetSystemStore(0, IID_ICSIStore, (IUnknown**)&pCSIStore);

  ComPtr<ICSITransaction> pTransc;
  CHECK(pCSIStore->BeginTransaction(0, IID_ICSIStore, L"C:\\Users\\HigHwind\\Desktop\\tmp\\cbstemp", &pTransc), "Failed...");

  //CCSDirect* pDirect = ((ISomeInterface*)(*((void****)(pStore.GetInterfacePtr())+1)+2))->GetStoreImplPtr();

  //pStore->LockAssemblies()

  ComPtr<IIdentityAuthority> pIdentAuth;
  const auto fnGetIdentityAuthority = g_mgr.GetFunction<decltype(GetIdentityAuthority)>(L"wcp.dll", "GetIdentityAuthority");
  fnGetIdentityAuthority(&pIdentAuth);
  ComPtr<IDefinitionIdentity> pDefIdent;
  
  CHECK(pIdentAuth->TextToDefinition(0, L"amd64_microsoft-windows-drvstore_31bf3856ad364e35_10.0.19041.546_none_ceed9c2ba2547800", &pDefIdent),
    "Convertion failed!");

  ComPtr<IEnumIDENTITY_ATTRIBUTE> pIdentAttrs;
  CHECK(pDefIdent->EnumAttributes(&pIdentAttrs), "Failed to enum attributes.");

  for (auto &attr : GetIEnumStructVector<IDENTITY_ATTRIBUTE>(pIdentAttrs))
  {
    wprintf(L"%s::%s = %s\\n", attr.pszNamespace, attr.pszName, attr.pszValue);
  }

  ComPtr<IUnknown> pRes;
  CHECK(pStore->GetAssemblyInformation(0, pDefIdent, IID_IStore, &pRes),
    "Failed to query assembly information.");

  return S_OK;
}

int main()
{
  BEGIN_ERROR_HANDLING();

  g_conf.output_log = true;
  g_conf.SetLogFile(CBS_LOG_FILE);
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
  //g_conf.mode = CCbsConfig::SessMode::Online;
  g_conf.mode = CCbsConfig::SessMode::Online;
  g_conf.stack_source = CCbsConfig::StackSource::SSShim;

  CHECK(g_mgr.FindStack(), "Failed to find stack [StackSource = %s].",
    GetEnumName(g_conf.stack_source).c_str());
  CHECK(g_mgr.Load(), "Failed to automatically load stack.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "We are to execute the task!");
  //system("pause");

  CHECK(DoTask_EnumeratePkgs(), "User-defined operations in the task have NOT been performed fully.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "We are to dispose the manager.");
  CHECK(g_mgr.Dispose(), "Failed to dispose StackManager.");

  CoUninitialize();

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "All resource cleared!");

  return S_OK;
}
