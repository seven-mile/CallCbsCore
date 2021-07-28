#pragma once

#include <dpx.h>

#pragma region logging
enum WdsLogSource {
  WdsLogSourceDPX = 0x10000,
  WdsLogSourceCBS = 0x20000,
  WdsLogSourceCSI = 0x18000,
  WdsLogSourceSXS = 0x28000,
  WdsLogSourceCMI = 0x30000,
  WdsLogSourceDEPLOY = 0x40000,
  WdsLogSourceDU = 0x50000,
  WdsLogSourceIBS = 0x60000,
  WdsLogSourceIBSLIB = 0x64000,
  WdsLogSourceDIAG = 0x70000,
  WdsLogSourceDIAGER = 0x74000,
  WdsLogSourceMIG = 0x80000,
  WdsLogSourceHWARE = 0x84000,
  WdsLogSourceMIGUI = 0x88000,
  WdsLogSourceUI = 0xA0000,
  WdsLogSourceCONX = 0xA4000,
  WdsLogSourceMOUPG = 0xA8000,
  WdsLogSourceWDS = 0xB0000,
  WdsLogSourceDISM = 0xB8000,
  WdsLogSourcePANTHR = 0x90000,
  WdsLogSourceWINPE = 0xC0000,
  WdsLogSourceSP = 0xC8000,
  WdsLogSourceLIB = 0xD0000,
  WdsLogSourceTOOL = 0xE0000
};

enum WdsLogLevel {
  WdsLogLevelAssert = 0x0000000,
  WdsLogLevelFatalError = 0x1000000,
  WdsLogLevelError = 0x2000000,
  WdsLogLevelWarning = 0x3000000,
  WdsLogLevelInfo = 0x4000000
  // Unused
  // WdsLogLevelStatus		= 0x5000000,
  // WdsLogLevelVerbose	= 0x68000000,
  // WdsLogLevelTrace		= 0x70000000
};

#define GetEnumName(x) (std::string(enum_name(x)))
HRESULT InternalLogA(WdsLogSource source, WdsLogLevel level, LPCSTR fmt, ...);
#pragma endregion

#pragma region text displaying
enum LineSize : UINT
{
  LineSizeLong = 50,
  LineSizeMedium = 40,
  LineSizeShort = 35
};

void InsertLine(UINT uSize);
#pragma endregion

#pragma region data conversion
SYSTEMTIME WindowsTimeStamp2SystemTime(const UINT64& timeStamp);
std::wstring ExpandEnvW(const std::wstring& str);
std::wstring TextizeHresult(HRESULT hr);
std::string TextizeHresultA(HRESULT hr);
#pragma endregion

#pragma region CBS objects helper
template <class T, class IEnumT>
inline std::vector<ComPtr<T>> GetIEnumComPtrVector(ComPtr<IEnumT> pEnum)
{
  std::vector<ComPtr<T>> v;

  UINT k;

  do {
    ComPtr<T> ptr;
    pEnum->Next(1, static_cast<T**>(&ptr), &k);
    if (ptr) v.push_back(ptr);
  } while (k);

  return v;
}

template <class T, class IEnumT>
inline std::vector<T> GetIEnumStructVector(ComPtr<IEnumT> pEnum)
{
  std::vector<T> v;
  UINT *pk = new UINT;

  while (true) {
    T ptr;
    HRESULT hr = pEnum->Next(1, &ptr, pk);
    if (*pk) v.push_back(ptr);
    else break;
  }

  return v;
}
ComPtr<ICbsPackage> GetFoundationPackage();
HRESULT PrintPackageInfo(const ComPtr<ICbsPackage>& pPkg, const bool bIns = true);
HRESULT PrintUpdateInfo(const ComPtr<ICbsUpdate>& pUpd);
#pragma endregion

#pragma region cabinet
// ==============================
// Cabinet Decompression
// ==============================
template <class T> requires(requires(T x) { x(0ull, 0ull); })
inline HRESULT DecompressCabinetToDirectory(std::wstring file, std::wstring dir, T progCallback) {
  BEGIN_ERROR_HANDLING();

  ComPtr<IDpxJob> pJob;
  CHECK(DpxNewJob(dir.c_str(), &pJob), "Failed to create new DpxJob.[dir = %S]", dir.c_str());
  ComPtr<IDpxContainer> pContainer;
  CHECK(pJob->AddContainer(file.c_str(), &pContainer), "Failed to add a DpxContainer.[file = %S]", file.c_str());

  CHECK(pContainer->ExtractAllFiles(), "Failed to order all files to be extracted.");
  
  /*ComPtr<IEnumDpxFiles> pFiles;
  pContainer->EnumFilesToExtract(&pFiles);
  for (auto &pFile : GetIEnumComPtrVector<IDpxFile>(pFiles)) {
    PWSTR szSource, szTarget;
    pFile->GetSourceFileName(&szSource);
    pFile->GetTargetFileName(&szTarget);
    std::wcout << std::format(L"Source: {}, Target: {}\n", szSource, szTarget);
  }*/

  CHECK(pJob->Resume(DPX_RESUME_TYPE::DPX_RESUME_AND_DOWNLOAD),
    "Failed to resume DpxJob and download.");

  for (DPX_PROGRESS prog; true; Sleep(250)) {
    CHECK(pJob->GetProgress(&prog), "Failed to get JobProgess information.");
    progCallback(prog.Files.Completed, prog.Files.Completed + prog.Files.Remaining);
    if (!prog.Files.Remaining) break;
  }
  return S_OK;
}
#pragma endregion

#pragma region permission
HRESULT SudoTI(const std::wstring& exec);
HRESULT CheckSudoSelf();
#pragma endregion

