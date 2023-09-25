#include <iostream>
#include <string>
#include <map>
#include "qps/rel_ref.h"

std::string RelRef::toString(RelRefType relRef) {
  static const std::map<RelRefType, std::string> refRef_to_string = {
      {RelRefType::FOLLOWS, "Follows"},
      {RelRefType::FOLLOWST, "Follows*"},
      {RelRefType::MODIFIESS, "Modifies"},
      {RelRefType::MODIFIESP, "Modifies"},
      {RelRefType::PARENTT, "Parent"},
      {RelRefType::PARENT, "Parent*"},
      {RelRefType::USESS, "Uses"},
      {RelRefType::USESP, "Uses"},
  };

  return refRef_to_string.at(relRef);
}

RelRefType RelRef::fromString(const std::string& relRef_str) {
  static const std::map<std::string, RelRefType> string_to_relRef = {
      {"Follows", RelRefType::FOLLOWS},
      {"Follows*", RelRefType::FOLLOWST},
      {"Parent", RelRefType::PARENT},
      {"Parent*", RelRefType::PARENTT},
      // Need to differentiate between modifies/ uses statement/ proc
      {"Modifies", RelRefType::MODIFIESS},
      {"Modifies", RelRefType::MODIFIESP},
      {"Uses", RelRefType::USESS},
      {"Uses", RelRefType::USESP},
  };

  return string_to_relRef.at(relRef_str);
}

std::set<std::string> RelRef::getStringRelRef() {
  return {"Follows", "Follows*", "Parent", "Parent*", "Modifies", "Uses"};
}
