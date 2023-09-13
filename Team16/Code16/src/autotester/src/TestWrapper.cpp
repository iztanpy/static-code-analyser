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
    this->pkb_ptr = std::make_unique<PKB>();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
    std::ifstream file(filename);
    std::string file_contents;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

    //std::cout << "OMO" << filename << std::endl;

    std::string input;
    std::string line;
    while (std::getline(file, line)) {
        /*    std::cout << "TEST" << std::endl;*/
        input += line;
    }
    WriteFacade writeFacade = WriteFacade(*this->pkb_ptr);
    SimpleParser parser(&writeFacade);
    parser.tokenise(input);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    // Pass the query to a tokenizer
    ReadFacade readFacade = ReadFacade(*this->pkb_ptr);
    QPS qps(readFacade);

    std::unordered_set<std::string> raw_results = qps.Evaluate(query);
    for (const std::string& result : raw_results) {
        std::cout << result << std::endl;
        results.push_back(result);
    }
}
