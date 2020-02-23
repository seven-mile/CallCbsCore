#pragma once

class CCbsConfig
{
	std::wstring m_logfile;
	bool m_bOutputLog = false;
public:
	CCbsConfig() = default;
	void SetLogFile(const std::wstring& val);
	std::wstring GetLogFile();
	void SetLogOutput(bool val);
	bool GetLogOutput();
};

extern CCbsConfig g_conf;
