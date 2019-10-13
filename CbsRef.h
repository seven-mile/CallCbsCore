#pragma once

extern HRESULT(__stdcall* vpfnCbsCoreInitialize)(IMalloc*, int(__stdcall*)(int), void (*)(), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
extern HRESULT(__stdcall* vpfnCbsCoreSetState)(int state, PROC value);
extern HRESULT(__stdcall* vpfnCbsCoreFinalize)();
extern HRESULT(__stdcall* vpfnSetTestMode)(int mode);

extern HRESULT(__stdcall* vpfnSxsStoreInitialize)(IMalloc*, int(__stdcall*)(int), void (*)(), void (*)(), void (*)(), void (*)(), IClassFactory**);
extern HRESULT(*vpfnSxsStoreFinalize)();

extern HRESULT(__stdcall* vpfnWdsSetupLogMessageA)(LPVOID pMsg, enum WdsLogSource source, const char*, const char*, ULONG, const char* file, const char* func, void* CurIP, ULONG, void*, UINT);
extern LPVOID(__stdcall* vpfnConstructPartialMsgVA)(enum WdsLogLevel level, const char* fmtMsg, va_list va);
extern LPVOID (*vpfnCurrentIP)();

extern int(__stdcall* vpfnGetSystemStore)(unsigned int, const struct _GUID&, struct IUnknown**);

extern IClassFactory* pCbsCoreFactory;
extern IClassFactory* pSxSFactory;

extern ICbsSession* pCbsSession;
extern ICbsSessionInternal* pCbsSessionIn;

extern ICSIStore* pCSIStore;

extern const GUID CLSID_CbsSession;

extern const GUID IID_ICbsSession;
extern const GUID IID_ICbsSessionPrivate;
extern const GUID IID_ICbsSession9;
extern const GUID IID_IServicingQuerier;
extern const GUID IID_ISxSStore;
extern const GUID IID_ICSIStore;
extern const GUID IID_ICbsUIHandler;

HRESULT FindStackByReg();
HRESULT LoadCbsCore();
HRESULT LoadSxSStore();
HRESULT LoadWdsCore();
HRESULT LoadWcp();
HRESULT InitCbsCoreAndSxSStore();
HRESULT OpenOnlineSession(_CbsSessionOption option = CbsSessionOptionNone);
HRESULT OpenOfflineSession(LPCTSTR szBootDrive, LPCTSTR szWindir, _CbsSessionOption option = CbsSessionOptionNone);
HRESULT CloseSessionAndFinalizeCbsCore();
