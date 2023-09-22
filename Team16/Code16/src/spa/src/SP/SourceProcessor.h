#pragma once

#include <vector>
#include <set>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "SP/sp_parser/SimpleParser.h"
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/DesignExtractor.h"
#include "PKB/PKB.h"
#include "PKB/API/WriteFacade.h"
#include "SP/sp_tokeniser/SPTokeniser.h"
#include "utils/Error.h"

class SourceProcessor {
 private:
    std::unique_ptr<PKB> pkb_ptr;
    SPtokeniser tokeniser;
    SimpleParser simpleParser;
    ASTVisitor visitor;
 public:
    explicit SourceProcessor(WriteFacade* writeFacade);
    void processSource(const std::string& simpleProgram);
    std::unordered_set<std::string> getVariables();
    std::unordered_set<std::string> getConstants();
    std::unordered_map<int, StmtEntity> getStatementTypesMap();
    std::unordered_map<int, std::string> getUsesLineLHSMap();
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSPatternMap();
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSVarMap();
    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap();
    std::unordered_map<int, std::string> getModifiesMap();
    std::unordered_map<int, std::unordered_set<int>> getParentStatementNumberMap();
    std::unordered_map<int, int> getFollowStatementNumberMap();
    std::set<std::string> getProcedureLabels();
};


