#pragma once

class CCbsConfig
{
  std::wstring m_logfile = L"";
public:
  bool output_log = false;
  std::wstring client_name = L"CallCbsCore";

  enum class SessMode {
    Online,
    Offline // arg_bootdrive should be set.
  } mode = SessMode::Online;
  std::wstring arg_bootdrive = L"";
  enum class StackSource {
    Registry, // online
    FolderPath, // offline, arg_path should be set as the folder.
    SSShim // offline, arg_path should be set as the ssshim dll path.
  } stack_source = StackSource::Registry;
  std::wstring arg_path = L"";

  void SetLogFile(const std::wstring& val);
  [[nodiscard]] auto GetLogFile() const { return m_logfile; }

  ~CCbsConfig() { }
};

extern CCbsConfig g_conf;
