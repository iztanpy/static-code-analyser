#include "SourceProcessor.h"

SourceProcessor::SourceProcessor(WriteFacade* writeFacadePtr) : simpleParser(writeFacadePtr, &this->visitor) {}

void SourceProcessor::processSource(const std::string& simpleProgram) {
    std::vector<struct Token> tokens = tokeniser.tokenise(simpleProgram);
    try {
        simpleParser.parse(tokens, 0);
    } catch (const InvalidSyntaxError e) {
        e.log();
    } catch (const InvalidTokenTypeError e) {
        e.log();
    }
    catch (const InvalidSemanticError e) {
        e.log();
    }
}

std::unordered_set<std::string> SourceProcessor::getVariables() {
  return visitor.getVariables();
}

std::unordered_set<std::string> SourceProcessor::getConstants() {
  return visitor.getConstants();
}

std::unordered_map<int, StatementTypes> SourceProcessor::getStatementTypesMap() {
  return visitor.getStatementTypesMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getUsesLineRHSPatternMap() {
  return visitor.getUsesLineRHSPatternMap();
}

std::unordered_map<int, std::string> SourceProcessor::getUsesLineLHSMap() {
  return visitor.getUsesLineLHSMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getUsesLineRHSVarMap() {
  return visitor.getUsesLineRHSVarMap();
}

std::unordered_map<int, std::unordered_set<std::string>> SourceProcessor::getUsesStatementNumberHashmap() {
  return visitor.getUsesStatementNumberHashmap();
}

std::unordered_map<int, std::string> SourceProcessor::getModifiesMap() {
  return visitor.getModifiesMap();
}

std::unordered_map<int, std::unordered_set<int>> SourceProcessor::getParentStatementNumberMap() {
    return visitor.getParentStatementNumberMap();
}

std::unordered_map<int, int> SourceProcessor::getFollowStatementNumberMap() {
    return visitor.getFollowStatementNumberMap();
}
