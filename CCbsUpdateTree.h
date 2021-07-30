#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class CCbsUpdateTree
{
  uint32_t cntUpdates;
  std::unordered_map<std::wstring, std::wstring> m_mapFa;
  std::unordered_map<std::wstring, std::vector<std::wstring>> m_mapArrDes;
public:
  CCbsUpdateTree();

  // delete other for global identity
  CCbsUpdateTree(const CCbsUpdateTree&) = delete;
  CCbsUpdateTree(CCbsUpdateTree&&) = delete;

  void AddDependency(const std::wstring &szParentUpdate, const std::wstring &szSonUpdate);
  const std::wstring &GetFather(const std::wstring &szUpdateName) const;
  const std::vector<std::wstring> &GetDescendants(const std::wstring &szUpdateName) const;
  const std::wstring &GetRoot() const;
  void ClearTree();
};

