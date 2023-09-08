#include "TestWrapper.h"
#include "qps/parsed_query.h"
#include "qps/query_evaluator.h"
#include "qps/query_tokenizer.h"
#include "qps/query_parser.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  // ...rest of your code...
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  // Pass the query to a tokenizer
    std::vector<QueryToken> query_tokens = QueryTokenizer::tokenize(query);
    // Parse the tokens
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(query_tokens);

//  QueryEvaluator evaluator;
//  std::list<std::string> raw_results = evaluator.evaluate(parsed_query);

//  results.assign(raw_results.begin(), raw_results.end());
}
