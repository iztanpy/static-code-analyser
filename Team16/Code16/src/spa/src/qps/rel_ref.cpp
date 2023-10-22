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
      {RelRefType::CALLS, "Calls"},
      {RelRefType::CALLST, "Calls*"},
      {RelRefType::NEXT, "Next"},
      {RelRefType::NEXTT, "Next*"},
      {RelRefType::AFFECTS, "Affects"}
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
      {"Calls", RelRefType::CALLS},
      {"Calls*", RelRefType::CALLST},
      {"Next", RelRefType::NEXT},
      {"Next*", RelRefType::NEXTT},
      {"Affects", RelRefType::AFFECTS}
  };

  return string_to_relRef.at(relRef_str);
}

std::set<std::string> RelRef::getStringRelRef() {
  return {"Follows", "Follows*", "Parent", "Parent*", "Modifies", "Uses",
          "Calls", "Calls*", "Next", "Next*", "Affects"};
}

int RelRef::getClauseScore(RelRefType rel_ref, int num_synonym) {
  static const int kSynonymPenalty = 8;
  static const std::map<RelRefType, int> kClauseScore = {
      {RelRefType::NEXT, 1}, {RelRefType::FOLLOWS, 1},
      {RelRefType::CALLS, 2}, {RelRefType::CALLST, 2},
      {RelRefType::PARENT, 3},
      {RelRefType::FOLLOWST, 4}, {RelRefType::PARENTT, 4},
      {RelRefType::MODIFIESS, 5},
      {RelRefType::MODIFIESP, 6},
      {RelRefType::WHILE, 7}, {RelRefType::IF, 7},
      {RelRefType::ASSIGN, 8},
      {RelRefType::USESS, 9},
      {RelRefType::SELECT, 10},
      {RelRefType::USESP, 11},
      {RelRefType::NEXTT, 20},
      {RelRefType::AFFECTS, 25},
  };

  assert((num_synonym == 1 || num_synonym == 2) && "[num_synonym] can only be 1 or 2");
  return kClauseScore.at(rel_ref) + (num_synonym == 2 ? kSynonymPenalty : 0);
}
