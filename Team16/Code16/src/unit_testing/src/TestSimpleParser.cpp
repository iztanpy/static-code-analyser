#include "SP/SimpleParser.h"
#include "SP/DesignExtractor.h"
#include "SP/SPTokeniser.h"

#include "catch.hpp"

using namespace std;

TokenType variableType = TokenType::kLiteralName;
TokenType constantType = TokenType::kLiteralInteger;
TokenType endType = TokenType::kSepSemicolon;
TokenType plusType = TokenType::kOperatorPlus;
TokenType equalType = TokenType::kEntityAssign;
Token tokenX = Token(variableType,"x", 0);
Token tokenY = Token(variableType,"y", 0);
Token tokenW = Token(variableType,"w", 0);
Token tokenEqual = Token(equalType, 0);
Token tokenX2 = Token(variableType,"x", 0);
Token tokenPlus = Token(plusType);
Token token1 = Token(constantType,"1", 0);
Token tokenEnd = Token(endType);

TEST_CASE("Test SimpleParser") { // line 0: x = x + 1
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    std::vector<Token> my_tokens{tokenX, tokenEqual, tokenX2, tokenPlus, token1, tokenEnd};
    std::cout << "tokens size " << my_tokens.size() << std::endl;
    REQUIRE(parser.parse(my_tokens, 0) == 6);
}

TEST_CASE("Test DesignExtractor1") { // x = x + 1
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>();
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>();
    std::shared_ptr<TNode> node1 = std::make_shared<ConstantTNode>(token1.value);
    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus->addChild(node1);
    nodeEqual->addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    auto* visitor = new ASTVisitor();
    de.extractDesign(nodeEqual, visitor);

    unordered_set<string> varSet = unordered_set<string>({"x"});
    unordered_set<string> constSet = unordered_set<string>({"1"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>({{"x", constSet}});

    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}


TEST_CASE("Test DesignExtractor only using only variables") { // x = x + y + w
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    std::shared_ptr<TNode> nodey = std::make_shared<VariableTNode>(tokenY.value);
    std::shared_ptr<TNode> nodePlus2 = std::make_shared<PlusTNode>();
    std::shared_ptr<TNode> nodew = std::make_shared<VariableTNode>(tokenW.value);
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>();
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>();

    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus2->addChild(nodey);
    nodePlus2->addChild(nodew);
    nodePlus->addChild(nodePlus2);
    nodeEqual->addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    auto* visitor = new ASTVisitor();
    de.extractDesign(nodeEqual, visitor);

    unordered_set<string> varSet = unordered_set<string>({"x", "y", "w"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});
    unordered_set<string> constSet = unordered_set<string>({});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>({});

    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}


TEST_CASE("Test DesignExtractor only using variables and constants") { // x = x + 1 + w
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    std::shared_ptr<TNode> nodePlus2 = std::make_shared<PlusTNode>();
    std::shared_ptr<TNode> nodew = std::make_shared<VariableTNode>(tokenW.value);
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>();
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>();
    std::shared_ptr<TNode> node1 = std::make_shared<ConstantTNode>(token1.value);

    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus2->addChild(node1);
    nodePlus2->addChild(nodew);
    nodePlus->addChild(nodePlus2);
    nodeEqual->addChild(nodePlus);

    DesignExtractor de = *new DesignExtractor();
    auto* visitor = new ASTVisitor();
    de.extractDesign(nodeEqual, visitor);

    unordered_set<string> varSet = unordered_set<string>({"x", "w"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});
    unordered_set<string> constSet = unordered_set<string>({"1"});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>({{"x", constSet}});

    REQUIRE(visitor->getAssignVarHashmap() == varUseMap);
    REQUIRE(visitor->getAssignConstHashmap() == constUseMap);
    REQUIRE(visitor->getVariablesHashset() == varSet);
    REQUIRE(visitor->getConstantsHashset() == constSet);
}


TEST_CASE("Test SimpleParser & DesignExtractor integration") { // x = x + 1;
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    vector<Token> tokenVector;
    tokenVector.push_back(tokenX);
    tokenVector.push_back(tokenEqual);
    tokenVector.push_back(tokenX);
    tokenVector.push_back(tokenPlus);
    tokenVector.push_back(token1);
    tokenVector.push_back(tokenEnd);
    parser.parse(tokenVector, 0);

    unordered_set<string> varSet = unordered_set<string>({"x"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>({{"x", varSet}});
    unordered_set<string> constSet = unordered_set<string>({"1"});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>({{"x", constSet}});

    REQUIRE(parser.assignmentParser->getAssignVarHashmap() == varUseMap);
    REQUIRE(parser.assignmentParser->getAssignConstHashmap() == constUseMap);
    REQUIRE(parser.assignmentParser->getVariablesHashset() == varSet);
    REQUIRE(parser.assignmentParser->getConstantsHashset() == constSet);
}

TEST_CASE(("Test SP single procedure")) {
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    SPtokeniser tokeniser;

    string simpleProgram = "procedure p {x = x + 1;}";
    std::vector<struct Token> tokens_simple = tokeniser.tokenise(simpleProgram);
    parser.parse(tokens_simple, 0);

    unordered_set<string> varSet = unordered_set<string>({"x"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>(
            {{"x", varSet}});
    unordered_set<string> constSet = unordered_set<string>({"1"});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>(
            {{"x", constSet}});

    REQUIRE(parser.assignmentParser->getAssignVarHashmap() == varUseMap);
    REQUIRE(parser.assignmentParser->getAssignConstHashmap() == constUseMap);
    REQUIRE(parser.assignmentParser->getVariablesHashset() == varSet);
    REQUIRE(parser.assignmentParser->getConstantsHashset() == constSet);
}

TEST_CASE(("Test SP multi procedure with keyword names")) {
    PKB pkb = PKB();
    WriteFacade writeFacade = WriteFacade(&pkb);
    SimpleParser parser(&writeFacade);
    SPtokeniser tokeniser;

    string simpleProgram = "procedure p {x = x + 1;} procedure procedure {y = y + x; w = w + 1;}";
    std::vector<struct Token> tokens_simple = tokeniser.tokenise(simpleProgram);
    parser.parse(tokens_simple, 0);

    unordered_set<string> varSet = unordered_set<string>({"x", "y", "w"});
    unordered_map<string, unordered_set<string>> varUseMap = unordered_map<string, unordered_set<string>>(
            {{"x", {"x"}}, {"y", {"x", "y"}}, {"w", {"w"}}});
    unordered_set<string> constSet = unordered_set<string>({"1"});
    unordered_map<string, unordered_set<string>> constUseMap = unordered_map<string, unordered_set<string>>(
            {{"x", constSet}, {"w", constSet}});

    REQUIRE(parser.assignmentParser->getAssignVarHashmap() == varUseMap);
    REQUIRE(parser.assignmentParser->getAssignConstHashmap() == constUseMap);
    REQUIRE(parser.assignmentParser->getVariablesHashset() == varSet);
    REQUIRE(parser.assignmentParser->getConstantsHashset() == constSet);
}

