#pragma once

#include "isolation.h"

#pragma region Enumerations

enum class CbsState : LONG {
  ResolvedInvalid = -32,
  StagedInvalid = -64,
  InstalledInvalid = -112,
  PermanentInvalid = -128,

  Absent = 0,
  UninstallPending = 5,
  Resolving = 16,
  Resolved = 32,
  Staging = 48,
  Staged = 64,
  Superseded = 80,
  InstallPending = 96,
  PartiallyInstalled = 101,
  Installed = 112,
  Permanent = 128,
  Default = 4096,
  Detect = 4100,
  Canceled = 4101
};

enum class UpdateSelection : LONG {
  Unknown = -1,
  Off = 0,
  Default = 255,
  On = 1
};

enum class CbsPackageType : LONG {
  Unknown = -2,
  Existing = -1,
  Cabinet = 0,
  Expanded = 1,
  Manifest = 2,
  XmlString = 3,
  ExpandedWithMum = 4
};

enum class CbsRequiredAction : DWORD {
  None = 0,
  Reboot = 1
};

enum class CbsSessionProperty : DWORD {
  RebootRequired = 1,
  ErrorDetail = 2,
  Serviceable = 3,
  CompressionEnabled = 4,
  Report = 5,
  CorruptionFlag = 6,

  VolatileSize = 8,
  NonVolatileSize = 9,
  SharedWithWindowsSize = 10,
  AccordingToExplorer = 11,
  LastScavengeDatetime = 12,
  SupersededPackageCount = 13,

  SessionCompletionDatatime = 15,
  PackageMinSize = 16,
  // ?
  TotalPackageMinSize = 18,

  RepairNeeded = 22,
  ReOffer = 23
};

enum class CbsSessionOption : DWORD {
  None = 0,
  LoadPersisted = 0x80,
  DoScavenge = 0x400,
  CancelAllPendedTransactions = 0x800,
  EnableCompression = 0x2000,
  DisableCompression = 0x4000,
  DetectAndRepairStoreCorruption = 0xC000,
  ReportStackInfo = 0x100000,
  DoSynchronousCleanup = 0x400000,
  AnalyzeComponentStore = 0x8000000,
  CancelOnlySmartPendedTransactions = 0x80000000
};

enum class CbsSessionEnhancedOption : DWORD {
  ManuallyRepair = 0x800
};

enum class CbsSessionState : LONG {
  Unknown = 0,
  Ready = 16,
  Queued = 32,
  Started = 48,
  Panned = 64,
  Resolved = 80,
  Staged = 96,
  ExecutionDelayed = 101,
  Installed = 112,
  ShutdownStart = 144,
  ShutdownFinish = 160,
  Startup = 176,
  StartupFinish = 192,
  Complete = 208,
  Interrupted = 224,
  Corrupted = 240,
  MarkedForRetry = 256
};


enum class CbsOperationType : LONG {
  None = 0,
  ExportRepository = 1,
  UpdateImage = 2,
  PerformPrepareServicingOperation = 3,
  PerformLateAcquisitionOperation = 4,
  InitICSIStore = 8
};

enum class CbsSessionConfigurableProperty : LONG {

};

enum class CbsApplicability : LONG {
  InvalidValue = -1,
  NotApplicable = 0,
  NeedsParent = 2,
  Applicable = 4
};

enum class CbsSelectability : LONG {
  InvalidValue = -1,
  Class1 = 1,
  Class2 = 2
};


enum class CbsInstallState : LONG {
  PartiallyInstalled = -19,
  Cancel = -18,
  Superseded = -17,
  Default = -16,
  Invalid_Permanent = -8,
  Invalid_Installed = -7,
  Invalid_Staged = -4,
  Invalid_Resolved = -2,
  Unknown = -1,

  Absent = 0,

  Resolving = 1,
  Resolved = 2,
  Staging = 3,
  Staged = 4,
  UninstallRequested = 5,
  InstallRequested = 6,
  Installed = 7,
  Permanent = 8,

  Invalid = 0x7fffffff
};

