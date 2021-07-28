#pragma once
#include "pch.h"
#include <functional>

#pragma region CoreDLLs::Reference
extern HRESULT(WINAPI* vpfnCbsCoreInitialize)(IMalloc*, int(WINAPI*)(int), void (*)(), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
extern HRESULT(WINAPI* vpfnCbsCoreSetState)(int state, PROC value);
extern HRESULT(WINAPI* vpfnCbsCoreFinalize)();
extern HRESULT(WINAPI* vpfnSetTestMode)(int mode);

extern HRESULT(WINAPI* vpfnSxsStoreInitialize)(IMalloc*, int(WINAPI*)(int), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
extern HRESULT(*vpfnSxsStoreFinalize)();

extern HRESULT(WINAPI* vpfnWdsSetupLogMessageA)(LPVOID pMsg, enum WdsLogSource source, const char*, const char*, ULONG, const char* file, const char* func, void* CurIP, ULONG, void*, UINT);
extern LPVOID(WINAPI* vpfnConstructPartialMsgVA)(enum WdsLogLevel level, const char* fmtMsg, va_list va);
extern LPVOID(*vpfnCurrentIP)();

extern int(WINAPI* vpfnGetSystemStore)(unsigned int, const struct _GUID&, struct IUnknown**);

extern const GUID CLSID_CbsSession;

extern const GUID IID_ICbsSession;
extern const GUID IID_ICbsSessionPrivate;
extern const GUID IID_ICbsSession9;
extern const GUID IID_IServicingQuerier;
extern const GUID IID_ISxSStore;
extern const GUID IID_ICSIStore;
extern const GUID IID_ICbsUIHandler;
extern const GUID IID_IStore;
extern const GUID IID_IStore2;
extern const GUID IID_ICSISmartInstaller;
#pragma endregion

int NilFunc1(int);
void NilFunc2();

class StackManager {
  bool bStackFound = false,
    bCbsCoreLoaded = false,
    bSxSStoreLoaded = false,
    bSSShimLoaded = false,
    bCbsCoreInited = false,
    bSxSStoreInited = false,
    bWdsCoreInited = false,
    bWcpInited = false;
  std::wstring ServicingStack;
  std::wstring CbsCore;
  std::wstring SxSStore;

  IClassFactory* pCbsCoreFactory = nullptr;
  IClassFactory* pSxSFactory = nullptr;

  ICSIStore* pCSIStore = nullptr;
  ICbsSession* pSess = nullptr;

  HRESULT FindStackByReg();
  HRESULT FindStackByPath(std::wstring path);
  HRESULT FindStackBySSShim(std::wstring path);

  // if you don't specify strBootDrive argument,
  // the session will be online.
  HRESULT ApplySess(CbsSessionOption opt,
    const std::wstring& strClientId = L"CallCbsCore",
    const std::wstring& strBootDrive = L"");
public:
  // set them if you need, but null is valid.
  struct Callbacks {
    decltype(&NilFunc1) LockProcCB;
    decltype(&NilFunc2) UnlockProcCB, InstCreatedCB, InstDestroyedCB,
      ReqShutdownNowCB, ReqShutdownProcessingCB;
  } CoreEvent = {};

  [[nodiscard]] ICbsSession* GetActiveSess() const;
  HRESULT GetNewSess(CbsSessionOption opt = CbsSessionOption::None);
  HRESULT SubmitSess();

  HRESULT FindStack();

  HRESULT LoadCbsCore();
  HRESULT LoadSxSStore();
  HRESULT LoadSSShim();

  HRESULT InitCbsCore();
  HRESULT InitSxSStore();
  HRESULT InitWdsCore();
  HRESULT InitWcp();

  // opt: seems must be 0, iid = IID_ICSIExternalTransformerExecutor or IID_ICSIStore
  HRESULT GetSystemStore(UINT opt, const _GUID& iid, IUnknown** ppOut) const;

  // Default Settings
  HRESULT Load();
  HRESULT Dispose();

  template<class FuncT>
  [[nodiscard]] static std::function<FuncT> GetFunction(const std::wstring module, const std::string name) {
    return reinterpret_cast<FuncT*>(GetProcAddress(GetModuleHandle(module.c_str()), name.c_str()));
  }

  ~StackManager() { Dispose(); }
};

extern StackManager g_mgr;

#define g_sess (g_mgr.GetActiveSess())
