#include "qps/query_parser/clause_builder/suchthat_clause_builder.h"

SuchThatClauseBuilder::SuchThatClauseBuilder() = default;

void SuchThatClauseBuilder::setRelRef(RelRefType relRef) {
  rel_ref = relRef;
}

void SuchThatClauseBuilder::setLhs(const QueryToken& param, const std::vector<Declaration>& declarations) {
  RefParam refParam;
  /*
   * Check if the rel ref is Follows, FollowsT, Next, NextT, Affects, Parent or ParentT
   * In these cases, LHS will all be StmtRef
   * Else, LHS could be either StmtRef or EntRef
   */
  switch (this->rel_ref) {
    case RelRefType::FOLLOWS:
    case RelRefType::FOLLOWST:
    case RelRefType::NEXT:
    case RelRefType::NEXTT:
    case RelRefType::AFFECTS:
    case RelRefType::PARENT:
    case RelRefType::PARENTT:lhs = getStmtRef(param, declarations);
      break;
    case RelRefType::CALLS:
    case RelRefType::CALLST:lhs = getEntRef(param, declarations);
      break;
    default:lhs = getBothStmtAndEntRef(param, declarations);
      break;
  }
}

void SuchThatClauseBuilder::setRhs(const QueryToken& param, const std::vector<Declaration>& declarations) {
  RefParam refParam;
  /*
   * Check if the rel ref is Follows, FollowsT, Next, NextT, Affects, Parent or ParentT
   * In these cases, RHS are all StmtRef
   * Else, RHS are all EntRef
   */
  switch (rel_ref) {
    case RelRefType::FOLLOWS:
    case RelRefType::FOLLOWST:
    case RelRefType::NEXT:
    case RelRefType::NEXTT:
    case RelRefType::AFFECTS:
    case RelRefType::PARENT:
    case RelRefType::PARENTT:rhs = getStmtRef(param, declarations);
      break;
    case RelRefType::CALLS:
    case RelRefType::CALLST:rhs = getEntRef(param, declarations);
      break;
    default:rhs = getEntRef(param, declarations);
      break;
  }
}

RefParam SuchThatClauseBuilder::getStmtRef(const QueryToken& param, const std::vector<Declaration>& declarations) {
  StmtRef refParam;
  int intValue;
  switch (param.type) {
    case PQLTokenType::SYNONYM:  // could be declaration, string or int
      for (const Declaration& declaration : declarations) {
        if (declaration.synonym == param.text) {
          refParam = declaration;
        }
      }
      break;
    case PQLTokenType::WILDCARD:refParam = Wildcard::Value;
      break;
    case PQLTokenType::INTEGER:intValue = std::stoi(param.text);
      refParam = intValue;
      break;
    default:
      // some other token type that does not belong in Such That clause
      throw QpsSyntaxError("Syntax error");
  }

  return refParam;
}

RefParam SuchThatClauseBuilder::getEntRef(const QueryToken& param, const std::vector<Declaration>& declarations) {
  EntRef refParam;
  switch (param.type) {
    case PQLTokenType::SYNONYM:  // could be declaration, string or int
      for (const Declaration& declaration : declarations) {
        if (declaration.synonym == param.text) {
          refParam = declaration;
        }
      }
      break;
    case PQLTokenType::WILDCARD:refParam = Wildcard::Value;
      break;
    case PQLTokenType::IDENT:refParam = param.text;
      break;
    default:
      // some other token type that does not belong in Such That clause
      throw QpsSyntaxError("Syntax error");
  }

  return refParam;
}

RefParam SuchThatClauseBuilder::getBothStmtAndEntRef(const QueryToken& param,
                                                     const std::vector<Declaration>& declarations) {
  StmtRef stmtRef;
  EntRef entRef;
  int intValue;
  switch (param.type) {
    case PQLTokenType::SYNONYM:  // technically we do not need to care if its stmt or ent
      for (const Declaration& declaration : declarations) {
        if (declaration.synonym == param.text) {
          stmtRef = declaration;
        }
      }
      return stmtRef;
    case PQLTokenType::WILDCARD:stmtRef = Wildcard::Value;
      return stmtRef;
    case PQLTokenType::INTEGER:intValue = std::stoi(param.text);
      stmtRef = intValue;
      return stmtRef;
    case PQLTokenType::IDENT:entRef = param.text;
      return entRef;
    default:
      // some other token type that does not belong in Such That clause
      throw QpsSyntaxError("Syntax error");
  }
}

std::unique_ptr<SuchThatClause> SuchThatClauseBuilder::getClause() const {
  switch (rel_ref) {
    case RelRefType::USESS:
    case RelRefType::USESP:
      // at this point we are not able to distinguish between UsesS and UsesP from the rel ref
      if (std::holds_alternative<StmtRef>(lhs) && std::holds_alternative<EntRef>(rhs)) {
        return std::make_unique<UsesS>(std::get<StmtRef>(lhs),
                                       std::get<EntRef>(rhs), is_not);
      } else if (std::holds_alternative<EntRef>(lhs) && std::holds_alternative<EntRef>(rhs)) {
        return std::make_unique<UsesP>(std::get<EntRef>(lhs), std::get<EntRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::FOLLOWS:
      if (std::holds_alternative<StmtRef>(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<Follows>(std::get<StmtRef>(lhs),
                                         std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::FOLLOWST:
      if (std::holds_alternative<StmtRef>(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<FollowsT>(std::get<StmtRef>(lhs),
                                          std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::PARENT:
      if (std::holds_alternative<StmtRef>(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<Parent>(std::get<StmtRef>(lhs),
                                        std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::PARENTT:
      if (std::holds_alternative<StmtRef>(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<ParentT>(std::get<StmtRef>(lhs),
                                         std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::MODIFIESS:
    case RelRefType::MODIFIESP:
      if (std::holds_alternative<StmtRef>(lhs) && std::holds_alternative<EntRef>(rhs)) {
        return std::make_unique<ModifiesS>(std::get<StmtRef>(lhs),
                                           std::get<EntRef>(rhs), is_not);
      } else if (std::holds_alternative<EntRef>(lhs) && std::holds_alternative<EntRef>(rhs)) {
        return std::make_unique<ModifiesP>(std::get<EntRef>(lhs), std::get<EntRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::CALLS:
      if (std::holds_alternative<EntRef >(lhs) && std::holds_alternative<EntRef>(rhs)) {
        return std::make_unique<Calls>(std::get<EntRef>(lhs), std::get<EntRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::CALLST:
      if (std::holds_alternative<EntRef >(lhs) && std::holds_alternative<EntRef>(rhs)) {
        return std::make_unique<CallsT>(std::get<EntRef>(lhs), std::get<EntRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::NEXT:
      if (std::holds_alternative<StmtRef >(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<Next>(std::get<StmtRef>(lhs), std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::NEXTT:
      if (std::holds_alternative<StmtRef >(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<NextT>(std::get<StmtRef>(lhs), std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    case RelRefType::AFFECTS:
      if (std::holds_alternative<StmtRef >(lhs) && std::holds_alternative<StmtRef>(rhs)) {
        return std::make_unique<Affects>(std::get<StmtRef>(lhs), std::get<StmtRef>(rhs), is_not);
      } else {
        throw QpsSyntaxError("Syntax error");
      }
    default:
      throw QpsSyntaxError("Unrecognised relation reference");
  }
}
