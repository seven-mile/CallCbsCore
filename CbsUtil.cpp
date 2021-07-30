#include "pch.h"

#include <UserEnv.h>
#pragma comment(lib, "Userenv.lib")
#include <WtsApi32.h>
#pragma comment(lib, "WtsApi32.lib")

#pragma comment(lib, "dpx.lib")

// Please don't use this function, use macro LogA instead.
HRESULT InternalLogA(WdsLogSource source, WdsLogLevel level, LPCSTR fmt, ...)
{
  HRESULT hr = HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION);
  va_list va;

  fmt = CONCAT_STR<CHAR>(fmt, " HRESULT - 0x%x - %s");

  if (g_conf.output_log)
  {
    CHAR szBuffer[255]{};
    va_start(va, fmt);

    _vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, fmt, va);
    printf("Log: ");
    vprintf(fmt, va);

    va_end(va);

    OutputDebugStringA(szBuffer);
    OutputDebugStringA("\n");
    printf("\n");
  }

  if (g_conf.output_log && vpfnWdsSetupLogMessageA) {
    va_start(va, fmt);

    hr |= vpfnWdsSetupLogMessageA(
      vpfnConstructPartialMsgVA(level, fmt, va),
      source, "D", 0, 1151, __FILE__, __FUNCTION__,
      vpfnCurrentIP(), 0, nullptr, 0);
  }

  return hr;
}



SYSTEMTIME WindowsTimeStamp2SystemTime(const UINT64& timeStamp)
{
  LARGE_INTEGER liTmp;
  liTmp.QuadPart = timeStamp;
  FILETIME fileTimeTmp, fileLocalTimeTmp;
  fileTimeTmp.dwLowDateTime = liTmp.LowPart;
  fileTimeTmp.dwHighDateTime = liTmp.HighPart;

  // Optional
  FileTimeToLocalFileTime(&fileTimeTmp, &fileLocalTimeTmp);

  SYSTEMTIME sysTimeTmp;
  FileTimeToSystemTime(&fileLocalTimeTmp, &sysTimeTmp);

  return sysTimeTmp;
}

std::wstring ExpandEnvW(const std::wstring& str)
{
  std::wcout << "Env Str:" << str << std::endl;
  std::wstring strBuf;
  strBuf.resize(1024);
  if (!ExpandEnvironmentStringsW(str.c_str(), &strBuf[0], 1024))
  {
    LogA(HRESULT_FROM_WIN32(GetLastError()), WdsLogSourceTOOL,
      WdsLogLevelFatalError, "ExpandEnvironmentStringsW failed!!!");
    return L"(null)";
  }
  strBuf.resize(wcslen(strBuf.c_str()));
  return strBuf;
}

