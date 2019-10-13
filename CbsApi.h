#pragma once

#include <isolation.h>

#pragma region Enumerations

enum CbsState {
	CbsStateResolvedInvalid = -32,
	CbsStateStagedInvalid = -64,
	CbsStateInstalledInvalid = -112,
	CbsStatePermanentInvalid = -128,

	CbsStateAbsent = 0,
	CbsStateUninstallPending = 5,
	CbsStateResolving = 16,
	CbsStateResolved = 32,
	CbsStateStaging = 48,
	CbsStateStaged = 64,
	CbsStateSuperseded = 80,
	CbsStateInstallPending = 96,
	CbsStatePartiallyInstalled = 101,
	CbsStateInstalled = 112,
	CbsStatePermanent = 128,
	CbsStateDefault = 4096,
	CbsStateDetect = 4100,
	CbsStateCanceled = 4101
};

enum UpdateSelection {
	UpdateSelectionUnknown = -1,
	UpdateSelectionOff = 0,
	UpdateSelectionDefault = 255,
	UpdateSelectionOn = 1
};

enum _CbsPackageType {
	CbsPackageTypeUnknown = -2,
	CbsPackageTypeExisting = -1,
	CbsPackageTypeCabinet = 0,
	CbsPackageTypeExpanded = 1,
	CbsPackageTypeManifest = 2,
	CbsPackageTypeXmlString = 3,
	CbsPackageTypeExpandedWithMum = 4
};

enum _CbsRequiredAction {
	CbsRequiredActionNone = 0,
	CbsRequiredActionReboot = 1
};

enum _CbsSessionProperty {
	CbsSessionPropertyRebootRequired = 1,
	CbsSessionPropertyErrorDetail = 2,
	CbsSessionPropertyServiceable = 3,
	CbsSessionPropertyCompressionEnabled = 4,
	CbsSessionPropertyReport = 5,
	CbsSessionPropertyCorruptionFlag = 6,

	CbsSessionPropertyVolatileSize = 8,
	CbsSessionPropertyNonVolatileSize = 9,
	CbsSessionPropertySharedWithWindowsSize = 10,
	CbsSessionPropertyAccordingToExplorer = 11,
	CbsSessionPropertyLastScavengeDatetime = 12,
	CbsSessionPropertySupersededPackageCount = 13,

	CbsSessionPropertySessionCompletionDatatime = 15,
	CbsSessionPropertyPackageMinSize = 16,
	// ?
	CbsSessionPropertyTotalPackageMinSize = 18,

	CbsSessionPropertyRepairNeeded = 22,
	CbsSessionPropertyReOffer = 23
};

enum _CbsSessionOption : DWORD {
	CbsSessionOptionNone = 0,
	CbsSessionOptionLoadPersisted = 0x80,
	CbsSessionOptionDoScavenge = 0x400,
	CbsSessionOptionCancelAllPendedTransactions = 0x800,
	CbsSessionOptionEnableCompression = 0x2000,
	CbsSessionOptionDisableCompression = 0x4000,
	CbsSessionOptionDetectAndRepairStoreCorruption = 0xC000,
	CbsSessionOptionReportStackInfo = 0x100000,
	CbsSessionOptionDoSynchronousCleanup = 0x400000,
	CbsSessionOptionAnalyzeComponentStore = 0x8000000,
	CbsSessionOptionCancelOnlySmartPendedTransactions = 0x80000000
};

enum _CbsSessionEnhancedOption : DWORD {
	CbsSessionEnhancedOptionManuallyRepair = 0x800
};

