#include "qps/clauses/suchthat_clauses/parent_evaluator.h"

bool ParentEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] PKB Not implemented");
}
UnaryConstraint ParentEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
bool ParentEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
UnaryConstraint ParentEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
BinaryConstraint ParentEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
UnaryConstraint ParentEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
bool ParentEvaluator::Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
UnaryConstraint ParentEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
bool ParentEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_ParentT) {
  throw QpsSemanticError("[Parent] Not implemented");
}