LPCTSTR TextizeSetupHresult(HRESULT hr)
{
  LPCTSTR result;
  int tmp1, tmp2;

  if (hr > -2146498546)
  {
    if (hr > -2146498481)
    {
      if (hr > -2146496512)
      {
        tmp1 = hr - 985089;
        if (tmp1)
        {
          tmp2 = tmp1 - 1;
          if (tmp2)
          {
            if (tmp2 != 1)
              return nullptr;
            result = _T("CBS_S_STACK_SHUTDOWN_REQUIRED");
          }
          else
          {
            result = _T("CBS_S_ALREADY_EXISTS");
          }
        }
        else
        {
          result = _T("CBS_S_BUSY");
        }
      }
      else if (hr == -2146496512)
      {
        result = _T("SPAPI_E_ERROR_NOT_INSTALLED");
      }
      else
      {
        switch (hr)
        {
        case (int)0x800F0900:
          result = _T("CBS_E_XML_PARSER_FAILURE");
          break;
        case (int)0x800F0901:
          result = _T("CBS_E_MANIFEST_VALIDATION_MULTIPLE_UPDATE_COMPONENT_ON_SAME_FAMILY_NOT_ALLOWED");
          break;
        case (int)0x800F0902:
          result = _T("CBS_E_BUSY");
          break;
        case (int)0x800F0903:
          result = _T("CBS_E_INVALID_RECALL");
          break;
        case (int)0x800F0904:
          result = _T("CBS_E_MORE_THAN_ONE_ACTIVE_EDITION");
          break;
        case (int)0x800F0905:
          result = _T("CBS_E_NO_ACTIVE_EDITION");
          break;
        case (int)0x800F0906:
          result = _T("CBS_E_DOWNLOAD_FAILURE");
          break;
        case (int)0x800F0907:
          result = _T("CBS_E_GROUPPOLICY_DISALLOWED");
          break;
        case (int)0x800F0908:
          result = _T("CBS_E_METERED_NETWORK");
          break;
        case (int)0x800F0909:
          result = _T("CBS_E_PUBLIC_OBJECT_LEAK");
          break;
        case (int)0x800F090B:
          result = _T("CBS_E_REPAIR_PACKAGE_CORRUPT");
          break;
        case (int)0x800F090C:
          result = _T("CBS_E_COMPONENT_NOT_INSTALLED_BY_CBS");
          break;
        case (int)0x800F090D:
          result = _T("CBS_E_MISSING_PACKAGE_MAPPING_INDEX");
          break;
        case (int)0x800F090E:
          result = _T("CBS_E_EMPTY_PACKAGE_MAPPING_INDEX");
          break;
        case (int)0x800F090F:
          result = _T("CBS_E_WINDOWS_UPDATE_SEARCH_FAILURE");
          break;
        case (int)0x800F0910:
          result = _T("CBS_E_WINDOWS_AUTOMATIC_UPDATE_SETTING_DISALLOWED");
          break;
        case (int)0x800F0920:
          result = _T("CBS_E_HANG_DETECTED");
          break;
        case (int)0x800F0921:
          result = _T("CBS_E_PRIMITIVES_FAILED");
          break;
        case (int)0x800F0922:
          result = _T("CBS_E_INSTALLERS_FAILED");
          break;
        case (int)0x800F0923:
          result = _T("CBS_E_SAFEMODE_ENTERED");
          break;
        case (int)0x800F0924:
          result = _T("CBS_E_SESSIONS_LEAKED");
          break;
        case (int)0x800F0925:
          result = _T("CBS_E_INVALID_EXECUTESTATE");
          break;
        case (int)0x800F0940:
          result = _T("CBS_E_WUSUS_MAPPING_UNAVAILABLE");
          break;
        case (int)0x800F0941:
          result = _T("CBS_E_WU_MAPPING_UNAVAILABLE");
          break;
        case (int)0x800F0942:
          result = _T("CBS_E_WUSUS_BYPASS_MAPPING_UNAVAILABLE");
          break;
        case (int)0x800F0943:
          result = _T("CBS_E_WUSUS_MISSING_PACKAGE_MAPPING_INDEX");
          break;
        case (int)0x800F0944:
          result = _T("CBS_E_WU_MISSING_PACKAGE_MAPPING_INDEX");
          break;
        case (int)0x800F0945:
          result = _T("CBS_E_WUSUS_BYPASS_MISSING_PACKAGE_MAPPING_INDEX");
          break;
        case (int)0x800F0946:
          result = _T("CBS_E_SOURCE_MISSING_FROM_WUSUS_CAB");
          break;
        case (int)0x800F0947:
          result = _T("CBS_E_SOURCE_MISSING_FROM_WUSUS_EXPRESS");
          break;
        case (int)0x800F0948:
          result = _T("CBS_E_SOURCE_MISSING_FROM_WU_CAB");
          break;
        case (int)0x800F0949:
          result = _T("CBS_E_SOURCE_MISSING_FROM_WU_EXPRESS");
          break;
        case (int)0x800F094A:
          result = _T("CBS_E_SOURCE_MISSING_FROM_WUSUS_BYPASS_CAB");
          break;
        case (int)0x800F094B:
          result = _T("CBS_E_SOURCE_MISSING_FROM_WUSUS_BYPASS_EXPRESS");
          break;
        case (int)0x800F094C:
          result = _T("CBS_E_3RD_PARTY_MAPPING_UNAVAILABLE");
          break;
        case (int)0x800F094D:
          result = _T("CBS_E_3RD_PARTY_MISSING_PACKAGE_MAPPING_INDEX");
          break;
        case (int)0x800F094E:
          result = _T("CBS_E_SOURCE_MISSING_FROM_3RD_PARTY_CAB");
          break;
        case (int)0x800F094F:
          result = _T("CBS_E_SOURCE_MISSING_FROM_3RD_PARTY_EXPRESS");
          break;
        case (int)0x800F0950:
          result = _T("CBS_E_INVALID_WINDOWS_UPDATE_COUNT");
          break;
        case (int)0x800F0951:
          result = _T("CBS_E_INVALID_NO_PRODUCT_REGISTERED");
          break;
        case (int)0x800F0952:
          result = _T("CBS_E_INVALID_ACTION_LIST_PACKAGE_COUNT");
          break;
        case (int)0x800F0953:
          result = _T("CBS_E_INVALID_ACTION_LIST_INSTALL_REASON");
          break;
        case (int)0x800F0954:
          result = _T("CBS_E_INVALID_WINDOWS_UPDATE_COUNT_WSUS");
          break;
        case (int)0x800F0955:
          result = _T("CBS_E_INVALID_PACKAGE_REQUEST_ON_MULTILINGUAL_FOD");
          break;
        case (int)0x800F0980:
          result = _T("PSFX_E_DELTA_NOT_SUPPORTED_FOR_COMPONENT");
          break;
        case (int)0x800F0981:
          result = _T("PSFX_E_REVERSE_AND_FORWARD_DELTAS_MISSING");
          break;
        case (int)0x800F0982:
          result = _T("PSFX_E_MATCHING_COMPONENT_NOT_FOUND");
          break;
        case (int)0x800F0983:
          result = _T("PSFX_E_MATCHING_COMPONENT_DIRECTORY_MISSING");
          break;
        case (int)0x800F0984:
          result = _T("PSFX_E_MATCHING_BINARY_MISSING");
          break;
        case (int)0x800F0985:
          result = _T("PSFX_E_APPLY_REVERSE_DELTA_FAILED");
          break;
        case (int)0x800F0986:
          result = _T("PSFX_E_APPLY_FORWARD_DELTA_FAILED");
          break;
        case (int)0x800F0987:
          result = _T("PSFX_E_NULL_DELTA_HYDRATION_FAILED");
          break;
        case (int)0x800F0988:
          result = _T("PSFX_E_INVALID_DELTA_COMBINATION");
          break;
        case (int)0x800F0989:
          result = _T("PSFX_E_REVERSE_DELTA_MISSING");
          break;
        default:
          return nullptr;
        }
      }
    }
    else if (hr == -2146498481)
    {
      result = _T("CBS_E_RESOLVE_FAILED");
    }
    else
    {
      switch (hr + 2146498545)
      {
      case 0:
        result = _T("CBS_E_MANIFEST_VALIDATION_DUPLICATE_ELEMENT");
        break;
      case 1:
        result = _T("CBS_E_MANIFEST_VALIDATION_MISSING_REQUIRED_ATTRIBUTES");
        break;
      case 2:
        result = _T("CBS_E_MANIFEST_VALIDATION_MISSING_REQUIRED_ELEMENTS");
        break;
      case 3:
        result = _T("CBS_E_MANIFEST_VALIDATION_UPDATES_PARENT_MISSING");
        break;
      case 4:
        result = _T("CBS_E_INVALID_INSTALL_STATE");
        break;
      case 5:
        result = _T("CBS_E_INVALID_CONFIG_VALUE");
        break;
      case 6:
        result = _T("CBS_E_INVALID_CARDINALITY");
        break;
      case 7:
        result = _T("CBS_E_DPX_JOB_STATE_SAVED");
        break;
      case 8:
        result = _T("CBS_E_PACKAGE_DELETED");
        break;
      case 9:
        result = _T("CBS_E_IDENTITY_MISMATCH");
        break;
      case 10:
        result = _T("CBS_E_DUPLICATE_UPDATENAME");
        break;
      case 11:
        result = _T("CBS_E_INVALID_DRIVER_OPERATION_KEY");
        break;
      case 12:
        result = _T("CBS_E_UNEXPECTED_PROCESSOR_ARCHITECTURE");
        break;
      case 13:
        result = _T("CBS_E_EXCESSIVE_EVALUATION");
        break;
      case 14:
        result = _T("CBS_E_CYCLE_EVALUATION");
        break;
      case 15:
        result = _T("CBS_E_NOT_APPLICABLE ");
        break;
      case 16:
        result = _T("CBS_E_SOURCE_MISSING");
        break;
      case 17:
        result = _T("CBS_E_CANCEL");
        break;
      case 18:
        result = _T("CBS_E_ABORT");
        break;
      case 19:
        result = _T("CBS_E_ILLEGAL_COMPONENT_UPDATE");
        break;
      case 20:
        result = _T("CBS_E_NEW_SERVICING_STACK_REQUIRED");
        break;
      case 21:
        result = _T("CBS_E_SOURCE_NOT_IN_LIST");
        break;
      case 22:
        result = _T("CBS_E_CANNOT_UNINSTALL");
        break;
      case 23:
        result = _T("CBS_E_PENDING_VICTIM");
        break;
      case 24:
        result = _T("CBS_E_STACK_SHUTDOWN_REQUIRED");
        break;
      case 25:
        result = _T("CBS_E_INSUFFICIENT_DISK_SPACE");
        break;
      case 26:
        result = _T("CBS_E_AC_POWER_REQUIRED");
        break;
      case 27:
        result = _T("CBS_E_STACK_UPDATE_FAILED_REBOOT_REQUIRED");
        break;
      case 28:
        result = _T("CBS_E_SQM_REPORT_IGNORED_AI_FAILURES_ON_TRANSACTION_RESOLVE");
        break;
      case 29:
        result = _T("CBS_E_DEPENDENT_FAILURE");
        break;
      case 30:
        result = _T("CBS_E_PAC_INITIAL_FAILURE");
        break;
      case 31:
        result = _T("CBS_E_NOT_ALLOWED_OFFLINE");
        break;
      case 32:
        result = _T("CBS_E_EXCLUSIVE_WOULD_MERGE");
        break;
      case 33:
        result = _T("CBS_E_IMAGE_UNSERVICEABLE");
        break;
      case 34:
        result = _T("CBS_E_STORE_CORRUPTION");
        break;
      case 35:
        result = _T("CBS_E_STORE_TOO_MUCH_CORRUPTION");
        break;
      case 36:
        result = _T("CBS_S_STACK_RESTART_REQUIRED");
        break;
      case 49:
        result = _T("CBS_E_SESSION_CORRUPT");
        break;
      case 50:
        result = _T("CBS_E_SESSION_INTERRUPTED");
        break;
      case 51:
        result = _T("CBS_E_SESSION_FINALIZED");
        break;
      case 52:
        result = _T("CBS_E_SESSION_READONLY");
        break;
      default:
        return nullptr;
      }
    }
  }
  else
  {
    if (hr == -2146498546)
      return _T("CBS_E_MANIFEST_VALIDATION_DUPLICATE_ATTRIBUTES");
    if (hr > (signed int)0x800F022A)
    {
      if (hr > -2146499840)
      {
        switch (hr + 2146498560)
        {
        case 0:
          result = _T("CBS_E_INTERNAL_ERROR");
          break;
        case 1:
          result = _T("CBS_E_NOT_INITIALIZED");
          break;
        case 2:
          result = _T("CBS_E_ALREADY_INITIALIZED");
          break;
        case 3:
          result = _T("CBS_E_INVALID_PARAMETER");
          break;
        case 4:
          result = _T("CBS_E_OPEN_FAILED");
          break;
        case 5:
          result = _T("CBS_E_INVALID_PACKAGE");
          break;
        case 6:
          result = _T("CBS_E_PENDING");
          break;
        case 7:
          result = _T("CBS_E_NOT_INSTALLABLE");
          break;
        case 8:
          result = _T("CBS_E_IMAGE_NOT_ACCESSIBLE");
          break;
        case 9:
          result = _T("CBS_E_ARRAY_ELEMENT_MISSING");
          break;
        case 10:
          result = _T("CBS_E_REESTABLISH_SESSION");
          break;
        case 11:
          result = _T("CBS_E_PROPERTY_NOT_AVAILABLE");
          break;
        case 12:
          result = _T("CBS_E_UNKNOWN_UPDATE");
          break;
        case 13:
          result = _T("CBS_E_MANIFEST_INVALID_ITEM");
          break;
        default:
          return nullptr;
        }
      }
      else if (hr == -2146499840)
      {
        result = _T("SPAPI_E_UNRECOVERABLE_STACK_OVERFLOW");
      }
      else
      {
        switch (hr + 2146500053)
        {
        case 0:
          result = _T("SPAPI_E_DI_DONT_INSTALL");
          break;
        case 1:
          result = _T("SPAPI_E_INVALID_FILTER_DRIVER");
          break;
        case 2:
          result = _T("SPAPI_E_NON_WINDOWS_NT_DRIVER");
          break;
        case 3:
          result = _T("SPAPI_E_NON_WINDOWS_DRIVER");
          break;
        case 4:
          result = _T("SPAPI_E_NO_CATALOG_FOR_OEM_INF");
          break;
        case 5:
          result = _T("SPAPI_E_DEVINSTALL_QUEUE_NONNATIVE");
          break;
        case 6:
          result = _T("SPAPI_E_NOT_DISABLEABLE");
          break;
        case 7:
          result = _T("SPAPI_E_CANT_REMOVE_DEVINST");
          break;
        case 8:
          result = _T("SPAPI_E_INVALID_TARGET");
          break;
        case 9:
          result = _T("SPAPI_E_DRIVER_NONNATIVE");
          break;
        case 10:
          result = _T("SPAPI_E_IN_WOW64");
          break;
        case 11:
          result = _T("SPAPI_E_SET_SYSTEM_RESTORE_POINT");
          break;
        case 12:
          result = _T("SPAPI_E_INCORRECTLY_COPIED_INF");
          break;
        case 13:
          result = _T("SPAPI_E_SCE_DISABLED");
          break;
        case 14:
          result = _T("SPAPI_E_UNKNOWN_EXCEPTION");
          break;
        case 15:
          result = _T("SPAPI_E_PNP_REGISTRY_ERROR");
          break;
        case 16:
          result = _T("SPAPI_E_REMOTE_REQUEST_UNSUPPORTED");
          break;
        case 17:
          result = _T("SPAPI_E_NOT_AN_INSTALLED_OEM_INF");
          break;
        case 18:
          result = _T("SPAPI_E_INF_IN_USE_BY_DEVICES");
          break;
        case 19:
          result = _T("SPAPI_E_DI_FUNCTION_OBSOLETE");
          break;
        case 20:
          result = _T("SPAPI_E_NO_AUTHENTICODE_CATALOG");
          break;
        case 21:
          result = _T("SPAPI_E_AUTHENTICODE_DISALLOWED");
          break;
        case 22:
          result = _T("SPAPI_E_AUTHENTICODE_TRUSTED_PUBLISHER");
          break;
        case 23:
          result = _T("SPAPI_E_AUTHENTICODE_TRUST_NOT_ESTABLISHED");
          break;
        case 24:
          result = _T("SPAPI_E_AUTHENTICODE_PUBLISHER_NOT_TRUSTED");
          break;
        case 25:
          result = _T("SPAPI_E_SIGNATURE_OSATTRIBUTE_MISMATCH");
          break;
        case 26:
          result = _T("SPAPI_E_ONLY_VALIDATE_VIA_AUTHENTICODE");
          break;
        case 27:
          result = _T("SPAPI_E_DEVICE_INSTALLER_NOT_READY");
          break;
        case 28:
          result = _T("SPAPI_E_DRIVER_STORE_ADD_FAILED");
          break;
        case 29:
          result = _T("SPAPI_E_DEVICE_INSTALL_BLOCKED");
          break;
        case 30:
          result = _T("SPAPI_E_DRIVER_INSTALL_BLOCKED");
          break;
        case 31:
          result = _T("SPAPI_E_WRONG_INF_TYPE");
          break;
        case 32:
          result = _T("SPAPI_E_FILE_HASH_NOT_IN_CATALOG");
          break;
        case 33:
          result = _T("SPAPI_E_DRIVER_STORE_DELETE_FAILED");
          break;
        default:
          return nullptr;
        }
      }
    }
    else
    {
      if (hr == 0x800F022A)
        return _T("SPAPI_E_INVALID_INF_LOGCONFIG");
      if (hr > 0x800F0211)
      {
        switch (hr + 0x7FF0FDEE)
        {
        case 0:
          result = _T("SPAPI_E_DEVINFO_LIST_LOCKED");
          break;
        case 1:
          result = _T("SPAPI_E_DEVINFO_DATA_LOCKED");
          break;
        case 2:
          result = _T("SPAPI_E_DI_BAD_PATH");
          break;
        case 3:
          result = _T("SPAPI_E_NO_CLASSINSTALL_PARAMS");
          break;
        case 4:
          result = _T("SPAPI_E_FILEQUEUE_LOCKED");
          break;
        case 5:
          result = _T("SPAPI_E_BAD_SERVICE_INSTALLSECT");
          break;
        case 6:
          result = _T("SPAPI_E_NO_CLASS_DRIVER_LIST");
          break;
        case 7:
          result = _T("SPAPI_E_NO_ASSOCIATED_SERVICE");
          break;
        case 8:
          result = _T("SPAPI_E_NO_DEFAULT_DEVICE_INTERFACE");
          break;
        case 9:
          result = _T("SPAPI_E_DEVICE_INTERFACE_ACTIVE");
          break;
        case 10:
          result = _T("SPAPI_E_DEVICE_INTERFACE_REMOVED");
          break;
        case 11:
          result = _T("SPAPI_E_BAD_INTERFACE_INSTALLSECT");
          break;
        case 12:
          result = _T("SPAPI_E_NO_SUCH_INTERFACE_CLASS");
          break;
        case 13:
          result = _T("SPAPI_E_INVALID_REFERENCE_STRING");
          break;
        case 14:
          result = _T("SPAPI_E_INVALID_MACHINENAME");
          break;
        case 15:
          result = _T("SPAPI_E_REMOTE_COMM_FAILURE");
          break;
        case 16:
          result = _T("SPAPI_E_MACHINE_UNAVAILABLE");
          break;
        case 17:
          result = _T("SPAPI_E_NO_CONFIGMGR_SERVICES");
          break;
        case 18:
          result = _T("SPAPI_E_INVALID_PROPPAGE_PROVIDER");
          break;
        case 19:
          result = _T("SPAPI_E_NO_SUCH_DEVICE_INTERFACE");
          break;
        case 20:
          result = _T("SPAPI_E_DI_POSTPROCESSING_REQUIRED");
          break;
        case 21:
          result = _T("SPAPI_E_INVALID_COINSTALLER");
          break;
        case 22:
          result = _T("SPAPI_E_NO_COMPAT_DRIVERS");
          break;
        case 23:
          result = _T("SPAPI_E_NO_DEVICE_ICON");
          break;
        default:
          return nullptr;
        }
      }
      else
      {
        if (hr == 0x800F0211)
          return _T("SPAPI_E_NO_DEVICE_SELECTED");
        if (hr <= -2146500092)
        {
          if (hr == -2146500092)
            return _T("SPAPI_E_KEY_DOES_NOT_EXIST");
          if (hr > -2146500350)
          {
            switch (hr)
            {
            case -2146500349:
              return _T("SPAPI_E_NO_BACKUP");
            case -2146500096:
              return _T("SPAPI_E_NO_ASSOCIATED_CLASS");
            case -2146500095:
              return _T("SPAPI_E_CLASS_MISMATCH");
            case -2146500094:
              return _T("SPAPI_E_DUPLICATE_FOUND");
            case -2146500093:
              return _T("SPAPI_E_NO_DRIVER_SELECTED");
            }
          }
          else
          {
            switch (hr)
            {
            case -2146500350:
              return _T("SPAPI_E_LINE_NOT_FOUND");
            case -2146500608:
              return _T("SPAPI_E_EXPECTED_SECTION_NAME");
            case -2146500607:
              return _T("SPAPI_E_BAD_SECTION_NAME_LINE");
            case -2146500606:
              return _T("SPAPI_E_SECTION_NAME_TOO_LONG");
            case -2146500605:
              return _T("SPAPI_E_GENERAL_SYNTAX");
            case -2146500352:
              return _T("SPAPI_E_WRONG_INF_STYLE");
            case -2146500351:
              return _T("SPAPI_E_SECTION_NOT_FOUND");
            }
          }
          return nullptr;
        }
        switch (hr + 2146500091)
        {
        case 0:
          result = _T("SPAPI_E_INVALID_DEVINST_NAME");
          break;
        case 1:
          result = _T("SPAPI_E_INVALID_CLASS");
          break;
        case 2:
          result = _T("SPAPI_E_DEVINST_ALREADY_EXISTS");
          break;
        case 3:
          result = _T("SPAPI_E_DEVINFO_NOT_REGISTERED");
          break;
        case 4:
          result = _T("SPAPI_E_INVALID_REG_PROPERTY");
          break;
        case 5:
          result = _T("SPAPI_E_NO_INF");
          break;
        case 6:
          result = _T("SPAPI_E_NO_SUCH_DEVINST");
          break;
        case 7:
          result = _T("SPAPI_E_CANT_LOAD_CLASS_ICON");
          break;
        case 8:
          result = _T("SPAPI_E_INVALID_CLASS_INSTALLER");
          break;
        case 9:
          result = _T("SPAPI_E_DI_DO_DEFAULT");
          break;
        case 10:
          result = _T("SPAPI_E_DI_NOFILECOPY");
          break;
        case 11:
          result = _T("SPAPI_E_INVALID_HWPROFILE");
          break;
        default:
          return nullptr;
        }
      }
    }
  }
  return result;
}

