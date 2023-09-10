#pragma once

#include "qps/query_evaluator/constraint.h"
#include "pkb/api/read_facade.h"
#include "qps/suchthat_clause.h"

class SuchThatEvaluator {
 public:
  SuchThatEvaluator(ReadFacade& pkb_reader,
                    ParamRef& lhs,
                    ParamRef& rhs);

  virtual ~SuchThatEvaluator() = default;

  Constraint Evaluate();

  virtual Constraint handle(Declaration lhs, Declaration rhs) = 0;
  virtual Constraint handle(Declaration lhs, Underscore rhs) = 0;
  virtual Constraint handle(Declaration lhs, IntegerIdent rhs) = 0;
  virtual Constraint handle(Underscore lhs, Declaration rhs) = 0;
  virtual Constraint handle(Underscore lhs, Underscore rhs) = 0;
  virtual Constraint handle(Underscore lhs, IntegerIdent rhs) = 0;
  virtual Constraint handle(IntegerIdent lhs, Declaration rhs) = 0;
  virtual Constraint handle(IntegerIdent lhs, Underscore rhs) = 0;
  virtual Constraint handle(IntegerIdent lhs, IntegerIdent rhs) = 0;

 private:
  ReadFacade& pkb_reader;
  ParamRef lhs;
  ParamRef rhs;
};