enum _CbsSessionState {
	CbsSessionStateUnknown = 0,
	CbsSessionStateReady = 16,
	CbsSessionStateQueued = 32,
	CbsSessionStateStarted = 48,
	CbsSessionStatePanned = 64,
	CbsSessionStateResolved = 80,
	CbsSessionStateStaged = 96,
	CbsSessionStateExecutionDelayed = 101,
	CbsSessionStateInstalled = 112,
	CbsSessionStateShutdownStart = 144,
	CbsSessionStateShutdownFinish = 160,
	CbsSessionStateStartup = 176,
	CbsSessionStateStartupFinish = 192,
	CbsSessionStateComplete = 208,
	CbsSessionStateInterrupted = 224,
	CbsSessionStateCorrupted = 240,
	CbsSessionStateMarkedForRetry = 256
};


enum _CbsOperationType {
	CbsOperationTypeNone = 0,
	CbsOperationTypeExportRepository = 1,
	CbsOperationTypeUpdateImage = 2,
	CbsOperationTypePerformPrepareServicingOperation = 3,
	CbsOperationTypePerformLateAcquisitionOperation = 4,
	CbsOperationTypeInitICSIStore = 8
};

enum _CbsSessionConfigurableProperty {

};

enum _CbsApplicability {
	CbsApplicabilityInvalidValue = -1,
	CbsApplicabilityNotApplicable = 0,
	CbsApplicabilityNeedsParent = 2,
	CbsApplicabilityApplicable = 4
};

enum _CbsSelectability {
	CbsSelectabilityInvalidValue = -1,
	CbsSelectabilityClass1 = 1,
	CbsSelectabilityClass2 = 2
};

enum _CbsInstallState {
	CbsInstallStatePartiallyInstalled = -19,
	CbsInstallStateCancel = -18,
	CbsInstallStateSuperseded = -17,
	CbsInstallStateDefault = -16,
	CbsInvalidStatePermanent = -8,
	CbsInvalidStateInstalled = -7,
	CbsInvalidStateStaged = -4,
	CbsInvalidStateResolved = -2,
	CbsInstallStateUnknown = -1,

	CbsInstallStateAbsent = 0,

	CbsInstallStateResolving = 1,
	CbsInstallStateResolved = 2,
	CbsInstallStateStaging = 3,
	CbsInstallStateStaged = 4,
	CbsInstallStateUninstallRequested = 5,
	CbsInstallStateInstallRequested = 6,
	CbsInstallStateInstalled = 7,
	CbsInstallStatePermanent = 8,
};

enum _CbsPackageProperty {
	CbsPackagePropertyIdentityString = 1,
	CbsPackagePropertyDisplayName = 2,
	CbsPackagePropertyDescription = 3,
	CbsPackagePropertyRestart = 4,
	CbsPackagePropertyPsfName = 5,
	CbsPackagePropertyHidden = 6,
	CbsPackagePropertyKeyword = 7,
	CbsPackagePropertyReleaseType = 8,
	CbsPackagePropertyProductName = 9,
	CbsPackagePropertyProductVersion = 10,
	CbsPackagePropertyPermenence = 11,
	CbsPackagePropertyCompany = 12,
	CbsPackagePropertyCopyright = 13,
	CbsPackagePropertySupportInformation = 14,
	CbsPackagePropertyCreationTimeStamp = 15,
	CbsPackagePropertyLastUpdateTimeStamp = 16,
	CbsPackagePropertyInstallTimeStamp = 17,
	CbsPackagePropertyInstallPackageName = 18,
	CbsPackagePropertyInstallLocation = 19,
	CbsPackagePropertyInstallClient = 20,
	CbsPackagePropertyInstallUserName = 21,
	CbsPackagePropertyExtendedError = 22,
	CbsPackagePropertyPended = 23,
	CbsPackagePropertyHotpatch = 24,
	CbsPackagePropertyExclusive = 25,
	CbsPackagePropertyAllowedOffline = 26,
	CbsPackagePropertyCompletelyOfflineCapable = 27,
	CbsPackagePropertyScavengeSequence = 28,
	CbsPackagePropertyPackageSize = 29,
	CbsPackagePropertySupersededTime = 30,
	CbsPackagePropertyStackUpdate = 31,
	CbsPackagePropertyReleaseQuality = 32,
	CbsPackagePropertyTargetPartition = 33,
	CbsPackagePropertyBinaryPartition = 34,
	CbsPackagePropertyCapabilityIdentity = 35,
	CbsPackagePropertyInternalStagingSizeWithExternalStorage = 36,
	CbsPackagePropertyInternalStagingSizeWithoutExternalStorage = 37,
	CbsPackagePropertyInstallSizeWithExternalStorage = 38,
	CbsPackagePropertyInstallSizeWithoutExternalStorage = 39,
	CbsPackagePropertyExternalStagingSize = 40,
	CbsPackagePropertyFeatureFMID = 41,
	CbsPackagePropertyFeatureGroup = 42,
	CbsPackagePropertyPermanentUntilReset = 43,
	CbsPackagePropertySatelliteType = 44,
	CbsPackagePropertyDeclareSatelliteLanguage = 45,
	CbsPackagePropertyDeclareSatelliteArch = 46,
	CbsPackagePropertyRemoveOnReset = 51
};

