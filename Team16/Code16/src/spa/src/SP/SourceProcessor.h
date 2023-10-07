#pragma once

#include <vector>
#include <utility>
#include <set>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "SP/sp_parser/Parser.h"
#include "SP/sp_parser/SimpleParser.h"
#include "SP/TNode.h"
#include "SP/sp_tokeniser/Token.h"
#include "SP/DesignExtractor.h"
#include "PKB/PKB.h"
#include "PKB/API/WriteFacade.h"
#include "SP/sp_tokeniser/SPTokeniser.h"
#include "utils/Error.h"



/**
 * @class SourceProcessor
 * @brief An abstraction responsible for processing and analyzing source code in the Simple Programming Language (SPL).
 *
 * The `SourceProcessor` class is responsible for processing and analyzing source code written in the
 * Simple Programming Language (SPL). It performs tokenization, parsing, and analysis tasks to extract
 * information about the program's structure and behavior by accessing methods from the SimpleParser.
 */
class SourceProcessor {
 private:
    std::unique_ptr<PKB> pkb_ptr;
    SPtokeniser tokeniser;
    SimpleParser simpleParser;
    ASTVisitor visitor;

 public:
    /**
     * @brief Constructs a SourceProcessor object with a WriteFacade for writing analysis results.
     * @param writeFacade A pointer to the WriteFacade used for writing analysis results.
    */
    explicit SourceProcessor(WriteFacade* writeFacade);
    /**
    * @brief Processes the source code and performs analysis.
    * @param simpleProgram The Simple Programming Language (SPL) source code to be processed.
    */
    void processSource(const std::string& simpleProgram);
    /**
     * @brief Retrieves a set of variables used in the program.
     * @return An unordered set containing variable names used in the program.
     */
    std::unordered_set<std::string> getVariables();
    /**
     * @brief Retrieves a set of constants used in the program.
     * @return An unordered set containing constant values used in the program.
     */
    std::unordered_set<std::string> getConstants();
    /**
     * @brief Retrieves a map of statement types associated with statement numbers.
     * @return An unordered map where keys are statement numbers and values are statement types.
     */
    std::unordered_map<int, StmtEntity> getStatementTypesMap();
    /**
     * @brief Retrieves a map of line numbers associated with the left-hand side (LHS) of USES relationships.
     * @return An unordered map where keys are line numbers and values are LHS variables in USES relationships.
     */
    std::unordered_map<int, std::string> getUsesLineLHSMap();
    /**
     * @brief Retrieves a map of line numbers associated with the right-hand side (RHS) of USES relationships.
     * @return An unordered map where keys are line numbers, and values are sets of RHS patterns in USES relationships.
     */
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSPatternMap();
    /**
     * @brief Retrieves a map of line numbers associated with the right-hand side (RHS) variables in USES relationships.
     * @return An unordered map where keys are line numbers, and values are sets of RHS variables in USES relationships.
     */
    std::unordered_map<int, std::unordered_set<std::string>> getUsesLineRHSVarMap();
    /**
     * @brief Retrieves a map of statement numbers associated with variables in USES relationships.
     * @return An unordered map where keys are statement numbers, and values are sets of variables in USES relationships.
     */
    std::unordered_map<int, std::unordered_set<std::string>> getUsesStatementNumberHashmap();
    /**
     * @brief Retrieves a map of statement numbers associated with variables in MODIFIES relationships.
     * @return An unordered map where keys are statement numbers, and values are variables in MODIFIES relationships.
     */
    std::unordered_map<int, std::string> getModifiesMap();
    /**
     * @brief Retrieves a map of statement numbers associated with their parent statement numbers.
     * @return An unordered map where keys are statement numbers, and values are sets of parent statement numbers.
     */
    std::unordered_map<int, std::unordered_set<int>> getParentStatementNumberMap();
    /**
     * @brief Retrieves a map of statement numbers associated with their following statement numbers.
     * @return An unordered map where keys are statement numbers, and values are following statement numbers.
     */
    std::unordered_map<int, int> getFollowStatementNumberMap();
    /**
     * @brief Retrieves a set of procedure labels found in the program.
     * @return A set containing procedure labels defined in the program.
    */
    std::set<std::string> getProcedureLabels();
    std::unordered_map<std::string, std::pair<int, int>> getProcedureLineNumberHashmap();
    std::unordered_map<std::string, std::unordered_set<std::string>> getCallerCalleeHashmap();
    std::unordered_map<int, std::string> getCallStatementNumberEntityHashmap();
};