enum class CbsPackageProperty : DWORD {
  IdentityString = 1,
  DisplayName = 2,
  Description = 3,
  Restart = 4,
  PsfName = 5,
  Hidden = 6,
  Keyword = 7,
  ReleaseType = 8,
  ProductName = 9,
  ProductVersion = 10,
  Permenence = 11,
  Company = 12,
  Copyright = 13,
  SupportInformation = 14,
  CreationTimeStamp = 15,
  LastUpdateTimeStamp = 16,
  InstallTimeStamp = 17,
  InstallPackageName = 18,
  InstallLocation = 19,
  InstallClient = 20,
  InstallUserName = 21,
  ExtendedError = 22,
  Pended = 23,
  Hotpatch = 24,
  Exclusive = 25,
  AllowedOffline = 26,
  CompletelyOfflineCapable = 27,
  ScavengeSequence = 28,
  PackageSize = 29,
  SupersededTime = 30,
  StackUpdate = 31,
  ReleaseQuality = 32,
  TargetPartition = 33,
  BinaryPartition = 34,
  CapabilityIdentity = 35,
  InternalStagingSizeWithExternalStorage = 36,
  InternalStagingSizeWithoutExternalStorage = 37,
  InstallSizeWithExternalStorage = 38,
  InstallSizeWithoutExternalStorage = 39,
  ExternalStagingSize = 40,
  FeatureFMID = 41,
  FeatureGroup = 42,
  PermanentUntilReset = 43,
  SatelliteType = 44,
  DeclareSatelliteLanguage = 45,
  DeclareSatelliteArch = 46,
  RemoveOnReset = 51
};

enum class CbsResourceType : DWORD {

};

enum class CbsCardinality : DWORD {

};

enum class CbsUpdateProperty : DWORD {
  Name = 1,
  DisplayName = 2,
  Description = 3,
  DisplayFile = 4,
  Restart = 5,
  PsfName = 6,
  DownloadSize = 7,
  SetMembership = 8
};

enum class tagCbsPackageDecryptionData {

};

enum class tagCbsPackageEncryptionEnum {

};

enum class CbsPackageChangeOption : DWORD {
  CbsTreatPackageAsPSFX = 1 << 15
};

enum class CbsOperationStage : DWORD {
  Waiting = 1,
  Planning = 5,
  Downloading = 15,
  Extracting = 20,
  Resolving = 25,
  Staging = 30,
  Installing = 40,
  InstallingEx = 50,
  ReservicingLCU = 60
};

enum class CbsCapabilitySourceFilter : DWORD {
  EnumAllowCloud = 0x4,
  EnumTreatLocalSourceAsUUPRepo = 0x22,
  EnumReserveUnavailableFOD = 0x26,
  LanguagePack = 0x40,
  ActionList = 0x10,
  WindowsUpdate = 0x100,
  RemovedMarkers = 0x1000,
};

#pragma endregion

struct SxSStoreInstallSourceInfo {

};

struct SxSStoreInstallReferenceW {

};

struct ICbsIdentity : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Clear(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE IsNull(_Out_ BOOL* pbIsNull) = 0;
  virtual HRESULT STDMETHODCALLTYPE IsEqual(_Out_ ICbsIdentity* pIdentComp, _Out_ BOOL* pbIsEqual) = 0;
  virtual HRESULT STDMETHODCALLTYPE LoadFromAttributes(PCWSTR szName, PCWSTR szPublicKeyToken, PCWSTR szProcessor, PCWSTR szLanguage, PCWSTR szVersion) = 0;
  virtual HRESULT STDMETHODCALLTYPE LoadFromStringId(PCWSTR szID) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetStringId(_Out_ PWSTR* pszID) = 0;
  virtual HRESULT STDMETHODCALLTYPE SaveAsStringId(_Out_ PWSTR* pszID) = 0;
  virtual HRESULT STDMETHODCALLTYPE InternalIsEqualbyAttribute(PCWSTR szName, PCWSTR szPublicKeyToken, PCWSTR szProcessor, PCWSTR szLanguage, PCWSTR szVersion, _Out_ BOOL* pbIsEqual) = 0;
  virtual BOOL STDMETHODCALLTYPE IsMoniker(void) = 0;
};

