#include "TestWrapper.h"

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

	SPtokeniser tokeniser; // Create an instance of the SPtokeniser class
	std::vector<struct Token> tokens = tokeniser.tokenise(input);

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
