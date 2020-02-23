#include "pch.h"
#include "CbsConf.h"

void CCbsConfig::SetLogFile(const std::wstring &val)
{
	m_logfile = val;
	SetEnvironmentVariable(_T("COMPONENT_BASED_SERVICING_LOGFILE"), val.c_str());
}

std::wstring CCbsConfig::GetLogFile()
{
	return m_logfile;
}

void CCbsConfig::SetLogOutput(bool val)
{
	m_bOutputLog = val;
}

bool CCbsConfig::GetLogOutput()
{
	return m_bOutputLog;
}

// extern
CCbsConfig g_conf;
