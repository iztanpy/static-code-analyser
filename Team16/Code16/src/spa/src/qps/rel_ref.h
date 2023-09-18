#pragma once

#include <string>
#include <set>

enum class RelRefType {
  FOLLOWS,
  FOLLOWST,
  PARENT,
  PARENTT,
  USESS,
  USESP,
  MODIFIESS,
  MODIFIESP
};

class RelRef {
 public:
  static std::string toString(RelRefType relRef);
  static RelRefType fromString(const std::string & relRef_str);
  static std::set<std::string> getStringRelRef();
};
