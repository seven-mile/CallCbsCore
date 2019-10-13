#pragma once

class CCbsUIHandlerImpl :
	public ICbsUIHandler
{
private:
	ULONG m_dwRef = 0;

public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObject) override;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override;
	virtual ULONG STDMETHODCALLTYPE Release(void) override;

	virtual HRESULT STDMETHODCALLTYPE Initiate(IEnumCbsUpdate*, int*) override;
	virtual HRESULT STDMETHODCALLTYPE Terminate(void) override;
	virtual HRESULT STDMETHODCALLTYPE Error(HRESULT, LPCTSTR, int*) override;
	virtual HRESULT STDMETHODCALLTYPE ResolveSource(LPCTSTR, ICbsIdentity*, LPCTSTR, LPTSTR*, int*) override;
	virtual HRESULT STDMETHODCALLTYPE Progress(_CbsInstallState, UINT, UINT, int*) override;
	virtual HRESULT STDMETHODCALLTYPE EnteringStage(UINT, enum _CbsOperationStage, int, int) override;
	virtual HRESULT STDMETHODCALLTYPE ProgressEx(_CbsInstallState, UINT, UINT, UINT, int*) override;

	CCbsUIHandlerImpl();
	virtual ~CCbsUIHandlerImpl() = default;
};