struct ICbsCapability : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE GetIdentity(ICbsIdentity**) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetProperty(CbsPackageProperty, _Out_ PWSTR*) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateUpdates(CbsApplicability pApplicability, CbsSelectability pSelectability, _Out_ struct IEnumCbsUpdate** ppUpdateList) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetUpdate(_In_ PCWSTR szUpdName, _Out_ struct ICbsUpdate** ppUpdate) = 0;
  virtual HRESULT STDMETHODCALLTYPE AddSource(_In_ PCWSTR basePath) = 0;
  virtual HRESULT STDMETHODCALLTYPE RemoveSource(_In_ PCWSTR basePath) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateSources(_Out_ IEnumString** pSourceList) = 0;
  virtual HRESULT STDMETHODCALLTYPE EvaluateApplicability(UINT option, _Out_ CbsInstallState* pApplicabilityState, _Out_ CbsInstallState* pCurrentState) = 0;
  virtual HRESULT STDMETHODCALLTYPE InitiateChanges(UINT installOptions, CbsInstallState targetState, _In_ struct ICbsUIHandler* pUIHandler) = 0;
  virtual HRESULT STDMETHODCALLTYPE Status(_Out_ CbsInstallState* pProgressState, _Out_ DWORD* pLastError) = 0;
  virtual HRESULT STDMETHODCALLTYPE ResourcesToCheck(CbsResourceType resType, _Out_ IEnumString** pResList) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetCapability(_Out_ PWSTR* pszNamespace, _Out_ PWSTR* pszLang, _Out_ PWSTR* pszArch, ULONG* dwVerMajor, ULONG* dwVerMinor) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDependencies(struct IEnumCbsCapability** pDependenciesList) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetSources(UINT*) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDownloadSize(ULONG* nBytes) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetInstallSize(ULONG* nBytes) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetInstallState(CbsInstallState* pState) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetOwnerInformation(UINT reserved, int*, UINT*, _Out_ PWSTR*) = 0;
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
  virtual HRESULT STDMETHODCALLTYPE GetProperty(CbsPackageProperty prop, _Out_ PWSTR* pValue) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateUpdates(CbsApplicability pApplicability, CbsSelectability pSelectability, _Out_ struct IEnumCbsUpdate** ppUpdateList) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetUpdate(PCWSTR szUpdName, _Out_ struct ICbsUpdate** ppUpdate) = 0;
  virtual HRESULT STDMETHODCALLTYPE AddSource(PCWSTR basePath) = 0;
  virtual HRESULT STDMETHODCALLTYPE RemoveSource(PCWSTR basePath) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnumerateSources(_Out_ IEnumString** pSourceList) = 0;
  virtual HRESULT STDMETHODCALLTYPE EvaluateApplicability(UINT option, _Out_ CbsInstallState* pApplicabilityState, _Out_ CbsInstallState* pCurrentState) = 0;
  // installOptions: \in 0xFF7F
  virtual HRESULT STDMETHODCALLTYPE InitiateChanges(UINT installOptions, CbsInstallState targetState, _In_ struct ICbsUIHandler* pUIHandler) = 0;
  virtual HRESULT STDMETHODCALLTYPE Status(_Out_ CbsInstallState* pProgressState, _Out_ DWORD* pLastError) = 0;
  virtual HRESULT STDMETHODCALLTYPE ResourcesToCheck(CbsResourceType resType, _Out_ IEnumString** pResList) = 0;
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

  virtual HRESULT STDMETHODCALLTYPE GetProperty(CbsUpdateProperty prop, _Out_ PWSTR* pValue) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetPackage(_Out_ ICbsPackage** ppPackage) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetParentUpdate(UINT index, _Out_ PWSTR* ppParent, _Out_ PWSTR* ppSet) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetCapability(_Out_ CbsApplicability* pApplicability, _Out_ CbsSelectability* pSelectability) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDeclaredSet(UINT unk, _Out_ PWSTR* pDeclaredSet, _Out_ CbsCardinality* Cardinality) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetInstallState(_Out_ CbsInstallState* pCurrentState, _Out_ CbsInstallState* pIntendedState, _Out_ CbsInstallState* pRequestState) = 0;
  virtual HRESULT STDMETHODCALLTYPE SetInstallState(UINT options, CbsInstallState state) = 0;
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
  virtual HRESULT STDMETHODCALLTYPE Error(HRESULT hr, PCWSTR szUnk, int*) = 0;
  virtual HRESULT STDMETHODCALLTYPE ResolveSource(PCWSTR, ICbsIdentity*, PCWSTR, PWSTR*, int*) = 0;
  virtual HRESULT STDMETHODCALLTYPE Progress(CbsInstallState stIns, UINT curProg, UINT totProg, int*) = 0;
  virtual HRESULT STDMETHODCALLTYPE EnteringStage(UINT, CbsOperationStage, int, int) = 0;
  virtual HRESULT STDMETHODCALLTYPE ProgressEx(CbsInstallState stIns, UINT curProg, UINT totProg, UINT, int*) = 0;
};

