#pragma once

#include "isolation.h"

struct CSI_FILE
{
  
};

struct IEnumCSI_FILE : IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct CSIFILE* rgpCol, _Out_ ULONG* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCSI_FILE**) = 0;
};

struct ICSITransaction : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE InstallDeployment(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t const *,wchar_t const *,wchar_t const *,wchar_t const *,wchar_t const *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE PinDeployment(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t const *,wchar_t const *,wchar_t const *,wchar_t const *,wchar_t const *,unsigned __int64,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE UninstallDeployment(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t const *,wchar_t const *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE UnpinDeployment(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t const *,wchar_t const *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumMissingComponents(ULONG,IEnumDefinitionIdentity * *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE AddComponent(ULONG,IDefinitionIdentity *,wchar_t const *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumMissingFiles(ULONG, IEnumCSI_FILE * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE AddFile(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t const *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE Analyze(ULONG,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE Commit(ULONG,struct ICSITransactionCommitSink *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE Abort(ULONG,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE Scavenge(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t const *,ULONG *) = 0;
};

struct ICSIRepairTransaction : ICSITransaction {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

};

struct ICSIStore : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE BeginTransaction(ULONG, _GUID const&, LPCTSTR, _Out_ ICSITransaction**) = 0;
  virtual HRESULT STDMETHODCALLTYPE CancelPendingTransaction(ULONG, _GUID const&, LPCTSTR, _Out_ ULONG*) = 0;
  virtual HRESULT STDMETHODCALLTYPE BeginRepairTransaction(ULONG, _Out_ ICSIRepairTransaction**, _Out_ ULONG*) = 0;
  virtual HRESULT STDMETHODCALLTYPE CancelPendingRepairTransaction(ULONG, _Out_ ULONG*) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentManifests(ULONG, UINT64, IDefinitionIdentity**, _GUID const&, _Out_ IUnknown**) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentInstalledVersions(ULONG, UINT64, _Out_ IDefinitionIdentity**, _Out_ ULONG* const, _Out_ struct _COMPONENT_VERSION* const) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentInformation(ULONG, ULONG, IDefinitionIdentity*, UINT64, _Out_ void*) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReplaceMacros(ULONG, IDefinitionIdentity*, LPCTSTR, _Out_ LPTSTR*) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumPendingTransactions(ULONG, _GUID const&, _Out_ IUnknown**) = 0;
  virtual HRESULT STDMETHODCALLTYPE CancelPendingTransactions(ULONG, UINT64, _Out_ LPCTSTR const*, _Out_ ULONG*) = 0;
};


struct IStorePendingTransaction : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct IStorePendingStoreRepairTransaction : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct IStorePendingRepairTransaction : IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct IStore2 : IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE Transact(unsigned __int64,enum _STORE_TXN_OPERATION const * const,ULONG * const,long * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE BindReferenceToAssembly(ULONG,IReferenceIdentity *,unsigned __int64,IDefinitionIdentity * * const,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE CalculateDelimiterOfDeploymentsBasedOnQuota(ULONG,unsigned __int64,IDefinitionAppId * * const,struct _STORE_ASSEMBLY_INSTALLATION_REFERENCE const *,unsigned __int64,unsigned __int64 *,unsigned __int64 *,unsigned __int64 *) = 0;
  virtual HRESULT STDMETHODCALLTYPE BindDefinitions(ULONG,unsigned __int64,IDefinitionIdentity * * const,unsigned __int64,IDefinitionIdentity * * const,struct _ISTORE_BINDING_RESULT * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetAssemblyInformation(ULONG,IDefinitionIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumAssemblies(ULONG,IReferenceIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumFiles(ULONG,IDefinitionIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumInstallationReferences(ULONG,IDefinitionIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE LockAssemblyPath(ULONG,IDefinitionIdentity *,void * *,wchar_t * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReleaseAssemblyPath(void *) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(IStream * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumCategories(ULONG,IReferenceIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumSubcategories(ULONG,IDefinitionIdentity *,wchar_t const *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumCategoryInstances(ULONG,IDefinitionIdentity *,wchar_t const *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDeploymentProperty(ULONG,IDefinitionAppId *,struct _STORE_ASSEMBLY_INSTALLATION_REFERENCE const *,_GUID const &,wchar_t const *,tagBLOB *) = 0;
  virtual HRESULT STDMETHODCALLTYPE LockApplicationPath(ULONG,IDefinitionAppId *,void * *,wchar_t * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReleaseApplicationPath(void *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumPrivateFiles(ULONG,IDefinitionAppId *,IDefinitionIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumInstallerDeploymentMetadata(ULONG,struct _STORE_ASSEMBLY_INSTALLATION_REFERENCE const *,IReferenceAppId *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumInstallerDeploymentMetadataProperties(ULONG,struct _STORE_ASSEMBLY_INSTALLATION_REFERENCE const *,IDefinitionAppId *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateDeploymentReferences(ULONG,_GUID const &,IDefinitionAppId *,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateComponentDeployments(ULONG,_GUID const &,IDefinitionIdentity *,IReferenceAppId *,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateTopLevelComponents(ULONG,_GUID const &,IReferenceAppId *,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateRelatedComponentVersions(ULONG,IDefinitionIdentity *,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE StageAndLockComponents(ULONG,unsigned __int64,_GUID const &,IDefinitionIdentity * * const,void * * const,wchar_t const * * const,IUnknown * * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE UpgradeComponentLocksToPathLocks(ULONG,unsigned __int64,void * * const,wchar_t * * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE LockAssemblies(ULONG dwFlags,unsigned __int64 ComponentCount,IDefinitionIdentity ** const ComponentIdentities,void * * const,wchar_t const * * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReleaseAssemblyLocks(ULONG,unsigned __int64,void * * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetAssembliesInformation(ULONG,unsigned __int64,IDefinitionIdentity * * const,_GUID const &,IUnknown * * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE AreComponentsCompleteFromIdentities(ULONG,unsigned __int64,IDefinitionIdentity * * const,int * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE AreComponentsCompleteFromCookies(ULONG,unsigned __int64,void * * const,int * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentsInstalledVersions(ULONG,unsigned __int64,IDefinitionIdentity * * const,ULONG * const,struct _COMPONENT_VERSION * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentsStatus(ULONG,unsigned __int64,IDefinitionIdentity * * const,ULONG * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE Transact2(ULONG,wchar_t const *,unsigned __int64,_STORE_TXN_OPERATION const * const,ULONG * const,ULONG *,IStorePendingTransaction * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReplaceMacros(ULONG,IDefinitionIdentity *,wchar_t const *,wchar_t * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE RepairComponents(ULONG,unsigned __int64,enum _STORE_REPAIR_OPERATION const * const,ULONG * const,ULONG *,IStorePendingRepairTransaction * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDeploymentsClosureStatus(ULONG,unsigned __int64,IDefinitionAppId * * const,ULONG * const) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDeploymentsClosure(ULONG,unsigned __int64,IDefinitionAppId * * const,_GUID const &,IUnknown * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE RepairComponentStore(ULONG,ULONG *,IStorePendingStoreRepairTransaction * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE QueueComponent(enum ICSISmartInstaller_Operation,struct ICDF *) = 0;
};

template<class T>
struct Windows_Vector
{
  
};

//template<class T>
//struct Windows_Auto
//{
//  
//};

struct _LUNICODE_STRING
{
  int len;
  int max;
  PWSTR str;
};

template<class T>
using Windows_Auto = ComPtr<T>;

struct CCSDirect
{
  virtual HRESULT STDMETHODCALLTYPE Release(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreateInstance(const GUID&, struct Windows_Rtl_IRtlObject**) = 0;

  virtual BOOL STDMETHODCALLTYPE IsStoreOffline(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetStoreIdentifier(_GUID *) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreateTransaction(ULONG,struct Windows_ComponentStore_Rtl_IRtlTransactionHook *,struct Windows_ComponentStore_Rtl_IRtlTransaction **) = 0;
  virtual HRESULT STDMETHODCALLTYPE RepairComponents(ULONG,struct Windows_Vector<struct Windows_ComponentStore_Rtl_RepairOperation const> const &,struct Windows_Vector<ULONG> const &,struct Windows_ComponentStore_Rtl_IRtlRepairTransactionResult **) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentManifest(struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct WcpManifest *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentManifest(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *, tagBLOB*) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentManifest(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct Windows_Cdf_Rtl_IRtlCdf **) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateComponents(ULONG,struct Windows_Identity_Rtl_IRtlReferenceIdentity *,struct Windows_ComponentStore_Rtl_IRtlComponentEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateWinnersList(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct Windows_ComponentStore_Rtl_IRtlComponentEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateComponentFiles(ULONG,struct Windows_AppId_Rtl_IRtlDefinitionAppId *,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct Windows_ComponentStore_Rtl_IRtlComponentFileEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE IsComponentComplete(ULONG,struct Windows_AppId_Rtl_IRtlDefinitionAppId *,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,bool *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateComponentTopLevelComponents(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct Windows_AppId_Rtl_IRtlReferenceAppId *,struct Windows_ComponentStore_Rtl_IRtlTopLevelComponentEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateTopLevelComponentReferences(ULONG,struct Windows_AppId_Rtl_IRtlDefinitionAppId *,struct Windows_ComponentStore_Rtl_StoreInstallerReference const *,struct Windows_ComponentStore_Rtl_IRtlTopLevelComponentReferenceEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateTopLevelComponents(ULONG,struct Windows_AppId_Rtl_IRtlReferenceAppId *,struct Windows_ComponentStore_Rtl_IRtlTopLevelComponentEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentWinningVersion(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct _FOUR_PART_VERSION *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDefaultStorageArea(ULONG,struct Windows_ComponentStore_Rtl_IRtlComponentStore_StorageAreaInformation *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetComponentStatus(ULONG,struct Windows_Vector<struct Windows_Identity_Rtl_IRtlDefinitionIdentity *> const &,struct Windows_Vector<ULONG> const &) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDeploymentClosureStatus(ULONG,struct Windows_Vector<struct Windows_AppId_Rtl_IRtlDefinitionAppId *> const &,struct Windows_Vector<ULONG> const &) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetCompositionalClosure(ULONG,struct Windows_Vector<struct Windows_AppId_Rtl_IRtlDefinitionAppId *> const &,struct Windows_ComponentStore_Rtl_IRtlCompositionalClosureEnumerator **) = 0;
  virtual HRESULT STDMETHODCALLTYPE DetermineComponentLockPath(ULONG,struct Windows_Vector<struct Windows_Identity_Rtl_IRtlDefinitionIdentity *> const &,struct Windows_Vector<_LUNICODE_STRING> const &) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreateLockIdentifier(ULONG,struct Windows_ComponentStore_Rtl_ComponentStoreLockIdentifier *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetTemporaryDirectory(ULONG,struct Windows_Rtl_IRtlDirectory **,void * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReleaseTemporaryDirectory(void *, struct UpdateMgr, ...) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReleaseTemporaryDirectory(void *) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetIsolationLayer(struct Windows_Rtl_IRtlSystemIsolationLayer **) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetExternalDirectory(struct Windows_Rtl_IRtlDirectory **) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetExternalDirectoryPath(_LUNICODE_STRING const * *) = 0;
  virtual HRESULT STDMETHODCALLTYPE HasExternalDirectory(bool *) = 0;
  virtual HRESULT STDMETHODCALLTYPE ReleaseComponentAndPathLocks(ULONG,struct Windows_Vector<struct Windows_ComponentStore_Rtl_StoreOperation_UnlockSingleComponent const> const &,struct Windows_Vector<struct Windows_ComponentStore_Rtl_StoreOperation_UnlockSingleComponentPath const> const &,struct Windows_Vector<ULONG> const &,struct Windows_Vector<ULONG> const &) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumStoreCorruptions(ULONG,struct Windows_Vector<struct Windows_ComponentStore_Rtl_IRtlComponentStore_StoreCorruptionResult> &,struct Windows_Rtl_IRtlSystemIsolationLayer *&,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE RepairStoreCorruptions(ULONG,struct Windows_Vector<struct Windows_ComponentStore_Rtl_IRtlComponentStore_StoreCorruptionRepairOp> const &,struct Windows_Rtl_IRtlSystemIsolationLayer *&,struct Windows_Vector<ULONG> &,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE WofCompressFileIfShouldBeCompressed(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,_LUNICODE_STRING const &,struct Windows_Rtl_IRtlFile *) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnsureComponentFullyPresent(ULONG,struct Windows_Identity_Rtl_IRtlDefinitionIdentity *,struct WcpManifest const *,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE QueryConfigurationOption(_GUID const &,ULONG,ULONG *) = 0;
  virtual HRESULT STDMETHODCALLTYPE QueryConfigurationOption(_GUID const &,bool,bool *) = 0;
  virtual HRESULT STDMETHODCALLTYPE IsMobileStore(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE UseDeconstructedDrivers(void) = 0;
};

struct ISomeInterface : IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual CCSDirect* STDMETHODCALLTYPE GetStoreImplPtr(void) = 0;
};
