#pragma once

#include <utility>
#include <unordered_set>
#include <string>

#include "qps/clauses/suchthat_clauses/suchthat_clause.h"

class UsesS : public SuchThatClause {
 public:
  UsesS(StmtRef lhs, EntRef rhs);
  StmtRef lhs;
  EntRef rhs;

  Constraint Evaluate(ReadFacade& pkb_reader) override;

 private:
  Constraint handle(Declaration& declaration1, Declaration& declaration2, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
  Constraint handle(Declaration& declaration, Wildcard& wildcard, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
  Constraint handle(Declaration& declaration, std::string& entity_name, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
  Constraint handle(Wildcard& wildcard, Declaration& declaration, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
  Constraint handle(Wildcard& wildcard1, Wildcard& wildcard2, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
  Constraint handle(Wildcard& wildcard, std::string& entity_name, ReadFacade& pkb_reader) {
    throw QpsSemanticError("Not implemented");
  }
  Constraint handle(int stmt_num, Declaration& declaration, ReadFacade& pkb_reader);
  Constraint handle(int stmt_num, Wildcard& wildcard, ReadFacade& pkb_reader);
  Constraint handle(int stmt_num, std::string& entity_name, ReadFacade& pkb_reader);
};
