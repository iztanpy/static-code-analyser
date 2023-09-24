#include <vector>
#include <memory>
#include "qps/query_parser/clause_builder/clause_director.h"
#include "qps/clauses/select_clause.h"
#include "qps/query_parser/clause_builder/select_clause_builder.h"
#include "qps/declaration.h"
#include "qps/query_parser/clause_builder/pattern_clause_builder.h"

SelectClause ClauseDirector::makeSelectClause(
    SelectClauseBuilder builder, const QueryToken& token, const std::vector<Declaration>& declarations
) {
  // find the design entity using the synonym in token
  DesignEntity design_entity;
  for (const Declaration& declaration : declarations) {
    if (declaration.synonym == token.text) {
      design_entity = declaration.design_entity;
      break;
    }
  }
  builder.setDeclaration({token.text, design_entity});
  return builder.getClause();
}

std::unique_ptr<SuchThatClause> ClauseDirector::makeSuchThatClause(SuchThatClauseBuilder builder,
                                                                   const std::vector<QueryToken>& tokens,
                                                                   const std::vector<Declaration>& declarations) {
  builder.setRelRef(RelRef::fromString(tokens[0].text));
  builder.setLhs(tokens[1], declarations);
  builder.setRhs(tokens[2], declarations);
  return builder.getClause();
}
std::unique_ptr<PatternClause> ClauseDirector::makePatternClause(PatternClauseBuilder builder,
                                                                 const std::vector<QueryToken>& tokens,
                                                                 const std::vector<Declaration>& declarations) {
  Declaration syn_assignment;
  for (const Declaration& declaration : declarations) {
    if (declaration.synonym == tokens[0].text) {
      syn_assignment = declaration;
      break;
    }
  }
  builder.setSynAssignment(syn_assignment);
  builder.setLhs(tokens[1], declarations);
  builder.setRhs(tokens[2], declarations);
  return builder.getClause();
}
