#include "Parser.h"

DesignExtractor* Parser::designExtractor = new DesignExtractor();

int Parser::index = 0;

int Parser::lineNumber = 1;

int Parser::currIfDepth = 0;

int Parser::currWhileDepth = 0;

bool Parser::isParsingProcedure = false;

std::stack<std::set<int>> Parser::followsStatementStack = std::stack<std::set<int>>();

std::stack<std::string> Parser::controlStructureStack = std::stack<std::string>();

std::unordered_map<int, std::unordered_set<int>> Parser::parentStatementNumberHashmap =
    std::unordered_map<int, std::unordered_set<int>>();

std::stack<int> Parser::parentStatementStack = std::stack<int>();

std::unordered_map<std::string, std::shared_ptr<CfgNode>> cfgNodeMap = std::unordered_map<std::string, std::shared_ptr<CfgNode>>(); 

void Parser::start_parse(std::vector<Token>& tokens, int curr_index) {
  int next_index = parse(tokens);
  if (next_index == -1) {
    throw InvalidSyntaxError();
  }
  index = next_index;
}
