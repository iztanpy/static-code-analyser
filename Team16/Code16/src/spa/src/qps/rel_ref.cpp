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
  static const int kSynonymPenalty = 1;
  static const std::map<RelRefType, int> kClauseScore = {
      // SelectClause
      {RelRefType::SELECT, 4},

      // SuchThatClause
      {RelRefType::FOLLOWS, 4},
      {RelRefType::FOLLOWST, 4},
      {RelRefType::MODIFIESS, 4},
      {RelRefType::MODIFIESP, 4},
      {RelRefType::PARENTT, 4},
      {RelRefType::PARENT, 4},
      {RelRefType::USESS, 4},
      {RelRefType::USESP, 4},
      {RelRefType::CALLS, 4},
      {RelRefType::CALLST, 4},
      {RelRefType::NEXT, 4},
      {RelRefType::NEXTT, 4},
      {RelRefType::AFFECTS, 4},

      // PatternClause
      {RelRefType::ASSIGN, 4},
      {RelRefType::WHILE, 4},
      {RelRefType::IF, 4}
  };

  assert((num_synonym == 1 || num_synonym == 2) && "[num_synonym] can only be 1 or 2");
  return kClauseScore.at(rel_ref) + (num_synonym == 2 ? kSynonymPenalty : 0);
}