std::wstring TextizeHresult(const HRESULT hr)
{
  auto strhr = NUM_TO_WSTR(hr);
  LPCTSTR result = nullptr;

  if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)(&result), 0, nullptr))
  {
    result = TextizeSetupHresult(hr);
  }

  return result ? result : strhr.c_str();
}

LPCSTR TextizeSetupHresultA(HRESULT hr)
{
  LPCSTR result;
  int tmp1, tmp2;

  if (hr > -2146498546)
  {
    if (hr > -2146498481)
    {
      if (hr > -2146496512)
      {
        tmp1 = hr - 985089;
        if (tmp1)
        {
          tmp2 = tmp1 - 1;
          if (tmp2)
          {
            if (tmp2 != 1)
              return nullptr;
            result = "CBS_S_STACK_SHUTDOWN_REQUIRED";
          }
          else
          {
            result = "CBS_S_ALREADY_EXISTS";
          }
        }
        else
        {
          result = "CBS_S_BUSY";
        }
      }
      else if (hr == -2146496512)
      {
        result = "SPAPI_E_ERROR_NOT_INSTALLED";
      }
      else
      {
        switch (hr)
        {
        case (int)0x800F0900:
          result = "CBS_E_XML_PARSER_FAILURE";
          break;
        case (int)0x800F0901:
          result = "CBS_E_MANIFEST_VALIDATION_MULTIPLE_UPDATE_COMPONENT_ON_SAME_FAMILY_NOT_ALLOWED";
          break;
        case (int)0x800F0902:
          result = "CBS_E_BUSY";
          break;
        case (int)0x800F0903:
          result = "CBS_E_INVALID_RECALL";
          break;
        case (int)0x800F0904:
          result = "CBS_E_MORE_THAN_ONE_ACTIVE_EDITION";
          break;
        case (int)0x800F0905:
          result = "CBS_E_NO_ACTIVE_EDITION";
          break;
        case (int)0x800F0906:
          result = "CBS_E_DOWNLOAD_FAILURE";
          break;
        case (int)0x800F0907:
          result = "CBS_E_GROUPPOLICY_DISALLOWED";
          break;
        case (int)0x800F0908:
          result = "CBS_E_METERED_NETWORK";
          break;
        case (int)0x800F0909:
          result = "CBS_E_PUBLIC_OBJECT_LEAK";
          break;
        case (int)0x800F090B:
          result = "CBS_E_REPAIR_PACKAGE_CORRUPT";
          break;
        case (int)0x800F090C:
          result = "CBS_E_COMPONENT_NOT_INSTALLED_BY_CBS";
          break;
        case (int)0x800F090D:
          result = "CBS_E_MISSING_PACKAGE_MAPPING_INDEX";
          break;
        case (int)0x800F090E:
          result = "CBS_E_EMPTY_PACKAGE_MAPPING_INDEX";
          break;
        case (int)0x800F090F:
          result = "CBS_E_WINDOWS_UPDATE_SEARCH_FAILURE";
          break;
        case (int)0x800F0910:
          result = "CBS_E_WINDOWS_AUTOMATIC_UPDATE_SETTING_DISALLOWED";
          break;
        case (int)0x800F0920:
          result = "CBS_E_HANG_DETECTED";
          break;
        case (int)0x800F0921:
          result = "CBS_E_PRIMITIVES_FAILED";
          break;
        case (int)0x800F0922:
          result = "CBS_E_INSTALLERS_FAILED";
          break;
        case (int)0x800F0923:
          result = "CBS_E_SAFEMODE_ENTERED";
          break;
        case (int)0x800F0924:
          result = "CBS_E_SESSIONS_LEAKED";
          break;
        case (int)0x800F0925:
          result = "CBS_E_INVALID_EXECUTESTATE";
          break;
        case (int)0x800F0940:
          result = "CBS_E_WUSUS_MAPPING_UNAVAILABLE";
          break;
        case (int)0x800F0941:
          result = "CBS_E_WU_MAPPING_UNAVAILABLE";
          break;
        case (int)0x800F0942:
          result = "CBS_E_WUSUS_BYPASS_MAPPING_UNAVAILABLE";
          break;
        case (int)0x800F0943:
          result = "CBS_E_WUSUS_MISSING_PACKAGE_MAPPING_INDEX";
          break;
        case (int)0x800F0944:
          result = "CBS_E_WU_MISSING_PACKAGE_MAPPING_INDEX";
          break;
        case (int)0x800F0945:
          result = "CBS_E_WUSUS_BYPASS_MISSING_PACKAGE_MAPPING_INDEX";
          break;
        case (int)0x800F0946:
          result = "CBS_E_SOURCE_MISSING_FROM_WUSUS_CAB";
          break;
        case (int)0x800F0947:
          result = "CBS_E_SOURCE_MISSING_FROM_WUSUS_EXPRESS";
          break;
        case (int)0x800F0948:
          result = "CBS_E_SOURCE_MISSING_FROM_WU_CAB";
          break;
        case (int)0x800F0949:
          result = "CBS_E_SOURCE_MISSING_FROM_WU_EXPRESS";
          break;
        case (int)0x800F094A:
          result = "CBS_E_SOURCE_MISSING_FROM_WUSUS_BYPASS_CAB";
          break;
        case (int)0x800F094B:
          result = "CBS_E_SOURCE_MISSING_FROM_WUSUS_BYPASS_EXPRESS";
          break;
        case (int)0x800F094C:
          result = "CBS_E_3RD_PARTY_MAPPING_UNAVAILABLE";
          break;
        case (int)0x800F094D:
          result = "CBS_E_3RD_PARTY_MISSING_PACKAGE_MAPPING_INDEX";
          break;
        case (int)0x800F094E:
          result = "CBS_E_SOURCE_MISSING_FROM_3RD_PARTY_CAB";
          break;
        case (int)0x800F094F:
          result = "CBS_E_SOURCE_MISSING_FROM_3RD_PARTY_EXPRESS";
          break;
        case (int)0x800F0950:
          result = "CBS_E_INVALID_WINDOWS_UPDATE_COUNT";
          break;
        case (int)0x800F0951:
          result = "CBS_E_INVALID_NO_PRODUCT_REGISTERED";
          break;
        case (int)0x800F0952:
          result = "CBS_E_INVALID_ACTION_LIST_PACKAGE_COUNT";
          break;
        case (int)0x800F0953:
          result = "CBS_E_INVALID_ACTION_LIST_INSTALL_REASON";
          break;
        case (int)0x800F0954:
          result = "CBS_E_INVALID_WINDOWS_UPDATE_COUNT_WSUS";
          break;
        case (int)0x800F0955:
          result = "CBS_E_INVALID_PACKAGE_REQUEST_ON_MULTILINGUAL_FOD";
          break;
        case (int)0x800F0980:
          result = "PSFX_E_DELTA_NOT_SUPPORTED_FOR_COMPONENT";
          break;
        case (int)0x800F0981:
          result = "PSFX_E_REVERSE_AND_FORWARD_DELTAS_MISSING";
          break;
        case (int)0x800F0982:
          result = "PSFX_E_MATCHING_COMPONENT_NOT_FOUND";
          break;
        case (int)0x800F0983:
          result = "PSFX_E_MATCHING_COMPONENT_DIRECTORY_MISSING";
          break;
        case (int)0x800F0984:
          result = "PSFX_E_MATCHING_BINARY_MISSING";
          break;
        case (int)0x800F0985:
          result = "PSFX_E_APPLY_REVERSE_DELTA_FAILED";
          break;
        case (int)0x800F0986:
          result = "PSFX_E_APPLY_FORWARD_DELTA_FAILED";
          break;
        case (int)0x800F0987:
          result = "PSFX_E_NULL_DELTA_HYDRATION_FAILED";
          break;
        case (int)0x800F0988:
          result = "PSFX_E_INVALID_DELTA_COMBINATION";
          break;
        case (int)0x800F0989:
          result = "PSFX_E_REVERSE_DELTA_MISSING";
          break;
        default:
          return nullptr;
        }
      }
    }
    else if (hr == -2146498481)
    {
      result = "CBS_E_RESOLVE_FAILED";
    }
    else
    {
      switch (hr + 2146498545)
      {
      case 0:
        result = "CBS_E_MANIFEST_VALIDATION_DUPLICATE_ELEMENT";
        break;
      case 1:
        result = "CBS_E_MANIFEST_VALIDATION_MISSING_REQUIRED_ATTRIBUTES";
        break;
      case 2:
        result = "CBS_E_MANIFEST_VALIDATION_MISSING_REQUIRED_ELEMENTS";
        break;
      case 3:
        result = "CBS_E_MANIFEST_VALIDATION_UPDATES_PARENT_MISSING";
        break;
      case 4:
        result = "CBS_E_INVALID_INSTALL_STATE";
        break;
      case 5:
        result = "CBS_E_INVALID_CONFIG_VALUE";
        break;
      case 6:
        result = "CBS_E_INVALID_CARDINALITY";
        break;
      case 7:
        result = "CBS_E_DPX_JOB_STATE_SAVED";
        break;
      case 8:
        result = "CBS_E_PACKAGE_DELETED";
        break;
      case 9:
        result = "CBS_E_IDENTITY_MISMATCH";
        break;
      case 10:
        result = "CBS_E_DUPLICATE_UPDATENAME";
        break;
      case 11:
        result = "CBS_E_INVALID_DRIVER_OPERATION_KEY";
        break;
      case 12:
        result = "CBS_E_UNEXPECTED_PROCESSOR_ARCHITECTURE";
        break;
      case 13:
        result = "CBS_E_EXCESSIVE_EVALUATION";
        break;
      case 14:
        result = "CBS_E_CYCLE_EVALUATION";
        break;
      case 15:
        result = "CBS_E_NOT_APPLICABLE ";
        break;
      case 16:
        result = "CBS_E_SOURCE_MISSING";
        break;
      case 17:
        result = "CBS_E_CANCEL";
        break;
      case 18:
        result = "CBS_E_ABORT";
        break;
      case 19:
        result = "CBS_E_ILLEGAL_COMPONENT_UPDATE";
        break;
      case 20:
        result = "CBS_E_NEW_SERVICING_STACK_REQUIRED";
        break;
      case 21:
        result = "CBS_E_SOURCE_NOT_IN_LIST";
        break;
      case 22:
        result = "CBS_E_CANNOT_UNINSTALL";
        break;
      case 23:
        result = "CBS_E_PENDING_VICTIM";
        break;
      case 24:
        result = "CBS_E_STACK_SHUTDOWN_REQUIRED";
        break;
      case 25:
        result = "CBS_E_INSUFFICIENT_DISK_SPACE";
        break;
      case 26:
        result = "CBS_E_AC_POWER_REQUIRED";
        break;
      case 27:
        result = "CBS_E_STACK_UPDATE_FAILED_REBOOT_REQUIRED";
        break;
      case 28:
        result = "CBS_E_SQM_REPORT_IGNORED_AI_FAILURES_ON_TRANSACTION_RESOLVE";
        break;
      case 29:
        result = "CBS_E_DEPENDENT_FAILURE";
        break;
      case 30:
        result = "CBS_E_PAC_INITIAL_FAILURE";
        break;
      case 31:
        result = "CBS_E_NOT_ALLOWED_OFFLINE";
        break;
      case 32:
        result = "CBS_E_EXCLUSIVE_WOULD_MERGE";
        break;
      case 33:
        result = "CBS_E_IMAGE_UNSERVICEABLE";
        break;
      case 34:
        result = "CBS_E_STORE_CORRUPTION";
        break;
      case 35:
        result = "CBS_E_STORE_TOO_MUCH_CORRUPTION";
        break;
      case 36:
        result = "CBS_S_STACK_RESTART_REQUIRED";
        break;
      case 49:
        result = "CBS_E_SESSION_CORRUPT";
        break;
      case 50:
        result = "CBS_E_SESSION_INTERRUPTED";
        break;
      case 51:
        result = "CBS_E_SESSION_FINALIZED";
        break;
      case 52:
        result = "CBS_E_SESSION_READONLY";
        break;
      default:
        return nullptr;
      }
    }
  }
  else
  {
    if (hr == -2146498546)
      return "CBS_E_MANIFEST_VALIDATION_DUPLICATE_ATTRIBUTES";
    if (hr > (signed int)0x800F022A)
    {
      if (hr > -2146499840)
      {
        switch (hr + 2146498560)
        {
        case 0:
          result = "CBS_E_INTERNAL_ERROR";
          break;
        case 1:
          result = "CBS_E_NOT_INITIALIZED";
          break;
        case 2:
          result = "CBS_E_ALREADY_INITIALIZED";
          break;
        case 3:
          result = "CBS_E_INVALID_PARAMETER";
          break;
        case 4:
          result = "CBS_E_OPEN_FAILED";
          break;
        case 5:
          result = "CBS_E_INVALID_PACKAGE";
          break;
        case 6:
          result = "CBS_E_PENDING";
          break;
        case 7:
          result = "CBS_E_NOT_INSTALLABLE";
          break;
        case 8:
          result = "CBS_E_IMAGE_NOT_ACCESSIBLE";
          break;
        case 9:
          result = "CBS_E_ARRAY_ELEMENT_MISSING";
          break;
        case 10:
          result = "CBS_E_REESTABLISH_SESSION";
          break;
        case 11:
          result = "CBS_E_PROPERTY_NOT_AVAILABLE";
          break;
        case 12:
          result = "CBS_E_UNKNOWN_UPDATE";
          break;
        case 13:
          result = "CBS_E_MANIFEST_INVALID_ITEM";
          break;
        default:
          return nullptr;
        }
      }
      else if (hr == -2146499840)
      {
        result = "SPAPI_E_UNRECOVERABLE_STACK_OVERFLOW";
      }
      else
      {
        switch (hr + 2146500053)
        {
        case 0:
          result = "SPAPI_E_DI_DONT_INSTALL";
          break;
        case 1:
          result = "SPAPI_E_INVALID_FILTER_DRIVER";
          break;
        case 2:
          result = "SPAPI_E_NON_WINDOWS_NT_DRIVER";
          break;
        case 3:
          result = "SPAPI_E_NON_WINDOWS_DRIVER";
          break;
        case 4:
          result = "SPAPI_E_NO_CATALOG_FOR_OEM_INF";
          break;
        case 5:
          result = "SPAPI_E_DEVINSTALL_QUEUE_NONNATIVE";
          break;
        case 6:
          result = "SPAPI_E_NOT_DISABLEABLE";
          break;
        case 7:
          result = "SPAPI_E_CANT_REMOVE_DEVINST";
          break;
        case 8:
          result = "SPAPI_E_INVALID_TARGET";
          break;
        case 9:
          result = "SPAPI_E_DRIVER_NONNATIVE";
          break;
        case 10:
          result = "SPAPI_E_IN_WOW64";
          break;
        case 11:
          result = "SPAPI_E_SET_SYSTEM_RESTORE_POINT";
          break;
        case 12:
          result = "SPAPI_E_INCORRECTLY_COPIED_INF";
          break;
        case 13:
          result = "SPAPI_E_SCE_DISABLED";
          break;
        case 14:
          result = "SPAPI_E_UNKNOWN_EXCEPTION";
          break;
        case 15:
          result = "SPAPI_E_PNP_REGISTRY_ERROR";
          break;
        case 16:
          result = "SPAPI_E_REMOTE_REQUEST_UNSUPPORTED";
          break;
        case 17:
          result = "SPAPI_E_NOT_AN_INSTALLED_OEM_INF";
          break;
        case 18:
          result = "SPAPI_E_INF_IN_USE_BY_DEVICES";
          break;
        case 19:
          result = "SPAPI_E_DI_FUNCTION_OBSOLETE";
          break;
        case 20:
          result = "SPAPI_E_NO_AUTHENTICODE_CATALOG";
          break;
        case 21:
          result = "SPAPI_E_AUTHENTICODE_DISALLOWED";
          break;
        case 22:
          result = "SPAPI_E_AUTHENTICODE_TRUSTED_PUBLISHER";
          break;
        case 23:
          result = "SPAPI_E_AUTHENTICODE_TRUST_NOT_ESTABLISHED";
          break;
        case 24:
          result = "SPAPI_E_AUTHENTICODE_PUBLISHER_NOT_TRUSTED";
          break;
        case 25:
          result = "SPAPI_E_SIGNATURE_OSATTRIBUTE_MISMATCH";
          break;
        case 26:
          result = "SPAPI_E_ONLY_VALIDATE_VIA_AUTHENTICODE";
          break;
        case 27:
          result = "SPAPI_E_DEVICE_INSTALLER_NOT_READY";
          break;
        case 28:
          result = "SPAPI_E_DRIVER_STORE_ADD_FAILED";
          break;
        case 29:
          result = "SPAPI_E_DEVICE_INSTALL_BLOCKED";
          break;
        case 30:
          result = "SPAPI_E_DRIVER_INSTALL_BLOCKED";
          break;
        case 31:
          result = "SPAPI_E_WRONG_INF_TYPE";
          break;
        case 32:
          result = "SPAPI_E_FILE_HASH_NOT_IN_CATALOG";
          break;
        case 33:
          result = "SPAPI_E_DRIVER_STORE_DELETE_FAILED";
          break;
        default:
          return nullptr;
        }
      }
    }
    else
    {
      if (hr == 0x800F022A)
        return "SPAPI_E_INVALID_INF_LOGCONFIG";
      if (hr > (signed int)0x800F0211)
      {
        switch (hr + 0x7FF0FDEE)
        {
        case 0:
          result = "SPAPI_E_DEVINFO_LIST_LOCKED";
          break;
        case 1:
          result = "SPAPI_E_DEVINFO_DATA_LOCKED";
          break;
        case 2:
          result = "SPAPI_E_DI_BAD_PATH";
          break;
        case 3:
          result = "SPAPI_E_NO_CLASSINSTALL_PARAMS";
          break;
        case 4:
          result = "SPAPI_E_FILEQUEUE_LOCKED";
          break;
        case 5:
          result = "SPAPI_E_BAD_SERVICE_INSTALLSECT";
          break;
        case 6:
          result = "SPAPI_E_NO_CLASS_DRIVER_LIST";
          break;
        case 7:
          result = "SPAPI_E_NO_ASSOCIATED_SERVICE";
          break;
        case 8:
          result = "SPAPI_E_NO_DEFAULT_DEVICE_INTERFACE";
          break;
        case 9:
          result = "SPAPI_E_DEVICE_INTERFACE_ACTIVE";
          break;
        case 10:
          result = "SPAPI_E_DEVICE_INTERFACE_REMOVED";
          break;
        case 11:
          result = "SPAPI_E_BAD_INTERFACE_INSTALLSECT";
          break;
        case 12:
          result = "SPAPI_E_NO_SUCH_INTERFACE_CLASS";
          break;
        case 13:
          result = "SPAPI_E_INVALID_REFERENCE_STRING";
          break;
        case 14:
          result = "SPAPI_E_INVALID_MACHINENAME";
          break;
        case 15:
          result = "SPAPI_E_REMOTE_COMM_FAILURE";
          break;
        case 16:
          result = "SPAPI_E_MACHINE_UNAVAILABLE";
          break;
        case 17:
          result = "SPAPI_E_NO_CONFIGMGR_SERVICES";
          break;
        case 18:
          result = "SPAPI_E_INVALID_PROPPAGE_PROVIDER";
          break;
        case 19:
          result = "SPAPI_E_NO_SUCH_DEVICE_INTERFACE";
          break;
        case 20:
          result = "SPAPI_E_DI_POSTPROCESSING_REQUIRED";
          break;
        case 21:
          result = "SPAPI_E_INVALID_COINSTALLER";
          break;
        case 22:
          result = "SPAPI_E_NO_COMPAT_DRIVERS";
          break;
        case 23:
          result = "SPAPI_E_NO_DEVICE_ICON";
          break;
        default:
          return nullptr;
        }
      }
      else
      {
        if (hr == 0x800F0211)
          return "SPAPI_E_NO_DEVICE_SELECTED";
        if (hr <= -2146500092)
        {
          if (hr == -2146500092)
            return "SPAPI_E_KEY_DOES_NOT_EXIST";
          if (hr > -2146500350)
          {
            switch (hr)
            {
            case -2146500349:
              return "SPAPI_E_NO_BACKUP";
            case -2146500096:
              return "SPAPI_E_NO_ASSOCIATED_CLASS";
            case -2146500095:
              return "SPAPI_E_CLASS_MISMATCH";
            case -2146500094:
              return "SPAPI_E_DUPLICATE_FOUND";
            case -2146500093:
              return "SPAPI_E_NO_DRIVER_SELECTED";
            }
          }
          else
          {
            switch (hr)
            {
            case -2146500350:
              return "SPAPI_E_LINE_NOT_FOUND";
            case -2146500608:
              return "SPAPI_E_EXPECTED_SECTION_NAME";
            case -2146500607:
              return "SPAPI_E_BAD_SECTION_NAME_LINE";
            case -2146500606:
              return "SPAPI_E_SECTION_NAME_TOO_LONG";
            case -2146500605:
              return "SPAPI_E_GENERAL_SYNTAX";
            case -2146500352:
              return "SPAPI_E_WRONG_INF_STYLE";
            case -2146500351:
              return "SPAPI_E_SECTION_NOT_FOUND";
            }
          }
          return nullptr;
        }
        switch (hr + 2146500091)
        {
        case 0:
          result = "SPAPI_E_INVALID_DEVINST_NAME";
          break;
        case 1:
          result = "SPAPI_E_INVALID_CLASS";
          break;
        case 2:
          result = "SPAPI_E_DEVINST_ALREADY_EXISTS";
          break;
        case 3:
          result = "SPAPI_E_DEVINFO_NOT_REGISTERED";
          break;
        case 4:
          result = "SPAPI_E_INVALID_REG_PROPERTY";
          break;
        case 5:
          result = "SPAPI_E_NO_INF";
          break;
        case 6:
          result = "SPAPI_E_NO_SUCH_DEVINST";
          break;
        case 7:
          result = "SPAPI_E_CANT_LOAD_CLASS_ICON";
          break;
        case 8:
          result = "SPAPI_E_INVALID_CLASS_INSTALLER";
          break;
        case 9:
          result = "SPAPI_E_DI_DO_DEFAULT";
          break;
        case 10:
          result = "SPAPI_E_DI_NOFILECOPY";
          break;
        case 11:
          result = "SPAPI_E_INVALID_HWPROFILE";
          break;
        default:
          return nullptr;
        }
      }
    }
  }
  return result;
}

