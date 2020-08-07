#pragma once
#include "pch.h"

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
#pragma endregion

int NilFunc1(int);
void NilFunc2();

class StackManager {
  bool bStackFound = false,
    bCbsCoreLoaded = false,
    bSxSStoreLoaded = false,
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
  HRESULT ApplySess(_CbsSessionOption opt,
    const std::wstring strClientId = L"CallCbsCore",
    const std::wstring strBootDrive = L"");
public:
  // set them if you need, but null is valid.
  struct Callbacks {
    decltype(&NilFunc1) LockProcCB;
    decltype(&NilFunc2) UnlockProcCB, InstCreatedCB, InstDestroyedCB,
      ReqShutdownNowCB, ReqShutdownProcessingCB;
  } CoreEvent = {};

  ICbsSession* GetActiveSess() {
    if (pSess) return pSess;
    RET_LOG(nullptr, E_NOT_VALID_STATE, "No active session available!");
  }
  HRESULT GetNewSess(_CbsSessionOption opt = CbsSessionOptionNone);
  HRESULT SubmitSess();

  HRESULT FindStack();

  HRESULT LoadCbsCore();
  HRESULT LoadSxSStore();

  HRESULT InitCbsCore();
  HRESULT InitSxSStore();
  HRESULT InitWdsCore();
  HRESULT InitWcp();

  // Default Settings
  HRESULT Load();
  HRESULT Dispose();

  ~StackManager() { Dispose(); }
};

extern StackManager g_mgr;

#define g_sess (g_mgr.GetActiveSess())
