#pragma once

#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <memory>

#include "PKB/API/ReadFacade.h"
#include "PKB/API/WriteFacade.h"
#include "PKB/PKB.h"
#include "qps/qps.h"
#include "SP/SourceProcessor.h"
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

 private:
  std::unique_ptr<PKB> pkb_ptr;
};
