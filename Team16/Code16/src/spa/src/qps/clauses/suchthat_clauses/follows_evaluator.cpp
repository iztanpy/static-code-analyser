#include "qps/clauses/suchthat_clauses/follows_evaluator.h"

bool FollowsEvaluator::Handle(int lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
UnaryConstraint FollowsEvaluator::Handle(int lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
bool FollowsEvaluator::Handle(int lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
UnaryConstraint FollowsEvaluator::Handle(Declaration& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
BinaryConstraint FollowsEvaluator::Handle(Declaration& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
UnaryConstraint FollowsEvaluator::Handle(Declaration& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
bool FollowsEvaluator::Handle(Wildcard& lhs, int rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
UnaryConstraint FollowsEvaluator::Handle(Wildcard& lhs, Declaration& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
bool FollowsEvaluator::Handle(Wildcard& lhs, Wildcard& rhs, ReadFacade& pkb_reader, bool is_FollowT) {
  throw QpsSemanticError("[Follows] Not implemented");
}
