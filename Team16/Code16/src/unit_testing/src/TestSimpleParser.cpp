#include "catch.hpp"
#include "SP/Token.h"
#include "SP/SourceProcessor.h"
#include "PKB/API/WriteFacade.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>


// Define your TokenType and Token objects here (if not already defined)
TokenType variableType = TokenType::kLiteralName;
TokenType constantType = TokenType::kLiteralInteger;
TokenType endType = TokenType::kSepSemicolon;
TokenType plusType = TokenType::kOperatorPlus;
TokenType equalType = TokenType::kEntityAssign;
Token tokenX = Token(variableType, "x", 0);
Token tokenY = Token(variableType, "y", 0);
Token tokenW = Token(variableType, "w", 0);
Token tokenEqual = Token(equalType, 0);
Token tokenX2 = Token(variableType, "x", 0);
Token tokenPlus = Token(plusType);
Token token1 = Token(constantType, "1", 0);
Token tokenEnd = Token(endType);


TEST_CASE("Test SimpleParser") { // line 0: x = x + 1
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SimpleParser parser(&writeFacade, new ASTVisitor());
    std::vector<Token> my_tokens{tokenX, tokenEqual, tokenX2, tokenPlus, token1, tokenEnd};
    std::cout << "tokens size " << my_tokens.size() << std::endl;
    REQUIRE(parser.parse(my_tokens, 0) == 6);
}

TEST_CASE("Test DesignExtractor1") { // x = x + 1
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>(tokenEqual.lineNumber);
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX2.lineNumber, tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>(tokenPlus.lineNumber);
    std::shared_ptr<TNode> node1 = std::make_shared<ConstantTNode>(token1.lineNumber, token1.value);
    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus->addChild(node1);
    nodeEqual->addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    auto* visitor = new ASTVisitor();
    de.extractDesign(nodeEqual, visitor);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"x"});
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({{"x", varSet}});
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({{"x", constSet}});

    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}


TEST_CASE("Test DesignExtractor only using only variables") { // x = x + y + w
    std::shared_ptr<TNode> nodey = std::make_shared<VariableTNode>(tokenY.lineNumber, tokenY.value);
    std::shared_ptr<TNode> nodePlus2 = std::make_shared<PlusTNode>(tokenPlus.lineNumber);
    std::shared_ptr<TNode> nodew = std::make_shared<VariableTNode>(tokenW.lineNumber, tokenW.value);
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>(tokenEqual.lineNumber);
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>(tokenPlus.lineNumber);

    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus2->addChild(nodey);
    nodePlus2->addChild(nodew);
    nodePlus->addChild(nodePlus2);
    nodeEqual->addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    auto* visitor = new ASTVisitor();
    de.extractDesign(nodeEqual, visitor);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"x", "y", "w"});
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({{"x", varSet}});
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({});
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({});

    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}


TEST_CASE("Test DesignExtractor only using variables and constants") { // x = x + 1 + w
    std::shared_ptr<TNode> nodePlus2 = std::make_shared<PlusTNode>(tokenEqual.lineNumber);
    std::shared_ptr<TNode> nodew = std::make_shared<VariableTNode>(tokenW.lineNumber, tokenW.value);
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>(tokenEqual.lineNumber);
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>(tokenPlus.lineNumber);
    std::shared_ptr<TNode> node1 = std::make_shared<ConstantTNode>(token1.lineNumber, token1.value);

    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus2->addChild(node1);
    nodePlus2->addChild(nodew);
    nodePlus->addChild(nodePlus2);
    nodeEqual->addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    auto* visitor = new ASTVisitor();
    de.extractDesign(nodeEqual, visitor);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "w" });
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", varSet} });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", constSet} });


    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}


TEST_CASE("Test SimpleParser & DesignExtractor integration") { // x = x + 1;
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto* visitor = new ASTVisitor();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SimpleParser parser(&writeFacade, visitor);
    std::vector<Token> tokenVector;
    tokenVector.push_back(tokenX);
    tokenVector.push_back(tokenEqual);
    tokenVector.push_back(tokenX);
    tokenVector.push_back(tokenPlus);
    tokenVector.push_back(token1);
    tokenVector.push_back(tokenEnd);
    parser.parse(tokenVector, 0);


    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x" });
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", varSet} });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", constSet} });


    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}

TEST_CASE(("Test SP single procedure")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x" });
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", varSet} });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", constSet} });

    REQUIRE(sourceProcessor.getAssignVarHashmap() == varUseMap);
    REQUIRE(sourceProcessor.getAssignConstHashmap() == constUseMap);
    REQUIRE(sourceProcessor.getVariablesHashset() == varSet);
    REQUIRE(sourceProcessor.getConstantsHashset() == constSet);
}

TEST_CASE(("Test SP multi procedure with keyword names")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1;} procedure procedure {y = y + x; w = w + 1;}";
    sourceProcessor.processSource(simpleProgram);


    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "w" });
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
        { {"x", {"x"}}, {"y", {"x", "y"}}, {"w", {"w"}} });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
        { {"x", constSet}, {"w", constSet} });

  REQUIRE(sourceProcessor.getAssignVarHashmap() == varUseMap);
  REQUIRE(sourceProcessor.getAssignConstHashmap() == constUseMap);
  REQUIRE(sourceProcessor.getVariablesHashset() == varSet);
  REQUIRE(sourceProcessor.getConstantsHashset() == constSet);
}

TEST_CASE(("Test SP storing of statement numbers for Uses")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({"x", "y"});
    std::unordered_map<std::string, std::unordered_set<std::string>> varUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
            {{"x", {"x"}}, {"y", {"x", "y"}}});
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({"1"});
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>(
            {{"x", constSet}, {"y", constSet}});
    std::unordered_map<int, std::string> usesStatementNumberVarHashmap = std::unordered_map<int, std::string>(
            {{1, "x"}, {2, "y"}, {3, "y"}});

    REQUIRE(sourceProcessor.getAssignVarHashmap() == varUseMap);
    REQUIRE(sourceProcessor.getAssignConstHashmap() == constUseMap);
    REQUIRE(sourceProcessor.getVariablesHashset() == varSet);
    REQUIRE(sourceProcessor.getConstantsHashset() == constSet);
    REQUIRE(sourceProcessor.getUsesStatementNumberVarHashmap() == usesStatementNumberVarHashmap);
}


TEST_CASE(("Test SP storing of assignment statements")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; read r; while(i = 0) { read f;} } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_set<int> assignmentStatementsHashset = std::unordered_set<int>({1, 5});
    REQUIRE(sourceProcessor.getAssignmentStatementsHashset() == assignmentStatementsHashset);
}


TEST_CASE(("Test SP assignment pattern")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_map<int, std::unordered_set<std::string>> usesStatementNumberHashmap = std::unordered_map<int, std::unordered_set<std::string>>(
            {{1, {"x", "1", "x + 1"}}, {2, {"x", "y", "1", "y + x + 1", "y + x"}}, {3, {"y", "x", "1", "y + x + 1", "y + x"}}});
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getUsesStatementNumberHashmap();
    REQUIRE(res == usesStatementNumberHashmap);

}