enum _CbsResourceType {

};

enum _CbsCardinality {

};

enum _CbsUpdateProperty {
	CbsUpdatePropertyName = 1,
	CbsUpdatePropertyDisplayName = 2,
	CbsUpdatePropertyDescription = 3,
	CbsUpdatePropertyDisplayFile = 4,
	CbsUpdatePropertyRestart = 5,
	CbsUpdatePropertyPsfName = 6,
	CbsUpdatePropertyDownloadSize = 7,
	CbsUpdatePropertySetMembership = 8
};

enum tagCbsPackageDecryptionData {

};

enum tagCBS_PACKAGE_ENCRYPTION_ENUM {

};

enum _CbsPackageChangeOption {
	CbsTreatPackageAsPSFX = 1 << 15
};

enum _CbsOperationStage {
	CbsOperationStageWaiting = 1,
	CbsOperationStagePlanning = 5,
	CbsOperationStageDownloading = 15,
	CbsOperationStageExtracting = 20,
	CbsOperationStageResolving = 25,
	CbsOperationStageStaging = 30,
	CbsOperationStageInstalling = 40,
	CbsOperationStageInstallingEx = 50,
	CbsOperationStageReservicingLCU = 60
};

enum _CbsCapabilitySourceFilter {
	CbsOnDemandSourceActionList = 0x10,
	CbsOnDemandSourceWindowsUpdate = 0x100,
	CbsOnDemandSourceRemovedMarkers = 0x1000,

};

#pragma endregion

struct _SXS_STORE_INSTALL_SOURCE_INFO {

};

struct _SXS_STORE_INSTALL_REFERENCEW {

};

struct ICbsIdentity : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Clear(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE IsNull(_Out_ BOOL* pbIsNull) = 0;
	virtual HRESULT STDMETHODCALLTYPE IsEqual(_Out_ ICbsIdentity* pIdentComp, _Out_ BOOL* pbIsEqual) = 0;
	virtual HRESULT STDMETHODCALLTYPE LoadFromAttributes(LPCTSTR szName, LPCTSTR szPublicKeyToken, LPCTSTR szProcessor, LPCTSTR szLanguage, LPCTSTR szVersion) = 0;
	virtual HRESULT STDMETHODCALLTYPE LoadFromStringId(LPCTSTR szID) = 0;
	virtual HRESULT STDMETHODCALLTYPE SaveAsStringId(_Out_ LPTSTR* pszID) = 0;
	virtual HRESULT STDMETHODCALLTYPE InternalIsEqualbyAttribute(LPCTSTR szName, LPCTSTR szPublicKeyToken, LPCTSTR szProcessor, LPCTSTR szLanguage, LPCTSTR szVersion, _Out_ BOOL* pbIsEqual) = 0;
	virtual BOOL STDMETHODCALLTYPE IsMoniker(void) = 0;
};

