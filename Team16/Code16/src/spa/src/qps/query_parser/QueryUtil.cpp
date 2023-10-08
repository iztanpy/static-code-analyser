#include <set>
#include <string>
#include "QueryUtil.h"
#include "qps/constants.h"
#include "utils/lexical_utils.h"
#include "qps/design_entity.h"
#include "qps/rel_ref.h"
#include "utils/string_utils.h"

bool QueryUtil::IsWildcard(const std::string& s) {
  return s[0] == qps_constants::kWildcard && s.length() == 1;
}

bool QueryUtil::IsSynonym(const std::string& s) {
  return lexical_utils::IsIdent(s);
}

bool QueryUtil::IsStmtRef(const std::string& s) {
  return (IsSynonym(s) || IsWildcard(s) || lexical_utils::IsInteger(s));
}

bool QueryUtil::IsEntRef(const std::string& s) {
  return (IsSynonym(s) || IsWildcard(s) || IsIdentWithDoubleQuotes(s));
}

bool QueryUtil::IsDesignEntity(const std::string& s) {
  std::set<std::string> stringDesignEntities = Entity::getStringDesignEntities();
  if (stringDesignEntities.find(s) != stringDesignEntities.end()) {
    return true;
  }
  return false;
}

bool QueryUtil::IsEnclosedInDoubleQuotes(const std::string& s) {
  return (s.size() >= 2 && s.front() == '\"' && s.back() == '\"');
}

bool QueryUtil::IsEnclosedInWildcard(const std::string& s) {
  return (s.size() >= 2 && s.front() == '_' && s.back() == '_');
}

bool QueryUtil::IsRelRef(const std::string& s) {
  std::set<std::string> stringRelRef = RelRef::getStringRelRef();
  if (stringRelRef.find(s) != stringRelRef.end()) {
    return true;
  }
  return false;
}

bool QueryUtil::IsExprSpec(const std::string & s) {
  return IsPartialMatchExpressionSpecification(s) || IsExactExpressionSpecification(s) || IsWildcard(s);
}

bool QueryUtil::IsExactExpressionSpecification(const std::string & s) {
  if (!IsEnclosedInDoubleQuotes(s)) {
    return false;
  }
  std::string processed_s = RemoveQuotations(s);
  return lexical_utils::IsExpr(processed_s);
}

bool QueryUtil::IsPartialMatchExpressionSpecification(const std::string& s) {
  if (!IsEnclosedInWildcard(s)) {
    return false;
  }
  std::string processed_s = RemovePartialMatch(s);
  return IsExactExpressionSpecification(processed_s);
}

std::string QueryUtil::RemoveQuotations(const std::string& s) {
  return string_util::Trim(s.substr(1, s.length() - 2));
}

std::string QueryUtil::RemovePartialMatch(const std::string& s) {
  return string_util::Trim(s.substr(1, s.length() - 2));
}

bool QueryUtil::IsIdentWithDoubleQuotes(const std::string& s) {
  return (s.size() >= 2 && s.front() == '\"' && s.back() == '\"'
      && lexical_utils::IsIdent(s.substr(1, s.length() - 2)));
}

bool QueryUtil::IsSynAssign(const std::string& s, const std::vector<Declaration>& declarations) {
  if (IsSynonym(s)) {
    for (const Declaration& declaration : declarations) {
      if (declaration.synonym == s && declaration.design_entity == DesignEntity::ASSIGN) {
        return true;
      }
    }
  }
  return false;
}

bool QueryUtil::IsInDeclarations(const std::string& s, const std::vector<Declaration>& declarations) {
  for (const Declaration& declaration : declarations) {
    if (declaration.synonym == s) {
      return true;
    }
  }
  return false;
}
