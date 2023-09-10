#include "qps/suchthat_clause.h"

SuchThatClause::SuchThatClause(RelRef rel_ref,
                               ParamRef& lhs,
                               ParamRef& rhs) : rel_ref(rel_ref), lhs(lhs), rhs(rhs) {}
