#include "pch.h"
#include "CCbsUpdateTree.h"

#include <stdexcept>
#include <format>


CCbsUpdateTree::CCbsUpdateTree()
{
  ClearTree();
}

void CCbsUpdateTree::AddDependency(const std::wstring& szParentUpdate, const std::wstring& szSonUpdate)
{
  if (m_mapFa.count(szSonUpdate)) {
    LogA(ERROR_BAD_ARGUMENTS, WdsLogSourceTOOL, WdsLogLevelWarning,
      "The update \"%S\" has two or more parents [\"%S\", \"%S\"], the second will be ignored.",
      szSonUpdate.c_str(), m_mapFa[szSonUpdate].c_str(), szParentUpdate.c_str());
    return;
    //throw std::logic_error(std::format("multiple dependency for son node \"{}\" !", WSTR_TO_STR(szSonUpdate).c_str()));
  }
  m_mapFa[szSonUpdate] = szParentUpdate;
  m_mapArrDes[szParentUpdate].push_back(szSonUpdate);
  cntUpdates++;
}

const std::wstring& CCbsUpdateTree::GetFather(const std::wstring& szUpdateName) const
{
  static std::wstring nullResult = L"(null)";
  if (auto it = m_mapFa.find(szUpdateName); it != m_mapFa.end()) {
    return it->second;
  } else {
    return nullResult;
  }
}

const std::vector<std::wstring>& CCbsUpdateTree::GetDescendants(const std::wstring& szUpdateName) const
{
  static std::vector<std::wstring> nullResult = {};
  if (auto it = m_mapArrDes.find(szUpdateName); it != m_mapArrDes.end()) {
    return it->second;
  } else {
    return nullResult;
  }
}

const std::wstring& CCbsUpdateTree::GetRoot() const
{
  static std::wstring strRootName = L"#ROOT#";
  return strRootName;
}

void CCbsUpdateTree::ClearTree()
{
  m_mapArrDes.clear();
  m_mapFa.clear();
  cntUpdates = 1u;
  m_mapArrDes[GetRoot()] = {};
}
