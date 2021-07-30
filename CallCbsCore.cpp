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

#include "CCbsUpdateTree.h"

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

  //InitCommonControls();

  WCHAR file_path[MAX_PATH]{};// file_title[MAX_PATH];
  //OPENFILENAMEW ofn{ 0 };
  //ofn.lStructSize = sizeof ofn;
  ////ofn.hwndOwner = GetConsoleWindow();
  //ofn.lpstrFilter = L"Cab files (*.cab)\0*.cab\0All files (*.*)\0*.*\0";
  //ofn.lpstrFile = file_path;
  //ofn.lpstrFileTitle = file_title;
  //ofn.nMaxFile = ofn.nMaxFileTitle = MAX_PATH;
  //ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  //if (!GetOpenFileNameW(&ofn)) RET_HR_LOG(S_OK, "The operation was canceled by user.");

  std::wcout << L"Please input the cab package file path:\n";
  wscanf_s(L"%s", file_path, 260);

  CHECK(g_sess->CreatePackage(0, CbsPackageType::Cabinet,
    file_path,
    _T("C:\\Users\\HigHwind\\Desktop\\tmp\\cbstemp\\sandbox"),
    &pPkg), "Failed to create package from cab file.");

  CbsInstallState stCur, stApp;
  CHECK(pPkg->EvaluateApplicability(0, &stApp, &stCur), "Failed to evaluate the package, maybe it's invalid.");

  if (stCur == CbsInstallState::Installed) {
    LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "You have installed this package! The program is to exit.");
    return S_OK;
  }

  if (stApp != CbsInstallState::Installed)
    RET_HR_LOG(E_NOT_VALID_STATE, "The package is not applicable for your OS.");

  CHECK(PrintPackageInfo(pPkg, stCur == CbsInstallState::Installed), "Failed to print package info.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Ready. The package is to be installed.");

  // Guaranteed success
  pPkg->InitiateChanges(0, CbsInstallState::Installed, pUiHandler);

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

HRESULT DoTask_UninstallPackage()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  // todo: do your own work

  // A callback handler interface implemented by user
  const ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("PkgInstaller");
  // CHECK(g_sess->RegisterCbsUIHandler(pUiHandler),
  //	"Failed to register ICbsUIHandler into current session.");

  ComPtr<ICbsPackage> pPkg;

  ComPtr<IEnumCbsIdentity> pIdents;
  CHECK(g_sess->EnumeratePackages(0x1b0, &pIdents), "Failed to enum pkgs.");

  bool flagFound = false;
  for (auto& x : GetIEnumComPtrVector<ICbsIdentity, IEnumCbsIdentity>(pIdents)) {
    BOOL isEqual = -1;
    PWSTR strId;
    unique_malloc_ptr<wchar_t> ustrId;
    x->GetStringId(&strId);
    ustrId.reset(strId);
    strId = nullptr;

    CHECK(x->InternalIsEqualbyAttribute(L"Package_for_KB4511515",
      L"31bf3856ad364e35", L"amd64", L"", L"6.3.1.2516", &isEqual),
      "Failed to check the pkg to uninstall!");
    if (isEqual == 1) {
      ComPtr<ICbsPackage> pPkg;
      CHECK(g_sess->OpenPackage(0, x, nullptr, &pPkg), "Failed to open pkg with identity.");
      // Guaranteed success
      pPkg->InitiateChanges(0, CbsInstallState::Absent, pUiHandler);
      flagFound = true;
      break;
    } else if (isEqual == 0) {
      continue;
    } else {
      RET_WIN32ERR_LOG(ERROR_INVALID_DATA, "Failed to recognize the value of IsEqual RetVal. [isEqual = %d]", isEqual);
    }
  }

  if (flagFound)
    LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Ready. The package is to be uninstalled.");
  else
    RET_WIN32ERR_LOG(ERROR_NOT_FOUND, "Failed to find the package to uninstall.");

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

