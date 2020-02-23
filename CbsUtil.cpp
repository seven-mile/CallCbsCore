#include "pch.h"

HRESULT WdsLogA(HRESULT hr, WdsLogSource source, WdsLogLevel level, const char* fmt, ...)
{
	va_list va;

	if (!vpfnWdsSetupLogMessageA || g_conf.GetLogOutput())
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
		
		return HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION);
	}

	va_start(va, fmt);

	return vpfnWdsSetupLogMessageA(
		vpfnConstructPartialMsgVA(level, fmt, va),
		source, "D", 0, 1151, __FILE__, __FUNCTION__,
		vpfnCurrentIP(), NULL, NULL, NULL);
}

HRESULT WdsLogHrInternalA(HRESULT hr, WdsLogSource source, WdsLogLevel level, const char* fmt, ...)
{
	va_list va;

	fmt = CONCAT_STR<CHAR>(fmt, " HRESULT - 0x%x - %s");

	if (!vpfnWdsSetupLogMessageA || g_conf.GetLogOutput())
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

		return HRESULT_FROM_WIN32(ERROR_INVALID_FUNCTION);
	}

	va_start(va, fmt);

	return vpfnWdsSetupLogMessageA(
		vpfnConstructPartialMsgVA(level, fmt, va),
		source, "D", 0, 1151, __FILE__, __FUNCTION__,
		vpfnCurrentIP(), NULL, NULL, NULL);
}

SYSTEMTIME WindowsTimeStamp2SystemTime(const UINT64 &timeStamp)
{
	LARGE_INTEGER liTmp;
	liTmp.QuadPart = timeStamp;
	FILETIME fileTimeTmp;
	fileTimeTmp.dwLowDateTime = liTmp.LowPart;
	fileTimeTmp.dwHighDateTime = liTmp.HighPart;
	SYSTEMTIME sysTimeTmp;
	FileTimeToSystemTime(&fileTimeTmp, &sysTimeTmp);

	return sysTimeTmp;
}

std::wstring ExpandEnvW(const std::wstring &str)
{
	std::wstring strBuf;
	strBuf.resize(255);
	ExpandEnvironmentStringsW(str.c_str(), &strBuf[0], 255);
	strBuf.resize(wcslen(strBuf.c_str()));
	return strBuf;
}

HRESULT TextizeCbsInstallState(_CbsInstallState &st, LPTSTR* ret)
{
	*ret = NULL;
	if (st == CbsInstallStatePartiallyInstalled) *ret = (LPTSTR)_T("Partially Installed");
	if (st == CbsInstallStateCancel) *ret = (LPTSTR)_T("Cancel");
	if (st == CbsInstallStateSuperseded) *ret = (LPTSTR)_T("Superseded");
	if (st == CbsInstallStateDefault) *ret = (LPTSTR)_T("Default");
	if (st == CbsInvalidStatePermanent) *ret = (LPTSTR)_T("Invalid Permanent");
	if (st == CbsInvalidStateInstalled) *ret = (LPTSTR)_T("Invalid Installed");
	if (st == CbsInvalidStateStaged) *ret = (LPTSTR)_T("Invalid Staged");
	if (st == CbsInvalidStateResolved) *ret = (LPTSTR)_T("Invalid Resolved");
	if (st == CbsInstallStateUnknown) *ret = (LPTSTR)_T("Unknown");

	if (st == CbsInstallStateAbsent) *ret = (LPTSTR)_T("Absent");
	if (st == CbsInstallStateResolving) *ret = (LPTSTR)_T("Resolving");
	if (st == CbsInstallStateResolved) *ret = (LPTSTR)_T("Resolved");
	if (st == CbsInstallStateStaging) *ret = (LPTSTR)_T("Staging");
	if (st == CbsInstallStateStaged) *ret = (LPTSTR)_T("Staged");
	if (st == CbsInstallStateUninstallRequested) *ret = (LPTSTR)_T("Uninstall Requested");
	if (st == CbsInstallStateInstallRequested) *ret = (LPTSTR)_T("Install Requested");
	if (st == CbsInstallStateInstalled) *ret = (LPTSTR)_T("Installed");
	if (st == CbsInstallStatePermanent) *ret = (LPTSTR)_T("Permanent");

	return *ret ? S_OK : E_INVALIDARG;
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
							return _T("Unknown Error");
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
					return _T("Unknown Error");
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
				return _T("Unknown Error");
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
					return _T("Unknown Error");
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
					return _T("Unknown Error");
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
					return _T("Unknown Error");
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
					return _T("Unknown Error");
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
					return _T("Unknown Error");
				}
			}
		}
	}
	return result;
}

LPCTSTR TextizeHresult(HRESULT hr)
{
	auto result = _T("Unknown Error");

	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)& result, 0, NULL))
		result = TextizeSetupHresult(hr);

	return result;
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
							return "Unknown Error";
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
					return "Unknown Error";
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
				return "Unknown Error";
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
					return "Unknown Error";
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
					return "Unknown Error";
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
					return "Unknown Error";
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
					return "Unknown Error";
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
					return "Unknown Error";
				}
			}
		}
	}
	return result;
}

LPCSTR TextizeHresultA(HRESULT hr)
{
	auto result = "Unknown Error";

	if (!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR)& result, 0, NULL))
		result = TextizeSetupHresultA(hr);
	
	return result;
}

void InsertLine(UINT uSize)
{
	for (UINT iCh = 0; iCh < uSize; iCh++)
	{
		_puttchar(_T('='));
	}
	_puttchar(_T('\n'));
}

HRESULT PrintPackageInfo(ComPtr<ICbsPackage> pPkg, bool bIns)
{
	BEGIN_ERROR_HANDLING();

	if (!pPkg) RET_HR_LOG(E_INVALIDARG, "The pointer to the pkg is invalid.");

#define GPkgProp(x, y) CHECK(pPkg->GetProperty(CbsPackageProperty##x, &(y)), "OK")

	InsertLine(LineSizeLong);


	LPTSTR szName, szDesc, szIdent, szReType, szSize, szSup;
	GPkgProp(DisplayName, szName);
	GPkgProp(Description, szDesc);
	GPkgProp(IdentityString, szIdent);
	GPkgProp(ReleaseType, szReType);
	GPkgProp(PackageSize, szSize);
	GPkgProp(SupportInformation, szSup);

	_tprintf(L"Name: %s\n", szName);
	_tprintf(L"Description: %s\n", szDesc);
	_tprintf(L"Identity: %s\n", szIdent);
	_tprintf(L"Release Type: %s\n", szReType);
	_tprintf(L"Pkg Size: %s Bytes\n", szSize);
	_tprintf(L"Support URL: %s\n", szSup);

	if (bIns) {
		InsertLine(LineSizeMedium);
		LPTSTR szInsTime;
		GPkgProp(InstallTimeStamp, szInsTime);
		auto tm = WindowsTimeStamp2SystemTime(STR_TO_NUM(szInsTime));
		_tprintf(L"Install time: %04hu/%02hu/%02hu %02hu:%02hu:%02hu\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
	}

	InsertLine(LineSizeLong);

	return S_OK;
#undef GPkgProp
}
