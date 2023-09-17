#include "SourceProcessor.h"

SourceProcessor::SourceProcessor(WriteFacade* writeFacadePtr) : simpleParser(writeFacadePtr, &this->visitor) {}

void SourceProcessor::processSource(const std::string& simpleProgram) {
    std::vector<struct Token> tokens = tokeniser.tokenise(simpleProgram);
    simpleParser.parse(tokens, 0);
}

std::unordered_map<std::string, std::unordered_set<std::string>> SourceProcessor::getAssignVarHashmap() {
  return visitor.getAssignVarHashmap();
}

std::unordered_map<std::string, std::unordered_set<std::string>> SourceProcessor::getAssignConstHashmap() {
  return visitor.getAssignConstHashmap();
}

std::unordered_set<std::string> SourceProcessor::getVariablesHashset() {
  return visitor.getVariablesHashset();
}

std::unordered_set<std::string> SourceProcessor::getConstantsHashset() {
  return visitor.getConstantsHashset();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getUsesStatementNumberHashmap() {
  return visitor.getUsesStatementNumberHashmap();
}

std::unordered_map<int, std::string> SourceProcessor::getUsesStatementNumberVarHashmap() {
  return visitor.getUsesStatementNumberVarHashmap();
}

std::unordered_set<int> SourceProcessor::getAssignmentStatementsHashset() {
  return visitor.getAssignmentStatementsHashset();
}