std::string TextizeHresultA(const HRESULT hr)
{
  auto strhr = NUM_TO_STR(hr);
  LPCSTR result = nullptr;

  if (!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR)(&result), 0, nullptr))
  {
    result = TextizeSetupHresultA(hr);
  }

  return result ? result : strhr.c_str();
}



void InsertLine(const UINT uSize)
{
  for (UINT iCh = 0; iCh < uSize; iCh++)
  {
    _puttchar(_T('='));
  }
  _puttchar(_T('\n'));
}

// ==============================
// Package Manager Snippets
// ==============================
ComPtr<ICbsPackage> GetFoundationPackage()
{
  BEGIN_ERROR_HANDLING();

  ComPtr<IEnumCbsIdentity> pIdents;
  CHECK(g_sess->EnumeratePackages(0x1b0, &pIdents), "Failed to enum pkgs.");

  for (auto& pIdent : GetIEnumComPtrVector<ICbsIdentity, IEnumCbsIdentity>(pIdents)) {
    PWSTR strId;
    pIdent->GetStringId(&strId);
    
    if (std::wstring tempStrId = strId; tempStrId
      .find(L"Microsoft-Windows-Foundation-Package~31bf3856ad364e35~")
        != std::wstring::npos) {
      ComPtr<ICbsPackage> pPkg;
      CHECK(g_sess->OpenPackage(0, pIdent, nullptr, &pPkg), "Failed to open pkg with identity.");
      return pPkg;
    }
  }

  return nullptr;
}