// I don't know how to use it
struct ICSIExternalTransformerExecutor : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Commit(PCWSTR, PCWSTR, PCWSTR, PCWSTR) = 0;
  virtual HRESULT STDMETHODCALLTYPE Initialize(ULONG, UINT64, PCWSTR, PCWSTR, PCWSTR) = 0;
  virtual HRESULT STDMETHODCALLTYPE Install(PCWSTR, PCWSTR) = 0;
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

struct ICbsFeaturePackage : ICbsPackage {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;


};

struct IEnumCbsCapability : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, _Out_ struct ICbsCapability** rgpCol, _Out_ UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsCapability**) = 0;
};

struct IEnumIdentity : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, _Out_ struct ICbsIdentity** rgpCol, _Out_ UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumIdentity**) = 0;
};

struct IEnumCbsIdentity : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, _Out_ struct ICbsIdentity** rgpCol, _Out_ UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsIdentity**) = 0;
};

struct IEnumCbsSession : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, _Out_ struct ICbsSession** rgpCol, _Out_ UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsIdentity**) = 0;
};

struct IEnumCbsUpdate : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, _Out_ struct ICbsUpdate** rgpCol, _Out_ UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsUpdate**) = 0;
};

struct IEnumCbsActivity : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, _Out_ struct ICbsActivity** rgpCol, _Out_ UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(_Out_ IEnumCbsActivity**) = 0;
};

struct IEnumCbsFeaturePackage : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Next(UINT celt, struct ICbsFeaturePackage** rgpCol, UINT* pbFetched) = 0;
  virtual HRESULT STDMETHODCALLTYPE Skip(UINT celt) = 0;
  virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(IEnumCbsFeaturePackage**) = 0;
};

