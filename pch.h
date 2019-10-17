#pragma once

#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <cassert>
#include <Windows.h>
#include <tchar.h>
#include <comdef.h>
#include <Shlwapi.h>
#include <atlbase.h>
#include <atlstr.h>

#include "CbsApi.h"

#include "CbsRef.h"
#include "CbsUtil.h"

#pragma comment(lib, "Shlwapi.lib")

// My definations

#define BEGIN_ERROR_HANDLING() HRESULT hr = (S_OK)
#define WdsLogWithHrTextA(_hr, ...) WdsLogHrInternalA(_hr,##__VA_ARGS__,_hr,TextizeHresultA(_hr))
#define CHECK(x, y, ...) if(FAILED(hr = (x))) return WdsLogWithHrTextA(hr,WdsLogSourceUI,WdsLogLevelError,y,##__VA_ARGS__),hr
#define RET_HR_LOG(_hr, _fmt, ...) return WdsLogWithHrTextA(_hr,WdsLogSourceUI,WdsLogLevelError,_fmt,##__VA_ARGS__),(_hr)
#define RET_WIN32ERR_LOG(_dwErr, _fmt, ...) RET_HR_LOG(HRESULT_FROM_WIN32(_dwErr),_fmt,##__VA_ARGS__)
#define RET_LASTERR_LOG(_fmt, ...) RET_WIN32ERR_LOG(GetLastError(),_fmt,##__VA_ARGS__)

constexpr auto CBS_LOG_FILE = _T("C:\\Users\\HigHwind\\Desktop\\NBS.log");

template <class T>
constexpr const T* CONCAT_STR(const T* str1, const T* str2)
{
	size_t sz1 = AtlStrLen(str1), sz2 = AtlStrLen(str2);
	T* str = new T[sz1 + sz2 + 1];

	for (size_t pos = 0; pos < sz1; pos++)
		str[pos] = str1[pos];
	for (size_t pos = 0; pos < sz2; pos++)
		str[sz1 + pos] = str2[pos];
	str[sz1 + sz2] = '\0';

	return str;
}

// Disable Warnings

#pragma warning(disable: 6031 6385 6386 6054)