HRESULT PrintPackageInfo(const ComPtr<ICbsPackage>& pPkg, const bool bIns)
{
  BEGIN_ERROR_HANDLING();

  if (!pPkg) RET_HR_LOG(E_INVALIDARG, "The pointer to the pkg is invalid.");

#define GPkgProp(x, y) \
unique_malloc_ptr<wchar_t> sz##y; \
{\
  PWSTR rawsz##y; \
  CHECK(pPkg->GetProperty(CbsPackageProperty::x, &(rawsz##y)), "OK");\
  (sz##y).reset(rawsz##y); \
}

  InsertLine(LineSizeLong);

  GPkgProp(DisplayName, Name);
  GPkgProp(Description, Desc);
  GPkgProp(IdentityString, Ident);
  GPkgProp(ReleaseType, ReType);
  GPkgProp(PackageSize, Size);
  GPkgProp(SupportInformation, Sup);

  CbsInstallState stIns = CbsInstallState::Invalid,
    stApp = CbsInstallState::Invalid;

  CHECK(pPkg->EvaluateApplicability(0, &stApp, &stIns), "Failed to check the state of the pkg.");
  auto strIns = GetEnumName(stIns),
    strApp = GetEnumName(stApp);

  _tprintf(L"Name: %s\n", szName.get());
  _tprintf(L"Description: %s\n", szDesc.get());
  _tprintf(L"Identity: %s\n", szIdent.get());
  _tprintf(L"Release Type: %s\n", szReType.get());
  _tprintf(L"Pkg Size: %s Bytes\n", szSize.get());
  _tprintf(L"Support URL: %s\n", szSup.get());

  _tprintf(L"Install State: %S\n", strIns.c_str());
  _tprintf(L"Applicable State: %S\n", strApp.c_str());

  if (bIns) {
    InsertLine(LineSizeMedium);
    GPkgProp(InstallTimeStamp, InsTime);
    auto tm = WindowsTimeStamp2SystemTime(STR_TO_NUM_HEX(szInsTime.get()));
    _tprintf(L"Install time: %04hu/%02hu/%02hu %02hu:%02hu:%02hu\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
  }

  InsertLine(LineSizeLong);

  return S_OK;
#undef GPkgProp
}

HRESULT PrintUpdateInfo(const ComPtr<ICbsUpdate>& pUpd)
{
  BEGIN_ERROR_HANDLING();

  if (!pUpd) RET_HR_LOG(E_INVALIDARG, "The pointer to the upd is invalid.");

#define GUpdProp(x, y) \
unique_malloc_ptr<wchar_t> sz##y; \
{ \
  PWSTR rawsz##y; \
  CHECK(pUpd->GetProperty(CbsUpdateProperty::x, &(rawsz##y)), "OK") \
  (sz##y).reset(rawsz##y);\
}

  InsertLine(LineSizeLong);
  PWSTR szParent, szSet;
  hr = pUpd->GetParentUpdate(0, &szParent, &szSet);
  const int CBS_E_ARRAY_ELEMENT_MISSING = -2146498551;
  const bool hasParent = hr != CBS_E_ARRAY_ELEMENT_MISSING;

  unique_malloc_ptr<wchar_t> uszParent, uszSet;
  if (hasParent) {
    uszParent.reset(szParent);
    uszSet.reset(szSet);
  }
  szParent = szSet = nullptr;

  GUpdProp(DisplayName, Name);
  GUpdProp(Description, Desc);
  GUpdProp(Name, RawName);
  GUpdProp(DisplayFile, File);
  GUpdProp(DownloadSize, DlSize);

  _tprintf(L"Display Name: %s\n", szName.get());
  _tprintf(L"Description: %s\n", szDesc.get());
  _tprintf(L"Raw Name: %s\n", szRawName.get());
  _tprintf(L"Display File: %s\n", szFile.get());
  _tprintf(L"Download Size: %s Bytes\n", szDlSize.get());
  {
    CbsInstallState installState, intendedState, requestedState;
    CHECK(pUpd->GetInstallState(&installState, &intendedState, &requestedState),
      "Failed to get update install state. [Update = %S]", szRawName.get());
    _tprintf(L"Install State: %S %S %S\n",
      GetEnumName(installState).c_str(),
      GetEnumName(intendedState).c_str(),
      GetEnumName(requestedState).c_str());
  }
  if (hasParent)
    _tprintf(L"Parent Upd: %s; \tSet: %s\n", uszParent.get(), uszSet.get());

  InsertLine(LineSizeLong);

  return S_OK;
#undef GUpdProp
}

// ==============================
// Permission
// ==============================
HRESULT GetServiceToken(const std::wstring& name, HANDLE& handle) {
  SERVICE_STATUS_PROCESS status;
  DWORD size = 0, cnt = 0;

  SC_HANDLE hService, hSCM = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
  if (!hSCM) RET_LASTERR_LOG("Failed to Open SCManager.");
  hService = OpenService(hSCM, name.c_str(), MAXIMUM_ALLOWED);
  if (!hService) RET_LASTERR_LOG("Failed to OpenService %S.", name.c_str());

  bool flgRunning = false;

  while (QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof status, &size))
  {
    if (status.dwCurrentState == SERVICE_RUNNING)
    {
      LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Queried status: Running.");
      flgRunning = true;
      break;
    }
    else if (status.dwCurrentState == SERVICE_STOPPED)
    {
      LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Queried status: Stopped, retrying...");
      if (cnt >= 5) RET_WIN32ERR_LOG(ERROR_TIMEOUT, "Too many retry times.");
      if (!(cnt++, StartService(hService, 0, nullptr))) RET_LASTERR_LOG("Failed to StartService %S.", name.c_str());
    }
    else if (status.dwCurrentState == SERVICE_START_PENDING
      || status.dwCurrentState == SERVICE_STOP_PENDING)
    {
      LogA(S_OK, WdsLogSourceUI, WdsLogLevelInfo, "Queried status: Pending, to sleep 0.25s.");
      SleepEx(250, FALSE);
    }
  }

  if (!flgRunning)
    RET_WIN32ERR_LOG(ERROR_SERVICE_NOT_ACTIVE, "Failed to StartService %S within 5 attempts.", name.c_str());

  CloseServiceHandle(hService);
  CloseServiceHandle(hSCM);

  HANDLE hProc = OpenProcess(MAXIMUM_ALLOWED, FALSE, status.dwProcessId);
  if (!hProc) RET_LASTERR_LOG("Failed to OpenServiceProcess [ProcessId = %lu].", status.dwProcessId);
  handle = 0;

  if (!OpenProcessToken(hProc, MAXIMUM_ALLOWED, &handle))
    RET_LASTERR_LOG("Failed to Open Service Process Token [ProcessId = %lu].", status.dwProcessId);
  CloseHandle(hProc);

  return S_OK;
}

HRESULT GetWinLogonToken(HANDLE& handle) {
  PWTS_PROCESS_INFOW pProcesses;
  DWORD dwProcessCount = 0, SessionId = 0;
  if (!ProcessIdToSessionId(GetCurrentProcessId(), &SessionId))
    RET_LASTERR_LOG("Failed to get current session id.");

  if (WTSEnumerateProcessesW(
    WTS_CURRENT_SERVER_HANDLE,
    0, 1,
    &pProcesses,
    &dwProcessCount))
  {
    for (DWORD i = 0; i < dwProcessCount; ++i)
    {
      PWTS_PROCESS_INFOW pProcess = &pProcesses[i];

      if (pProcess->SessionId != SessionId)
        continue;

      if (!pProcess->pProcessName)
        continue;

      if (_wcsicmp(L"winlogon.exe", pProcess->pProcessName) != 0)
        continue;

      if (!pProcess->pUserSid)
        continue;

      if (!IsWellKnownSid(
        pProcess->pUserSid, WELL_KNOWN_SID_TYPE::WinLocalSystemSid))
        continue;

      auto hProc = OpenProcess(MAXIMUM_ALLOWED, FALSE, pProcess->ProcessId);

      if (!hProc) RET_LASTERR_LOG("Failed to OpenProcess for winlogon process.");

      if (!OpenProcessToken(hProc, MAXIMUM_ALLOWED, &handle))
        RET_LASTERR_LOG("Failed to open the token of winlogon process.");

      CloseHandle(hProc);

      return S_OK;
    }
  }

  return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
}

HRESULT ImpersonateSys() {
  BEGIN_ERROR_HANDLING();

  HANDLE hSysToken = 0, hNewToken = 0;
  CHECK(GetWinLogonToken(hSysToken), "Failed to get the token of winlogon.exe .");
  if (!DuplicateTokenEx(hSysToken, MAXIMUM_ALLOWED, nullptr, SecurityImpersonation, TokenImpersonation, &hNewToken))
    RET_LASTERR_LOG("Failed to DuplicateTokenEx the token of winlogon.exe(SYSTEM User).");

  PTOKEN_PRIVILEGES pPrivs = nullptr;
  DWORD neededSize = 0;
  GetTokenInformation(hNewToken, TokenPrivileges, nullptr, 0, &neededSize);
  if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && neededSize > 0) {
    pPrivs = (PTOKEN_PRIVILEGES)HeapAlloc(GetProcessHeap(), 0, neededSize);
    if (!GetTokenInformation(hNewToken, TokenPrivileges, pPrivs, neededSize, &neededSize))
      RET_LASTERR_LOG("Failed to GetTokenPrivileges with the token of System User.");
  }
  else RET_LASTERR_LOG("Failed to Pre-get the buffer size of the privilege information of the duplicated System token.");

  for (size_t idx = 0; idx < pPrivs->PrivilegeCount; idx++)
    pPrivs->Privileges[idx].Attributes = SE_PRIVILEGE_ENABLED;

  if (!AdjustTokenPrivileges(hNewToken, FALSE, pPrivs, 0, nullptr, nullptr))
    RET_LASTERR_LOG("Failed to AdjustTokenPrivileges the new token.");

  if (!HeapFree(GetProcessHeap(), 0, pPrivs))
    RET_LASTERR_LOG("Failed to Free the buffer pPrivs.");

  if (!SetThreadToken(nullptr, hNewToken))
    RET_LASTERR_LOG("Failed to impersonate System User with SetThreadToken function.");

  CloseHandle(hSysToken);
  CloseHandle(hNewToken);

  return S_OK;
}

DWORD CreateProcessAs(
  HANDLE hToken,
  LPCWSTR lpCommandLine,
  LPCWSTR lpCurrentDirectory,
  DWORD WaitInterval,
  DWORD ProcessPriority = 0,
  DWORD ShowWindowMode = SW_SHOWDEFAULT,
  bool CreateNewConsole = true)
{
  DWORD dwCreationFlags = CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT;

  if (CreateNewConsole)
  {
    dwCreationFlags |= CREATE_NEW_CONSOLE;
  }

  STARTUPINFOW StartupInfo = { 0 };
  PROCESS_INFORMATION ProcessInfo = { 0 };

  StartupInfo.cb = sizeof(STARTUPINFOW);

  StartupInfo.lpDesktop = const_cast<LPWSTR>(L"WinSta0\\Default");

  StartupInfo.dwFlags |= STARTF_USESHOWWINDOW;
  StartupInfo.wShowWindow = static_cast<WORD>(ShowWindowMode);

  LPVOID lpEnvironment = nullptr;

  BOOL result = FALSE;
  DWORD lastError = 0;

  HANDLE hCurToken;
  if (!OpenProcessToken(GetCurrentProcess(), MAXIMUM_ALLOWED, &hCurToken))
    RET_LASTERR_LOG("Failed to read the token of the current process.");

  if (CreateEnvironmentBlock(&lpEnvironment, hCurToken, TRUE))
  {
    std::wstring ExpandedString = ExpandEnvW(lpCommandLine);

    result = CreateProcessAsUserW(
      hToken,
      nullptr,
      const_cast<LPWSTR>(ExpandedString.c_str()),
      nullptr,
      nullptr,
      FALSE,
      dwCreationFlags,
      lpEnvironment,
      lpCurrentDirectory,
      &StartupInfo,
      &ProcessInfo);

    if (result)
    {
      SetPriorityClass(ProcessInfo.hProcess, ProcessPriority);

      ResumeThread(ProcessInfo.hThread);

      WaitForSingleObjectEx(
        ProcessInfo.hProcess, WaitInterval, FALSE);

      CloseHandle(ProcessInfo.hProcess);
      CloseHandle(ProcessInfo.hThread);
    }
    else lastError = GetLastError();

    DestroyEnvironmentBlock(lpEnvironment);
  }

  CloseHandle(hCurToken);

  return lastError;
}

HRESULT SudoTI(const std::wstring& exec)
{
  BEGIN_ERROR_HANDLING();

  CHECK(ImpersonateSys(), "Failed to impersonate System user!");

  HANDLE hTIToken = 0, hNewToken = 0;
  CHECK(GetServiceToken(_T("TrustedInstaller"), hTIToken), "Failed to GetTIServiceToken.");
  if (!DuplicateTokenEx(hTIToken, MAXIMUM_ALLOWED, nullptr, SecurityIdentification, TokenPrimary, &hNewToken))
    RET_LASTERR_LOG("Failed to DuplicateTokenEx the token of TI.");
  CloseHandle(hTIToken);

  DWORD dwSessionId;
  if (!ProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId))
    RET_LASTERR_LOG("Failed to get the session id of the current process.");
  if (!SetTokenInformation(hNewToken, TokenSessionId, &dwSessionId, sizeof DWORD))
    RET_LASTERR_LOG("Failed to set TokenSessionId for the duplicated token of TI.");

  WCHAR strBuffer[256];
  GetCurrentDirectory(256, strBuffer);
  if (CreateProcessAs(hNewToken, L"C:\\Windows\\System32\\cmd.exe", strBuffer, 0))
    RET_LASTERR_LOG("Failed to CreateProcess CMD as TI.");
  if (CreateProcessAs(hNewToken, exec.c_str(), strBuffer, 0))
    RET_LASTERR_LOG("Failed to CreateProcessAs TI.");

  CloseHandle(hNewToken);

  return S_OK;
}

HRESULT CheckSudoSelf()
{
  BEGIN_ERROR_HANDLING();

  HANDLE hCurToken = 0;
  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hCurToken))
    RET_LASTERR_LOG("Failed to read the token of the current process.");

  PTOKEN_GROUPS pGroups = nullptr;
  DWORD neededSize;
  GetTokenInformation(hCurToken, TokenGroups, nullptr, 0, &neededSize);
  if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && neededSize > 0)
  {
    pGroups = (PTOKEN_GROUPS)HeapAlloc(GetProcessHeap(), 0, neededSize);
    if (!GetTokenInformation(hCurToken, TokenGroups, pGroups, neededSize, &neededSize))
      RET_LASTERR_LOG("Failed to GetTokenInformation for the current process token.");
  }
  else RET_LASTERR_LOG("Failed to Pre-get the buffer size of the group information of the current process token.");

  PSID pSidTI = nullptr;
  if (!ConvertStringSidToSid(SID_TI, &pSidTI))
    RET_LASTERR_LOG("Failed to Create TI Sid.");

  BOOL bIsTI = FALSE;
  for (size_t idx = 0; idx < pGroups->GroupCount; idx++)
    bIsTI |= EqualSid(pSidTI, pGroups->Groups[idx].Sid);

  if (!HeapFree(GetProcessHeap(), 0, pGroups))
    RET_LASTERR_LOG("Failed to free the buffer pGroups.");
  pGroups = nullptr;

  FreeSid(pSidTI);
  CloseHandle(hCurToken);

  // ================================
  // if not TI, restart self with TI!
  // ================================
  if (!bIsTI) {
    WCHAR strExe[256];
    neededSize = 256;

    if (!QueryFullProcessImageName(GetCurrentProcess(), 0, strExe, &neededSize))
      RET_LASTERR_LOG("Failed to Query the full exe path.");

    CHECK(SudoTI(strExe), "Failed to SudoTI.");

    return S_ASYNCHRONOUS;
  } else {
    // already TI, waiting for debugger
    //std::wcout << L"Waiting for debugger attaching, press any key to continue...\n";
    //getchar();
  }


  return S_OK;
}
