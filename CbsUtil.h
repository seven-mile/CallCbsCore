#pragma once

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
	//Unused
	//WdsLogLevelStatus		= 0x5000000,
	//WdsLogLevelVerbose	= 0x68000000,
	//WdsLogLevelTrace		= 0x70000000
};

enum LineSize : UINT
{
	LineSizeLong = 50,
	LineSizeMedium = 40,
	LineSizeShort = 35
};

HRESULT WdsLogA(HRESULT hr, WdsLogSource source, WdsLogLevel level, const char* fmt, ...);
HRESULT WdsLogHrInternalA(HRESULT hr, WdsLogSource source, WdsLogLevel level, const char* fmt, ...);
SYSTEMTIME WindowsTimeStamp2SystemTime(const UINT64 &timeStamp);
std::wstring ExpandEnvW(const std::wstring &str);
HRESULT TextizeCbsInstallState(const _CbsInstallState &st, LPTSTR* ret);
LPCTSTR TextizeHresult(HRESULT hr);
LPCSTR TextizeHresultA(HRESULT hr);
void InsertLine(UINT uSize);

template <class T, class IEnumT> std::vector<CComPtr<T>> GetIEnumVector(CComPtr<IEnumT> pEnum)
{
	std::vector<CComPtr<T>> v;

	ULONG k;

	do {
		CComPtr<T> ptr;
		pEnum->Next(1, (T**)&ptr, &k);
		if (ptr) v.push_back(ptr);
	} while (k);

	return v;
}