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
	return *ppvObject = NULL, E_NOINTERFACE;
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
	_tprintf(_T("Init: pUpds = %p\n"), pUpds);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Terminate(void)
{
	_tprintf(_T("Term: Operation(s) completed.\n"));
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Error(HRESULT hre, LPCTSTR szUnk, int* pUnk)
{
	_tprintf(_T("Error: HRESULT: %x, %s\n"), hre, szUnk);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::ResolveSource(LPCTSTR szUnk1, ICbsIdentity* pIdent, LPCTSTR szUnk2, LPTSTR* pszUnk, int* pUnk)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::Progress(_CbsInstallState insSt, UINT curProg, UINT totProg, int* pUnk)
{
	BEGIN_ERROR_HANDLING();

	LPTSTR strp;
	CHECK(TextizeCbsInstallState(insSt, &strp), "Failed to textize _CbsInstallState.");
	_tprintf(_T("Prog: State = %s, %.1f%% completed.\n"), strp, 100.0 * curProg / totProg);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::EnteringStage(UINT unk1, _CbsOperationStage opSg, int unk2, int unk3)
{
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CCbsUIHandlerImpl::ProgressEx(_CbsInstallState insSt, UINT curProg, UINT totProg, UINT unk3, int* pUnk)
{
	BEGIN_ERROR_HANDLING();

	LPTSTR strp;
	CHECK(TextizeCbsInstallState(insSt, &strp), "Failed to textize _CbsInstallState.");
	_tprintf(_T("ProgEx: State = %s, %.1f%% completed.\n"), strp, 100.0 * curProg / totProg);
	return S_OK;
}

CCbsUIHandlerImpl::CCbsUIHandlerImpl()
{
	m_dwRef = 0;
}
