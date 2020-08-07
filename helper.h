#pragma once

#include <string>
#include <sstream>

// Typedefs

template<class Interface>
using ComPtr = _com_ptr_t<_com_IIID<Interface, nullptr>>;

// Exception handling helper functions

#define BEGIN_ERROR_HANDLING() HRESULT hr = (S_OK)
#define LogA(_hr,sc,lv,...) InternalLogA(sc,lv,##__VA_ARGS__,_hr,TextizeHresultA(_hr).c_str())
#define CHECK(x,y,...) {if(FAILED(hr = (x))) return LogA(hr,WdsLogSourceUI,WdsLogLevelFatalError,y,##__VA_ARGS__),hr;}
#define RET_LOG(_ret,_hr,_fmt,...) return LogA(_hr,WdsLogSourceUI,WdsLogLevelError,_fmt,##__VA_ARGS__),(_ret)
#define RET_HR_LOG(_hr,_fmt,...) RET_LOG(_hr,_hr,_fmt,##__VA_ARGS__)
#define RET_WIN32ERR_LOG(_dwErr,_fmt,...) RET_HR_LOG(HRESULT_FROM_WIN32(_dwErr),_fmt,##__VA_ARGS__)
#define RET_LASTERR_LOG(_fmt,...) RET_WIN32ERR_LOG(GetLastError(),_fmt,##__VA_ARGS__)

// Constants

constexpr auto SID_TI = _T("S-1-5-80-956008885-3418522649-1831038044-1853292631-2271478464");
constexpr auto CBS_LOG_FILE = _T("C:\\Users\\HigHwind\\Desktop\\tmp\\cbstemp\\NBS.log");

// Constexpr calculation helper functions

template <class T>
constexpr size_t STR_LEN(const T* str) {

  size_t ret(0);
  for (const T* p = str; *p; p++, ret++);
  return ret;
}

template <class T>
constexpr const T* CONCAT_STR(const T* str1, const T* str2)
{
  size_t sz1 = STR_LEN(str1), sz2 = STR_LEN(str2);
  T* str = new T[sz1 + sz2 + 1];

  for (size_t pos = 0; pos < sz1; pos++)
    str[pos] = str1[pos];
  for (size_t pos = 0; pos < sz2; pos++)
    str[sz1 + pos] = str2[pos];
  str[sz1 + sz2] = '\0';

  return str;
}

template <class TNum>
inline std::string NUM_TO_STR(TNum n) {
  static std::stringstream ss;
  ss.clear();
  ss << n;
  std::string ret;
  ss >> ret;
  return ret;
}

template <class TNum>
inline std::wstring NUM_TO_WSTR(TNum n) {
  static std::wstringstream ss;
  ss.clear();
  ss << n;
  std::wstring ret;
  ss >> ret;
  return ret;
}

inline int64_t STR_TO_NUM(const std::string& s) {
  std::stringstream ss;
  ss << s;
  int64_t n;
  ss >> n;
  return n;
}

// decimal
inline int64_t STR_TO_NUM(const std::wstring& s) {
  std::wstringstream ss;
  ss << s;
  int64_t n;
  ss >> n;
  return n;
}

inline int64_t STR_TO_NUM_HEX(const std::wstring& s) {
  std::wstringstream ss;
  ss << std::hex;
  ss << s;
  int64_t n;
  ss >> n;
  return n;
}

// Disable Warnings

#pragma warning(disable: 6011 6031 6385 6386 6387 6054 26812)
