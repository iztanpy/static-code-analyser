#include <set>
#include <stack>
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

bool QueryUtil::IsEnclosedInBrackets(const std::string& s) {
  return (s.size() >= 2 && s.front() == '(' && s.back() == ')');
}

bool QueryUtil::IsEnclosedInTuple(const std::string& s) {
  return (s.size() >= 2 && s.front() == '<' && s.back() == '>');
}

bool QueryUtil::IsSelectBoolean(const std::string & s, const std::vector<Declaration>& declarations) {
  return (s == "BOOLEAN" && !IsInDeclarations(s, declarations));
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

std::string QueryUtil::RemoveBrackets(const std::string& s) {
  return string_util::Trim(s.substr(1, s.length() - 2));
}

std::string QueryUtil::RemoveTuple(const std::string& s) {
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

bool QueryUtil::IsSynIf(const std::string& s, const std::vector<Declaration>& declarations) {
  if (IsSynonym(s)) {
    for (const Declaration& declaration : declarations) {
      if (declaration.synonym == s && declaration.design_entity == DesignEntity::IF_STMT) {
        return true;
      }
    }
  }
  return false;
}

bool QueryUtil::IsSynWhile(const std::string& s, const std::vector<Declaration>& declarations) {
  if (IsSynonym(s)) {
    for (const Declaration& declaration : declarations) {
      if (declaration.synonym == s && declaration.design_entity == DesignEntity::WHILE_LOOP) {
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

bool QueryUtil::IsOperator(const char & c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c =='%';
}

int QueryUtil::getPrecedence(char op) {
  if (op == '+' || op == '-') {
    return 1;
  } else if (op == '*' || op == '/' || op == '%') {
    return 2;
  }
  return 0;  // Default for non-operators
}

bool QueryUtil::hasPrecedence(char op1, char op2) {
  int prec_1 = getPrecedence(op1);
  int prec_2 = getPrecedence(op2);
  if (prec_1 == prec_2) {
    return true;  // Left-associative operators have the same precedence
  }
  return prec_1 < prec_2;
}

void QueryUtil::processOperator(std::stack<char>& operators, std::stack<std::string>& operands) {
  char op = operators.top();
  operators.pop();
  std::string rightOperand = operands.top();
  operands.pop();
  std::string leftOperand = operands.top();
  operands.pop();
  operands.push("(" + leftOperand + op + rightOperand + ")");
}

std::string bracket_variables(char c) {
  std::string result;
  result += "(";
  result += std::string(1, c);
  result += ")";
  return result;
}

std::string bracket_variables(std::string s) {
  std::string result;
  result += "(";
  result += s;
  result += ")";
  return result;
}

std::string QueryUtil::addParentheses(const std::string& expression) {
  std::string input = expression;
  std::stack<char> operators;
  std::stack<std::string> operands;
  std::stack<char> brackets;
  std::vector<std::string> output;
  std::string currentStr;

  for (char c : input) {
    if (isalnum(c)) {
      currentStr += c;
    } else {
      if (!currentStr.empty()) {
        operands.push(bracket_variables(currentStr));
        currentStr.clear();
      }
      if (c == '(') {
        operators.push(c);
      } else if (c == ')') {
        while (!operators.empty() && operators.top() != '(') {
          processOperator(operators, operands);
        }
        operators.pop();  // Pop the opening parenthesis
      } else if (IsOperator(c)) {
        while (!operators.empty() && hasPrecedence(c, operators.top())) {
          processOperator(operators, operands);
        }
        operators.push(c);
      }
    }
  }
  if (!currentStr.empty()) {
    operands.push(bracket_variables(currentStr));
    currentStr.clear();
  }

  while (!operators.empty()) {
    processOperator(operators, operands);
  }

  return operands.top();
}

bool QueryUtil::IsAttrName(std::string & s) {
  std::set<std::string> attr_names = {
      "procName",
      "varName",
      "value",
      "stmt#"
  };
  // If string is not in attr_names
  if (attr_names.find(s) == attr_names.end()) {
    return false;
  }
  return true;
}

bool QueryUtil::IsAttrRef(std::string & s) {
  std::vector<std::string> split_s = string_util::SplitStringBy('.', s);
  if (split_s.size() != 2) {
    // If more than two '.' or does not contain '.'
    return false;
  }
  return IsSynonym(split_s[0]) && IsAttrName(split_s[1]);
}

bool QueryUtil::IsRef(std::string & s) {
  return IsIdentWithDoubleQuotes(s) || lexical_utils::IsInteger(s) || IsAttrRef(s);
}
