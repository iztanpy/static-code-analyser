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
      {RelRefType::WITH, 1},
      {RelRefType::NEXT, 2}, {RelRefType::FOLLOWS, 2},
      {RelRefType::CALLS, 3}, {RelRefType::CALLST, 3},
      {RelRefType::PARENT, 4},
      {RelRefType::FOLLOWST, 5}, {RelRefType::PARENTT, 5},
      {RelRefType::MODIFIESS, 6},
      {RelRefType::MODIFIESP, 7},
      {RelRefType::WHILE, 8}, {RelRefType::IF, 8},
      {RelRefType::ASSIGN, 9},
      {RelRefType::USESS, 10},
      {RelRefType::SELECT, 11},
      {RelRefType::USESP, 12},
      {RelRefType::NEXTT, 21},
      {RelRefType::AFFECTS, 25},
  };

  assert((num_synonym == 1 || num_synonym == 2) && "[num_synonym] can only be 1 or 2");
  return kClauseScore.at(rel_ref) + (num_synonym == 2 ? kSynonymPenalty : 0);
}