struct ICbsCapability : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE GetIdentity(ICbsIdentity**) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProperty(_CbsPackageProperty, _Out_ LPTSTR*) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumerateUpdates(_CbsApplicability pApplicability, _CbsSelectability pSelectability, _Out_ struct IEnumCbsUpdate** ppUpdateList) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUpdate(LPCTSTR szUpdName, _Out_ struct ICbsUpdate** ppUpdate) = 0;
	virtual HRESULT STDMETHODCALLTYPE AddSource(LPCTSTR basePath) = 0;
	virtual HRESULT STDMETHODCALLTYPE RemoveSource(LPCTSTR basePath) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumerateSources(_Out_ IEnumString** pSourceList) = 0;
	virtual HRESULT STDMETHODCALLTYPE EvaluateApplicability(UINT option, _Out_ _CbsInstallState* pApplicabilityState, _Out_ _CbsInstallState* pCurrentState) = 0;
	virtual HRESULT STDMETHODCALLTYPE InitiateChanges(UINT installOptions, _CbsInstallState targetState, _In_ struct ICbsUIHandler* pUIHandler) = 0;
	virtual HRESULT STDMETHODCALLTYPE Status(_Out_ _CbsInstallState* pProgressState, _Out_ DWORD* pLastError) = 0;
	virtual HRESULT STDMETHODCALLTYPE ResourcesToCheck(_CbsResourceType resType, _Out_ IEnumString** pResList) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCapability(_Out_ LPTSTR* pszNamespace, _Out_ LPTSTR* pszLang, _Out_ LPTSTR* pszArch, ULONG* dwVerMajor, ULONG* dwVerMinor) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDependencies(struct IEnumCbsCapability** pDependenciesList) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetSources(UINT*) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDownloadSize(ULONG* nBytes) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetInstallSize(ULONG* nBytes) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetInstallState(_CbsInstallState* pState) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetOwnerInformation(UINT reserved, int*, UINT*, _Out_ LPTSTR*) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumerateFeaturePackages(struct IEnumCbsFeaturePackage** ppFeaturePkgList) = 0;

	virtual VOID STDMETHODCALLTYPE InternalAddRef(void) = 0;
	virtual VOID STDMETHODCALLTYPE InternalRelease(void) = 0;
	// provides 80255a37_11ed_4b44_a388_87fe71de6306 (ICbsCustomInformation)
	virtual HRESULT STDMETHODCALLTYPE ExtendedQueryInterface(_GUID const&, LPVOID* ppvObject) = 0;
	virtual VOID STDMETHODCALLTYPE DumpInformation() = 0;
};

struct ICbsPackage : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE GetIdentity(_Out_ ICbsIdentity** pIdentity) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProperty(_CbsPackageProperty prop, _Out_ LPTSTR* pValue) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumerateUpdates(_CbsApplicability pApplicability, _CbsSelectability pSelectability, _Out_ struct IEnumCbsUpdate** ppUpdateList) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetUpdate(LPCTSTR szUpdName, _Out_ struct ICbsUpdate** ppUpdate) = 0;
	virtual HRESULT STDMETHODCALLTYPE AddSource(LPCTSTR basePath) = 0;
	virtual HRESULT STDMETHODCALLTYPE RemoveSource(LPCTSTR basePath) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnumerateSources(_Out_ IEnumString** pSourceList) = 0;
	virtual HRESULT STDMETHODCALLTYPE EvaluateApplicability(UINT option, _Out_ _CbsInstallState* pApplicabilityState, _Out_ _CbsInstallState* pCurrentState) = 0;
	// installOptions: \in 0xFF7F
	virtual HRESULT STDMETHODCALLTYPE InitiateChanges(UINT installOptions, _CbsInstallState targetState, _In_ struct ICbsUIHandler* pUIHandler) = 0;
	virtual HRESULT STDMETHODCALLTYPE Status(_Out_ _CbsInstallState* pProgressState, _Out_ DWORD* pLastError) = 0;
	virtual HRESULT STDMETHODCALLTYPE ResourcesToCheck(_CbsResourceType resType, _Out_ IEnumString** pResList) = 0;
	virtual STDMETHODCALLTYPE ~ICbsPackage() = 0;
	virtual VOID STDMETHODCALLTYPE InternalAddRef(void) = 0;
	virtual VOID STDMETHODCALLTYPE InternalRelease(void) = 0;
	// provides 80255a37_11ed_4b44_a388_87fe71de6306 (ICbsCustomInformation)
	virtual HRESULT STDMETHODCALLTYPE ExtendedQueryInterface(_GUID const&, _Out_ LPVOID* ppvObject) = 0;
	virtual VOID STDMETHODCALLTYPE DumpInformation() = 0;
};

