#pragma once

#include <vector>
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
    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignVarHashmap();
    std::unordered_map<std::string, std::unordered_set<std::string>> getAssignConstHashmap();
    std::unordered_set<std::string> getVariablesHashset();
    std::unordered_set<std::string> getConstantsHashset();

    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap();
    std::unordered_map<int, std::string> getUsesStatementNumberVarHashmap();
    std::unordered_set<int> getAssignmentStatementsHashset();
};