struct ICbsSession : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE Initialize(_In_ CbsSessionOption sessionOptions, _In_ PCWSTR clientID, _In_opt_ PCWSTR bootDrive, _In_opt_ PCWSTR winDir) = 0;
  virtual HRESULT STDMETHODCALLTYPE Finalize(_In_ CbsRequiredAction* pRequiredAction) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreatePackage(_In_ UINT options, _In_ CbsPackageType packageType, _In_ PCWSTR szPkgPath, _In_ PCWSTR szSandboxPath, _Out_ ICbsPackage** pPackage) = 0;
  // options: 0 and 2 bit (0 | 1 | 4), unkArg will only appear in error log information
  virtual HRESULT STDMETHODCALLTYPE OpenPackage(_In_ UINT options, _In_ ICbsIdentity* pPackageIdentity, _In_ PCWSTR unkArgAboutLog, _Out_ ICbsPackage** pPackage) = 0;
  // options: normal: 0x1b0, all: 0x50 or 0x70
  virtual HRESULT STDMETHODCALLTYPE EnumeratePackages(_In_ UINT options, _Out_ IEnumCbsIdentity** pPackageList) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreateCbsIdentity(_Out_ ICbsIdentity** ppIdentity) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetStatus(_Out_ UINT* pCurrentPhase, _Out_ CbsSessionState* pLastSuccessfulSessionState, _Out_ BOOL* pbCompleted, _Out_ HRESULT* phrStatus) = 0;
  virtual HRESULT STDMETHODCALLTYPE Resume(_In_ ICbsUIHandler* pUIHandler) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetSessionId(_In_ PWSTR* pszId) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetProperty(_In_ CbsSessionProperty prop, _Out_ PWSTR* pValue) = 0;
  virtual HRESULT STDMETHODCALLTYPE AddPhaseBreak(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE FinalizeEx(_In_ UINT, _Out_ CbsRequiredAction* pRequiredAction) = 0;
  // Session7 is here
  virtual HRESULT STDMETHODCALLTYPE AddSource(_In_ UINT options, _In_ PCWSTR basePath) = 0;
  virtual HRESULT STDMETHODCALLTYPE RegisterCbsUIHandler(_In_ ICbsUIHandler* pUIHandler) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreateWindowsUpdatePackage(_In_ UINT, _In_ PCWSTR, _In_ GUID, _In_ UINT, _In_ CbsPackageType, _In_ PCWSTR, _In_ PCWSTR, _In_ UINT, _Out_ tagCbsPackageDecryptionData* const, _In_ tagCbsPackageEncryptionEnum, _Out_ ICbsPackage**) = 0;
  // sourceFilter: \in 0xFF, 0x40 means LanguagePacks
  virtual HRESULT STDMETHODCALLTYPE EnumerateCapabilities(_In_ UINT sourceFilter, _In_z_ PCWSTR szNamespace, _In_z_ PCWSTR szLang, _In_ PCWSTR szArch, _In_ ULONG dwMajor, _In_ ULONG dwMinor, _Out_ IEnumCbsCapability**) = 0;
  virtual HRESULT STDMETHODCALLTYPE InitializeEx(_In_ UINT sessionOptions, _In_ PCWSTR sourceName, _In_ PCWSTR bootDrive, _In_ PCWSTR winDir, _In_ PCWSTR externalDir) = 0;
  virtual HRESULT STDMETHODCALLTYPE CreateExternalTransformerExecutor(_Out_ struct ICSIExternalTransformerExecutor** ppCsiExecutor) = 0;
  virtual HRESULT STDMETHODCALLTYPE ObserveSessions(_In_ UINT options, _In_ struct ICbsSessionObserverListener* pListener, _Out_ struct IEnumCbsSession**) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetActivities(_In_ __int64 options, _Out_ struct IEnumCbsActivity**) = 0;
  virtual HRESULT STDMETHODCALLTYPE SetEnhancedOptions(_In_ CbsSessionEnhancedOption enhancedOptions) = 0;
  virtual HRESULT STDMETHODCALLTYPE SetProperty(_In_ CbsSessionConfigurableProperty prop, _In_ PCWSTR value) = 0;
  virtual HRESULT STDMETHODCALLTYPE PerformOperation(UINT reserved, _In_ CbsOperationType type) = 0;
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

  virtual HRESULT STDMETHODCALLTYPE QuerySessionStatus(_In_ PCWSTR szSessionID, _Out_ HRESULT* phrStatus) = 0;
  virtual HRESULT STDMETHODCALLTYPE Process(
    _In_ const UINT* ServicingProcessorOptions,
    _In_ PCWSTR szActionListPath,
    _In_ PCWSTR szSandboxPath,
    _In_ PCWSTR szClientID,
    _In_ ICbsUIHandler* pUIHandler,
    _Out_ CbsRequiredAction* requiredAction,
    _Out_ PWSTR* pszSessionID) = 0;
  virtual HRESULT STDMETHODCALLTYPE WritePackageFileList(
    _In_ const UINT* ServicingProcessorOptions,
    _In_ PCWSTR szActionListPath,
    _In_ PCWSTR szSandboxPath,
    _In_ PCWSTR szFileListPath,
    _In_ PCWSTR szClientID,
    _In_ PCWSTR szPackageFilePath) = 0;
};

struct ICbsSessionInternal;

struct ISxSStore : IUnknown {
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _Out_ LPVOID* ppvObject) override = 0;
  virtual ULONG STDMETHODCALLTYPE AddRef(void) override = 0;
  virtual ULONG STDMETHODCALLTYPE Release(void) override = 0;

  virtual HRESULT STDMETHODCALLTYPE BeginAssemblyInstall(ULONG) = 0;
  virtual HRESULT STDMETHODCALLTYPE InstallAssembly(ULONG, _Out_ USHORT const*, _Out_ SxSStoreInstallSourceInfo const*, _Out_ SxSStoreInstallReferenceW const*) = 0;
  virtual HRESULT STDMETHODCALLTYPE EndAssemblyInstall(ULONG, _Out_ ULONG*) = 0;
  virtual HRESULT STDMETHODCALLTYPE UninstallAssembly(ULONG, _Out_ USHORT const*, _Out_ SxSStoreInstallReferenceW const*, _Out_ ULONG*) = 0;
};