struct ICbsUpdate : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE GetProperty(_CbsUpdateProperty prop, _Out_ LPTSTR* pValue) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetPackage(_Out_ ICbsPackage** ppPackage) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetParentUpdate(UINT index, _Out_ LPTSTR* ppParent, _Out_ LPTSTR* ppSet) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCapability(_Out_ _CbsApplicability* pApplicability, _Out_ _CbsSelectability* pSelectability) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetDeclaredSet(UINT unk, _Out_ LPTSTR* pDeclaredSet, _Out_ _CbsCardinality* Cardinality) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetInstallState(_Out_ _CbsInstallState* pCurrentState, _Out_ _CbsInstallState* pIntendedState, _Out_ _CbsInstallState* pRequestState) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetInstallState(UINT options, _CbsInstallState state) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetRealInstallState(_Out_ CbsState* pCurrentState, _Out_ CbsState* pIntendedState, _Out_ UpdateSelection* pRequestState) = 0;
	virtual VOID STDMETHODCALLTYPE DumpInformation(void) = 0;
	virtual STDMETHODCALLTYPE ~ICbsUpdate() = 0;
	virtual VOID STDMETHODCALLTYPE InternalAddRef(void) = 0;
	virtual VOID STDMETHODCALLTYPE InternalRelease(void) = 0;
	// provides 80255a37_11ed_4b44_a388_87fe71de6306
	virtual HRESULT STDMETHODCALLTYPE ExtendedQueryInterface(_GUID const&, _Out_ LPVOID* ppvObject) = 0;

};


// Unusable abstract callback interface, you should implement it
struct ICbsUIHandler : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Initiate(IEnumCbsUpdate* pUpdList, int*) = 0;
	virtual HRESULT STDMETHODCALLTYPE Terminate(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Error(HRESULT hr, LPCTSTR szUnk, int*) = 0;
	virtual HRESULT STDMETHODCALLTYPE ResolveSource(LPCTSTR, ICbsIdentity*, LPCTSTR, LPTSTR*, int*) = 0;
	virtual HRESULT STDMETHODCALLTYPE Progress(_CbsInstallState stIns, UINT curProg, UINT totProg, int*) = 0;
	virtual HRESULT STDMETHODCALLTYPE EnteringStage(UINT, enum _CbsOperationStage, int, int) = 0;
	virtual HRESULT STDMETHODCALLTYPE ProgressEx(_CbsInstallState stIns, UINT curProg, UINT totProg, UINT, int*) = 0;
};

// I don't know how to use it
struct ICSIExternalTransformerExecutor : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Commit(LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR) = 0;
	virtual HRESULT STDMETHODCALLTYPE Initialize(ULONG, UINT64, LPCTSTR, LPCTSTR, LPCTSTR) = 0;
	virtual HRESULT STDMETHODCALLTYPE Install(LPCTSTR, LPCTSTR) = 0;
};

struct ICbsCustomInformation : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

};

struct ICbsSessionObserverListener : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct ICbsActivity : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct ICbsFeaturePackage : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct IEnumCbsCapability : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct ICbsCapability** rgpCol, _Out_ ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsCapability**) = 0;
};

