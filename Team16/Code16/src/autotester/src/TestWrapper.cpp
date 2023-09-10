#include <vector>

#include "TestWrapper.h"
#include "qps/parsed_query.h"
#include "qps/query_evaluator/query_evaluator.h"
#include "qps/query_tokenizer.h"
#include "qps/query_parser.h"
#include "pkb/api/ReadFacade.h"
#include "pkb/PKB.h"

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
    std::ifstream file(filename);
    std::string file_contents;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

    std::string input;
    std::string line;
    while (std::getline(file, line)) {
        input += line + "\n";
    }

    SPtokeniser tokeniser;  // Create an instance of the SPtokeniser class
    std::vector<struct Token> tokens = tokeniser.tokenise(input);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    // Pass the query to a tokenizer
    std::vector<QueryToken> query_tokens = QueryTokenizer::tokenize(query);
    // Parse the tokens
    ParsedQuery parsed_query = QueryParser::ParseTokenizedQuery(query_tokens);

//  PKB PKB;
//  ReadFacade pkb_reader(&PKB);
//  QueryEvaluator query_evaluator(pkb_reader);
//  std::vector<std::string> raw_results = query_evaluator.Evaluate(parsed_query);
//
//  for (const std::string& result : raw_results) {
//    results.push_back(result);
//  }
}
