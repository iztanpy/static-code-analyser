#pragma once

#include "qps/query_evaluator/constraint.h"
#include "PKB/API/ReadFacade.h"
#include "qps/suchthat_clause.h"

/**
 * @brief Evaluates the SuchThat clause of a query
 *  Base class that all other SuchThatEvaluators inherit from
 *  We design it this way so that let's say UsesSEvaluator can override
 *  the below 9 virtual handle function to handle the different types of parameters
 *  Children classes can share the "strategy" logic within Evaluate
 *
 *  Users of children classes can just call Evaluate() to get back
 *  the Constraint object which contains the possible values that the
 *  synonym can take
 */
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