struct IEnumIdentity : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct ICbsIdentity** rgpCol, _Out_ ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumIdentity**) = 0;
};

struct IEnumCbsIdentity : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct ICbsIdentity** rgpCol, _Out_ ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsIdentity**) = 0;
};

struct IEnumCbsSession : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct ICbsSession** rgpCol, _Out_ ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsIdentity**) = 0;
};

struct IEnumCbsUpdate : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct ICbsUpdate** rgpCol, _Out_ ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsUpdate**) = 0;
};

struct IEnumCbsActivity : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, _Out_ struct ICbsActivity** rgpCol, _Out_ ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsActivity**) = 0;
};

struct IEnumCbsFeaturePackage : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, struct ICbsFeaturePackage** rgpCol, ULONG* pbFetched) = 0;
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt) = 0;
	virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumCbsFeaturePackage**) = 0;
};

struct ICbsSession : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE Initialize(_In_ _CbsSessionOption sessionOptions, _In_ LPCTSTR clientID, _In_opt_ LPCTSTR bootDrive, _In_opt_ LPCTSTR winDir) = 0;
	virtual HRESULT STDMETHODCALLTYPE Finalize(_In_ _CbsRequiredAction* pRequiredAction) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreatePackage(_In_ UINT options, _In_ _CbsPackageType packageType, _In_ LPCTSTR szPkgPath, _In_ LPCTSTR szSandboxPath, _Out_ ICbsPackage** pPackage) = 0;
	// options: 0 and 2 bit (0 | 1 | 4), unkArg will only appear in error log information
	virtual HRESULT STDMETHODCALLTYPE OpenPackage(_In_ UINT options, _In_ ICbsIdentity* pPackageIdentity, _In_ LPCTSTR unkArgAboutLog, _Out_ ICbsPackage** pPackage) = 0;
	// options: normal: 0x1b0, all: 0x50 or 0x70
	virtual HRESULT STDMETHODCALLTYPE EnumeratePackages(_In_ UINT options, _Out_ IEnumCbsIdentity** pPackageList) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateCbsIdentity(_Out_ ICbsIdentity** ppIdentity) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetStatus(_Out_ UINT* pCurrentPhase, _Out_ _CbsSessionState* pLastSuccessfulSessionState, _Out_ BOOL* pbCompleted, _Out_ HRESULT* phrStatus) = 0;
	virtual HRESULT STDMETHODCALLTYPE Resume(_In_ ICbsUIHandler* pUIHandler) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetSessionId(_In_ LPTSTR* pszIdentity) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetProperty(_In_ _CbsSessionProperty prop, _Out_ LPTSTR* pValue) = 0;
	virtual HRESULT STDMETHODCALLTYPE AddPhaseBreak(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE FinalizeEx(_In_ UINT, _Out_ _CbsRequiredAction* pRequiredAction) = 0;
	// Session7 is here
	virtual HRESULT STDMETHODCALLTYPE AddSource(_In_ UINT options, _In_ LPCTSTR basePath) = 0;
	virtual HRESULT STDMETHODCALLTYPE RegisterCbsUIHandler(_In_ ICbsUIHandler* pUIHandler) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateWindowsUpdatePackage(_In_ UINT, _In_ LPCTSTR, _In_ GUID, _In_ UINT, _In_ _CbsPackageType, _In_ LPCTSTR, _In_ LPCTSTR, _In_ UINT, _Out_ tagCbsPackageDecryptionData* const, _In_ tagCBS_PACKAGE_ENCRYPTION_ENUM, _Out_ ICbsPackage**) = 0;
	// sourceFilter: \in 0xFF, 0x40 means LanguagePacks
	virtual HRESULT STDMETHODCALLTYPE EnumerateCapabilities(_In_ UINT sourceFilter, _In_ LPCTSTR, _In_ LPCTSTR, _In_ LPCTSTR, _In_ ULONG, _In_ ULONG, _Out_ IEnumCbsCapability**) = 0;
	virtual HRESULT STDMETHODCALLTYPE InitializeEx(_In_ UINT sessionOptions, _In_ LPCTSTR sourceName, _In_ LPCTSTR bootDrive, _In_ LPCTSTR winDir, _In_ LPCTSTR externalDir) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateExternalTransformerExecutor(_Out_ struct ICSIExternalTransformerExecutor** ppCsiExecutor) = 0;
	virtual HRESULT STDMETHODCALLTYPE ObserveSessions(_In_ UINT options, _In_ struct ICbsSessionObserverListener* pListener, _Out_ struct IEnumCbsSession**) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetActivities(_In_ __int64 options, _Out_ struct IEnumCbsActivity**) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetEnhancedOptions(_In_ _CbsSessionEnhancedOption enhancedOptions) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetProperty(_In_ _CbsSessionConfigurableProperty prop, _In_ LPCTSTR value) = 0;
	virtual HRESULT STDMETHODCALLTYPE PerformOperation(UINT reserved, _In_ _CbsOperationType type) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetClientToken(_In_ __int64) = 0;

	virtual VOID STDMETHODCALLTYPE DumpInformation(void) = 0;
	virtual STDMETHODCALLTYPE ~ICbsSession() = 0;
	virtual VOID STDMETHODCALLTYPE InternalAddRef(void) = 0;
	virtual VOID STDMETHODCALLTYPE InternalRelease(void) = 0;
	// provides IServicingXXX (3 interfaces) in TurboStack.dll
	virtual HRESULT STDMETHODCALLTYPE ExtendedQueryInterface(_In_ _GUID const& riid, _Out_ LPVOID* ppvObject) = 0;

	// Vtbl2, Inherit from IServicingProcessor

	virtual HRESULT STDMETHODCALLTYPE QueryInterface2(_In_ REFIID riid, _Out_ LPVOID* ppvObject) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef2(void) = 0;
	virtual ULONG STDMETHODCALLTYPE Release2(void) = 0;

	virtual HRESULT STDMETHODCALLTYPE QuerySessionStatus(_In_ LPCTSTR szSessionID, _Out_ HRESULT* phrStatus) = 0;
	virtual HRESULT STDMETHODCALLTYPE Process(
		_In_ const UINT* ServicingProcessorOptions,
		_In_ LPCTSTR szActionListPath,
		_In_ LPCTSTR szSandboxPath,
		_In_ LPCTSTR szClientID,
		_In_ ICbsUIHandler* pUIHandler,
		_Out_ _CbsRequiredAction* requiredAction,
		_Out_ LPTSTR* pszSessionID) = 0;
	virtual HRESULT STDMETHODCALLTYPE WritePackageFileList(
		_In_ const UINT* ServicingProcessorOptions,
		_In_ LPCTSTR szActionListPath,
		_In_ LPCTSTR szSandboxPath,
		_In_ LPCTSTR szFileListPath,
		_In_ LPCTSTR szClientID,
		_In_ LPCTSTR szPackageFilePath) = 0;
};

struct ICbsSessionInternal;

struct ISxSStore : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

	virtual HRESULT STDMETHODCALLTYPE BeginAssemblyInstall(ULONG) = 0;
	virtual HRESULT STDMETHODCALLTYPE InstallAssembly(ULONG, _Out_ USHORT const*, _Out_ _SXS_STORE_INSTALL_SOURCE_INFO const*, _Out_ _SXS_STORE_INSTALL_REFERENCEW const*) = 0;
	virtual HRESULT STDMETHODCALLTYPE EndAssemblyInstall(ULONG, _Out_ ULONG*) = 0;
	virtual HRESULT STDMETHODCALLTYPE UninstallAssembly(ULONG, _Out_ USHORT const*, _Out_ _SXS_STORE_INSTALL_REFERENCEW const*, _Out_ ULONG*) = 0;
};

struct ICSITransaction : IUnknown {
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


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

