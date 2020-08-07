#pragma once

class CCbsConfig
{
  std::wstring m_logfile;
public:
  bool output_log = false;
  std::wstring strClientName = L"CallCbsCore";

  enum class SessMode {
    Online,
    Offline // arg_bootdrive should be set.
  } mode;
  std::wstring arg_bootdrive;
  enum class StackSource {
    Registry, // online
    FolderPath, // offline, arg_path should be set as the folder.
    SSShim // offline, arg_path should be set as the ssshim dll path.
  } stack_source;
  std::wstring arg_path;

  CCbsConfig() = default;

  void SetLogFile(const std::wstring& val);
  auto GetLogFile() { return m_logfile; }
};

extern CCbsConfig g_conf;
