#include "pch.h"
#include "CbsConf.h"

void CCbsConfig::SetLogFile(const std::wstring& val)
{
  m_logfile = val;
  SetEnvironmentVariable(_T("COMPONENT_BASED_SERVICING_LOGFILE"), val.c_str());
}

// extern
CCbsConfig g_conf;
