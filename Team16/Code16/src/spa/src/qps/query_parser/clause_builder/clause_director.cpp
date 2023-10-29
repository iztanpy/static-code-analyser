#include <vector>
#include <memory>
#include "qps/query_parser/clause_builder/clause_director.h"
#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include "qps/declaration.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

std::unique_ptr<Clause> ClauseDirector::makeSelectClause(
    SelectClauseBuilder builder, const QueryToken& token, const std::vector<Declaration>& declarations
) {
  // find the design entity using the synonym in token
  DesignEntity design_entity;
  Declaration decl;
  for (const Declaration& declaration : declarations) {
    if (declaration.synonym == token.text) {
      decl = declaration;
//      design_entity = declaration.design_entity;
      break;
    }
  }
  PQLTokenType token_type = token.type;
  builder.setDeclaration(decl, token_type);
  return builder.getClause();
}

std::unique_ptr<Clause> ClauseDirector::makeSuchThatClause(SuchThatClauseBuilder builder,
                                                           const std::vector<QueryToken>& tokens,
                                                           const std::vector<Declaration>& declarations) {
  builder.setRelRef(RelRef::fromString(tokens[0].text));
  builder.setLhs(tokens[1], declarations);
  builder.setRhs(tokens[2], declarations);
  return builder.getClause();
}
std::unique_ptr<Clause> ClauseDirector::makePatternClause(PatternClauseBuilder builder,
                                                                 const std::vector<QueryToken>& tokens,
                                                                 const std::vector<Declaration>& declarations) {
  builder.setPatternType(tokens[0], declarations);
  builder.setLhs(tokens[1], declarations);
  builder.setRhs(tokens[2], declarations);
  return builder.getClause();
}

std::unique_ptr<Clause> ClauseDirector::makeWithClause(WithClauseBuilder builder,
                                                       const std::vector<QueryToken> & tokens,
                                                       const std::vector<Declaration> & declarations) {
  builder.setLhs(tokens[0], declarations);
  builder.setRhs(tokens[1], declarations);
  return builder.getClause();
}
