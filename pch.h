#pragma once

#include <iostream>
#include <algorithm>
#include <memory>
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
#define CHECK(x, y, ...) if(FAILED(hr = (x))) return WdsLogA(hr,WdsLogSourceUI,WdsLogLevelError,y,##__VA_ARGS__),hr
#define RET_HR_LOG(_hr, _fmt, ...) return WdsLogA(_hr,WdsLogSourceUI,WdsLogLevelError,_fmt,##__VA_ARGS__),(_hr)
#define RET_WIN32ERR_LOG(_dwErr, _fmt, ...) RET_HR_LOG(HRESULT_FROM_WIN32(_dwErr),_fmt,##__VA_ARGS__)
#define RET_LASTERR_LOG(_fmt, ...) RET_WIN32ERR_LOG(GetLastError(),_fmt,##__VA_ARGS__)

// Disable Warnings

#pragma warning(disable: 6031 6385 6386 6054)