HRESULT DoTask_EnumerateUpds()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  ComPtr<CCbsUIHandlerImpl> pUiHandler = new CCbsUIHandlerImpl("UpdLister");
  ComPtr<IEnumCbsUpdate> pUpds;
  auto pFound = GetFoundationPackage();
  if (!pFound)
    RET_WIN32ERR_LOG(ERROR_NOT_FOUND, "Foundation Package not found!");

  CHECK(pFound->EnumerateUpdates(CbsApplicability::NeedsParent, CbsSelectability::AllClass, &pUpds),
    "Failed to enum upds from foundation.");

  for (auto& pUpd : GetIEnumComPtrVector<ICbsUpdate>(pUpds)) {
    CHECK(PrintUpdateInfo(pUpd), "Failed to print update info.");
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

  pUpd->SetInstallState(0, CbsInstallState::Installed);

  pFound->InitiateChanges(0, CbsInstallState::Installed, pUiHandler);

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

HRESULT DoTask_FindFoundation() {
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to get a new session.");

  auto pFound = GetFoundationPackage();
  assert(pFound);

  CHECK(PrintPackageInfo(pFound), "Failed to print Foundation Package Information.");

  CHECK(g_mgr.SubmitSess(), "Failed to submit the changes in the session.");

  return S_OK;
}

HRESULT DoTask_ConstructUpdateTree()
{
  BEGIN_ERROR_HANDLING();

  CHECK(g_mgr.GetNewSess(), "Failed to apply new regular session.");

  CCbsUpdateTree tree;
  auto pFound = GetFoundationPackage();
  if (!pFound)
    RET_WIN32ERR_LOG(ERROR_NOT_FOUND, "Failed to get foundation pkg.");

  auto joinUpdates = [&](ComPtr<IEnumCbsUpdate> pUpds){
    for (auto pUpd : GetIEnumComPtrVector<ICbsUpdate>(pUpds)) {
      PWSTR strTemp, strTemp2;
      CHECK(pUpd->GetProperty(CbsUpdateProperty::Name, &strTemp),
        "Failed to get raw name property of the update.");
      unique_malloc_ptr<wchar_t> uszName{strTemp};
      strTemp = nullptr;

      const int CBS_E_ARRAY_ELEMENT_MISSING = -2146498551;
      hr = pUpd->GetParentUpdate(0, &strTemp, &strTemp2);
      unique_malloc_ptr<wchar_t> uszParentName;
      unique_malloc_ptr<wchar_t> uszSet;
      if (SUCCEEDED(hr)) {
        uszParentName.reset(strTemp);
        uszSet.reset(strTemp2);
        strTemp = strTemp2 = nullptr;
      }

      const bool hasParent = hr != CBS_E_ARRAY_ELEMENT_MISSING;
      if (hasParent) {
        if (SUCCEEDED(hr))
          tree.AddDependency(uszParentName.get(), uszName.get());
        else {
          printf("Hey???\n");
        }
      } else {
        tree.AddDependency(tree.GetRoot(), uszName.get());
      }
    }
    return S_OK;
  };

  {
    ComPtr<IEnumCbsUpdate> pUpds;
    CHECK(pFound->EnumerateUpdates(CbsApplicability::Applicable, CbsSelectability::RootClass, &pUpds),
      "Failed to get the first batch of updates.");
    
    CHECK(joinUpdates(pUpds), "Failed to join the updates to the tree. [First]");
  }
  {
    ComPtr<IEnumCbsUpdate> pUpds;
    CHECK(pFound->EnumerateUpdates(CbsApplicability::NeedsParent, CbsSelectability::AllClass, &pUpds),
      "Failed to get the first batch of updates.");
    
    CHECK(joinUpdates(pUpds), "Failed to join the updates to the tree. [Second]");
  }

  std::function<void(std::wstring, std::wstring)> dfsTree;
  dfsTree = [&](std::wstring cur, std::wstring tabs) {
    std::wcout << tabs << ' ' << cur << '\n';
    for (auto& des : tree.GetDescendants(cur))
      dfsTree(des, tabs + L"====");
  };

  dfsTree(tree.GetRoot(), L"");

  CHECK(g_mgr.SubmitSess(), "Failed to submit our changes.");

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
  g_conf.SetLogFile(CBS_LOG_FILE);
  g_conf.mode = CCbsConfig::SessMode::Online;
  g_conf.stack_source = CCbsConfig::StackSource::SSShim;
  //g_conf.mode = CCbsConfig::SessMode::Offline;
  //g_conf.arg_path = L"D:\\MyCache\\Ws\\Windows";//L"C:\\Windows";
  //g_conf.arg_bootdrive = L"D:\\MyCache\\Ws";

  CHECK(g_mgr.FindStack(), "Failed to find stack [StackSource = %s].",
    GetEnumName(g_conf.stack_source).c_str());
  CHECK(g_mgr.Load(), "Failed to automatically load stack.");

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "We are to execute the task!");
  //system("pause");

  CHECK(DoTask_ConstructUpdateTree(), "User-defined operations in the task have NOT been performed fully.");

  /*CHECK(DecompressCabinetToDirectory(L"C:\\Users\\HigHwind\\Desktop\\tmp\\cab\\nt6.3\\windows8.1-kb4511515-x64_413f88394d618b3f07e1d24e606a6b164ff5a104.msu", L"C:\\Users\\HigHwind\\Desktop\\tmp\\cab\\nt6.3\\temp", [](UINT64 completed, UINT64 all){
    std::wcout << std::format(L"now prog: {:.1f}\n", 100.0*completed/all);
  }), "User-defined operations in the task have NOT been performed fully.");*/

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "We are to dispose the manager.");
  CHECK(g_mgr.Dispose(), "Failed to dispose StackManager.");

  if (g_pMalloc) {
    g_pMalloc->Release();
    g_pMalloc = nullptr;
  }

  CoUninitialize();

  LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "All resource cleared!");

  getchar();

  return S_OK;
}
