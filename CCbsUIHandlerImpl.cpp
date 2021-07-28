#include "pch.h"
#include "CCbsUIHandlerImpl.h"

HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::QueryInterface(REFIID riid, LPVOID* ppvObject)
{
  if (riid == IID_ICbsUIHandler || riid == IID_IUnknown)
  {
    *ppvObject = (LPVOID)this;
    this->AddRef();
    return S_OK;
  }
  return *ppvObject = nullptr, E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CCbsUIHandlerImpl::AddRef(void)
{
  return InterlockedIncrement(&m_dwRef);
}

ULONG STDMETHODCALLTYPE CCbsUIHandlerImpl::Release(void)
{
  ULONG retCnt = InterlockedDecrement(&m_dwRef);
  if (retCnt == 0)
  {
    delete this;
  }
  return retCnt;
}

HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Initiate(IEnumCbsUpdate* pUpds, int* pUnk)
{
  _tprintf(_T("(%S) Init: pUpds = %p\n"), m_name.c_str(), pUpds);
  return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Terminate(void)
{
  _tprintf(_T("(%S) Term: Operation(s) completed.\n"), m_name.c_str());
  return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Error(HRESULT hre, PCWSTR szUnk, int* pUnk)
{
  _tprintf(_T("(%S) Error: HRESULT: %x, %s\n"), m_name.c_str(), hre, szUnk);
  return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::ResolveSource(PCWSTR szUnk1, ICbsIdentity* pIdent, PCWSTR szUnk2, PWSTR* pszUnk, int* pUnk)
{
  return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Progress(CbsInstallState insSt, UINT curProg, UINT totProg, int* pUnk)
{
  _tprintf(_T("(%S) Prog: State = %S, %.1f%% completed.\n"),
    m_name.c_str(), GetEnumName(insSt).c_str(), 100.0 * curProg / totProg);
  return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::EnteringStage(UINT unk1, CbsOperationStage opSg, int unk2, int unk3)
{
  return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::ProgressEx(CbsInstallState insSt, UINT curProg, UINT totProg, UINT unk3, int* pUnk)
{
  _tprintf(_T("(%S) ProgEx: State = %S, %.1f%% completed.\n"),
    m_name.c_str(), GetEnumName(insSt).c_str(), 100.0 * curProg / totProg);
  return S_OK;
}

CCbsUIHandlerImpl::CCbsUIHandlerImpl(std::string name)
{
  m_dwRef = 0;

  static size_t nAllocId = 0;

  if (name == "__und__")
    name = "UIHandler" + NUM_TO_STR(nAllocId++);

  m_name = name;
}
