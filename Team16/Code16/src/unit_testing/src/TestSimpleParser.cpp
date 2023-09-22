#include "catch.hpp"
#include "SP/sp_tokeniser/Token.h"
#include "SP/SourceProcessor.h"
#include "utils/StmtEntity.h"
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
TokenType readType = TokenType::kEntityRead;
Token tokenProc = Token(TokenType::kEntityProcedure, "p", 0);
Token tokenProcName = Token(TokenType::kLiteralName, "poo", 0);
Token tokenOpenBrace = Token(TokenType::kSepOpenBrace, 0);
Token tokenCloseBrace = Token(TokenType::kSepCloseBrace, 0);
Token tokenX = Token(variableType, "x", 0);
Token tokenY = Token(variableType, "y", 0);
Token tokenW = Token(variableType, "w", 0);
Token tokenEqual = Token(equalType, 0);
Token tokenX2 = Token(variableType, "x", 0);
Token tokenPlus = Token(plusType);
Token token1 = Token(constantType, "1", 0);
Token tokenEnd = Token(endType);
Token tokenRead = Token(readType);




TEST_CASE("Test follows relation one level nesting") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = "procedure p { x = x + 1; x = x + 2; x = x + 3; }";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, int> followStatementNumberHashmap = {
        {1, 2},
        {2, 3},
    };
    std::unordered_map<int, int> res = sourceProcessor.getFollowStatementNumberMap();
    for (auto it = res.begin(); it != res.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
    REQUIRE(sourceProcessor.getFollowStatementNumberMap() == followStatementNumberHashmap);
}

//
//TEST_CASE("Test Multiple Procedures") {
//    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//    WriteFacade writeFacade(*pkb_ptr);
//    SourceProcessor sourceProcessor(&writeFacade);
//    std::string simpleProgram3 = "procedure p { x = x + 1; x = x + 2; x = x + 3; } procedure p { x = x + 1; x = x + 2; x = x + 3; }";
//    sourceProcessor.processSource(simpleProgram3);
//    // check std log to see if error is logged
//    REQUIRE(1 == 1); 
//}

TEST_CASE("Test follows & parent relation two level nesting") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
        procedure p {
          x = 0;
          while (x == 0) {
            if (x == 1) then {
              read x;
              x = x + 1;
            } else {
              print k;
              x = x + 3;
            }
            x = x + 1;
          }
          while (x > 0) {
            x = x + 1;
          }
        }
     )";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, int> followStatementNumberHashmap = {
        {1, 2},
        {2, 9},
        {3, 8},
        {4, 5},
        {6, 7},
    };
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {2, {3, 8}},
        {3, {4, 5, 6, 7}},
        {9, {10}}
    };

    std::unordered_map<int, int> res = sourceProcessor.getFollowStatementNumberMap();
    for (auto it = res.begin(); it != res.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
    REQUIRE(sourceProcessor.getFollowStatementNumberMap() == followStatementNumberHashmap);
    REQUIRE(sourceProcessor.getParentStatementNumberMap() == parentStatementNumberHashmap);
}


TEST_CASE("Test if else nested loop retrieval") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = "procedure p { if (number > 0) then { if (x == 1) then { x = x + 1; } } else { x = x + 1; } }";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap2 = {
        {1, {2, 4}},
        {2, {3}}
    };
    std::unordered_map<int, std::unordered_set<int>> res2 = sourceProcessor.getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap2 == res2);
}

TEST_CASE("Test if else loop retrieval") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p { if (number > 0) then { x = x + y; } else { x = 3; } }";
    sourceProcessor.processSource(simpleProgram2);
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2, 3}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}

TEST_CASE("Test if-else-while nested retrieval") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string complexProgram = "procedure p { "
        "while (number > 0) { " // 1
        "if (x == 1) then { " // 2
        "while (x == 1) { " // 3
        "x = x + 1; " // 4
        "} "
        "} else { "
        "x = x + 2; " // 5
        "} "
        "} "
        "}";
    sourceProcessor.processSource(complexProgram);

    // Expected parent statement numbers map (with while starting from index 1)
    std::unordered_map<int, std::unordered_set<int>> expectedParentStatementNumberHashmap = {
        {1, {2}},    // Procedure p contains while (number > 0)
        {2, {3,5}}, // While (number > 0) contains if (x == 1) and else
        {3, {4}},    // If (x == 1) contains while (x == 1)
    };

    // Retrieve the actual parent statement numbers map, excluding the procedure declaration (statement 1)
    std::unordered_map<int, std::unordered_set<int>> actualParentStatementNumberHashmap = sourceProcessor.getParentStatementNumberMap();

    // Check if the actual map matches the expected map
    REQUIRE(expectedParentStatementNumberHashmap == actualParentStatementNumberHashmap);

}

TEST_CASE(("Test multiple while while nested retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p {  while (number > 0) { while (x > 0) { while (a > b) {x = x + 1; } } }";
    sourceProcessor.processSource(simpleProgram2);
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2}},
        {2, {3}},
        {3, {4}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}

TEST_CASE(("Test SP storing of assignment statements")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram =
        "procedure p {x = x + 1; read r; while (i == 0) { read f;} } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
        { {1, "x"}, {5, "y"} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int,
        std::unordered_set<std::string>>({ {1, {"x", "1", "x + 1"}},
                                          {5, {"y", "x", "y + x", "y + x + 1", "1"}} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
        std::unordered_set<std::string>>({ {1, {"x"}}, {3, {"i"}}, {5, {"x", "y"}} });
    REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
    REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test SP Uses: nested while and if/then/else")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram =
      "procedure p {x = x + 1; read r; while (i == 0) { if( (a==3)||(b>1) ) then{ read f;} else{ read k; }} } "
      "procedure wee { while(!(c==1)){y = y + x + 1;}}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {1, "x"}, {8, "y"} });
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int,
      std::unordered_set<std::string>>({ {1, {"x", "1", "x + 1"}},
                                            {8, {"y", "x", "y + x", "y + x + 1", "1"}} });
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>({
          {1, {"x"}},
          {3, {"i"}},
          {4, {"a", "b"}},
          {7, {"c"}},
          {8, {"x", "y"}}
      });
  REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
  REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
}


TEST_CASE(("Test Single While Loop Retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p {  while (number > 0)  { digit = number + 10; sum = sum + digit; } }";
    sourceProcessor.processSource(simpleProgram2);

    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2, 3}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}


TEST_CASE(("Test 2 Nested While Loops Retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p {  while (number > 0)  { while (test > 2) { test = 0; a= x+y; } } }";
    sourceProcessor.processSource(simpleProgram2);

    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2}},
        {2, {3,4}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}


TEST_CASE(("Test Conditional Tokens Retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p { while ((x == 1) || (x==2))  { read x; } }";
    sourceProcessor.processSource(simpleProgram2);
    std::string simpleProgram3 = "procedure q { while ((x != 1) || (y != 1)) { read x; } }";
    sourceProcessor.processSource(simpleProgram3);
    std::string simpleProgram4 = "procedure r { while (!(x == 1)) { read x; } }";
    sourceProcessor.processSource(simpleProgram4);
    std::string simpleProgram5 = "procedure s { while ((!x) || (x == y)) { read x; } }";
    sourceProcessor.processSource(simpleProgram5);
  
    REQUIRE(1 == 1);
}


TEST_CASE(("Test Print Parser")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p { print k; }";
    REQUIRE(1 == 1);
}



TEST_CASE(("Test Call Parser")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p { call p; }";
    REQUIRE(1 == 1);
}


TEST_CASE("Test SimpleParser") { // line 0: x = x + 1
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SimpleParser parser(&writeFacade, new ASTVisitor());
    std::vector<Token> my_tokens{tokenProc, tokenProcName, tokenOpenBrace, tokenX, tokenEqual, tokenX2, tokenPlus, token1,
                                 tokenEnd, tokenCloseBrace };
    std::cout << "tokens size " << my_tokens.size() << std::endl;
    REQUIRE(parser.parse(my_tokens, 0) == 10);
}
//
//TEST_CASE("Test DesignExtractor only using variables and constants") { // x = x + 1 + w
//    std::shared_ptr<TNode> nodePlus2 = std::make_shared<PlusTNode>(tokenEqual.lineNumber);
//    std::shared_ptr<TNode> nodew = std::make_shared<VariableTNode>(tokenW.lineNumber, tokenW.value);
//    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
//    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>(tokenEqual.lineNumber);
//    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(tokenX.lineNumber, tokenX.value);
//    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>(tokenPlus.lineNumber);
//    std::shared_ptr<TNode> node1 = std::make_shared<ConstantTNode>(token1.lineNumber, token1.value);
//
//    nodeEqual->addChild(nodeX);
//    nodePlus->addChild(nodeX2);
//    nodePlus->addChild(node1);
//    nodePlus2->addChild(nodePlus);
//    nodePlus2->addChild(nodew);
//    nodeEqual->addChild(nodePlus2);
//
//    DesignExtractor de = *new DesignExtractor();
//    auto* visitor = new ASTVisitor();
//    de.extractDesign(nodeEqual, visitor);
//
//    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "w" });
//    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
//
//    REQUIRE(visitor->getVariables() == varSet);
//    REQUIRE(visitor->getConstants() == constSet);
//}

//
//TEST_CASE("Test SimpleParser & DesignExtractor integration") { // x = x + 1;
//    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//    auto* visitor = new ASTVisitor();
//    auto writeFacade = WriteFacade(*pkb_ptr);
//    SimpleParser parser(&writeFacade, visitor);
//    std::vector<Token> tokenVector;
//    tokenVector.push_back(tokenX);
//    tokenVector.push_back(tokenEqual);
//    tokenVector.push_back(tokenX);
//    tokenVector.push_back(tokenPlus);
//    tokenVector.push_back(token1);
//    tokenVector.push_back(tokenEnd);
//    parser.parse(tokenVector, 0);
//
//    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x" });
//    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
//    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x", "1", "x + 1"}} });
//    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"} });
//    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}} });
//    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", constSet} });
//
//    REQUIRE(visitor->getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
//    REQUIRE(visitor->getUsesLineLHSMap() == usesLineLHSMap);
//    REQUIRE(visitor->getUsesLineRHSVarMap() == usesLineRHSVarMap);
//    REQUIRE(visitor->getVariables() == varSet);
//    REQUIRE(visitor->getConstants() == constSet);
//}


TEST_CASE(("Test SP single procedure")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x", "1", "x + 1"}} });
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}} });
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", constSet} });

    REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
    REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
    REQUIRE(sourceProcessor.getVariables() == varSet);
    REQUIRE(sourceProcessor.getConstants() == constSet);
}

TEST_CASE(("Test SP multi procedure with keyword names")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1;} procedure procedure {y = y + x; w = w + 1;}";
    sourceProcessor.processSource(simpleProgram);


    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "w" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x", "1", "x + 1"}}, {2, {"y", "x", "y + x"}}, {3, {"w", "1", "w + 1"}}});
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"}, {2, "y"}, {3, "w"} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}}, {2, {"x", "y"}}, {3, {"w"}}});

    REQUIRE(sourceProcessor.getVariables() == varSet);
    REQUIRE(sourceProcessor.getConstants() == constSet);
    REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
    REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
    REQUIRE(sourceProcessor.getVariables() == varSet);
    REQUIRE(sourceProcessor.getConstants() == constSet);
}

TEST_CASE(("Test SP storing of Uses: Assign")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1;} procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
        { {1, "x"}, {2, "y"}, {3, "y"} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int,
        std::unordered_set<std::string>>({
                                          {1, {"x + 1", "1", "x"}},
                                          {2, {"y + x + 1", "y + x", "x", "y", "1"}},
                                          {3, {"y + x + 1", "y + x", "x", "y", "1"}},
        });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>({{1, {"x"}}, {2, {"x", "y"}}, {3, {"x", "y"}}});
    REQUIRE(sourceProcessor.getVariables() == varSet);
    REQUIRE(sourceProcessor.getConstants() == constSet);
    REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
    REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
}




TEST_CASE(("Test SP to PKB <line, RHS patterns>, <line, LHS var>")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; read k;} procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap =
        std::unordered_map<int, std::unordered_set<std::string>>(
        { {1, {"x", "1", "x + 1"}},
          {2, {"x", "y", "1", "y + x + 1", "y + x"}},
          {4, {"y", "x", "1", "y + x + 1", "y + x"}} });
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getUsesLineRHSPatternMap();
    REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
}

TEST_CASE(("Test SP read")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {a = x + 1; y = y + x + 1; } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap =
        std::unordered_map<int, std::unordered_set<std::string>>(
            {{1, {"x", "1", "x + 1"}},
             {2, {"x", "y", "1", "y + x + 1", "y + x"}},
             {3, {"y", "x", "1", "y + x + 1", "y + x"}}});
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
            {{1, "a"}, {2, "y"}, {3, "y"}});
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getUsesLineRHSPatternMap();
    std::unordered_map<int, std::string> res2 = sourceProcessor.getUsesLineLHSMap();
    REQUIRE(res == usesLineRHSPatternMap);
    REQUIRE(res2 == usesLineLHSMap);
}


TEST_CASE(("Test SP to PKB LineUsesVar")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    WriteFacade writeFacade = WriteFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);

    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; } procedure wee { w = x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap =
        std::unordered_map<int, std::unordered_set<std::string>>(
            {{1, {"x"}}, {2, {"x", "y"}}, {3, {"x"}}});
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getUsesLineRHSVarMap();
    REQUIRE(res == usesLineRHSVarMap);
}



TEST_CASE(("Test SP storing of Uses: Print")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; print k;} procedure wee {y =y +x + 1;print kay;}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "k", "kay" });
  std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {1, "x"}, {2, "y"}, {4, "y"} });
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int,
      std::unordered_set<std::string>>({ {1, {"x + 1", "1", "x"}},
                                            {2, {"y + x + 1", "y + x", "x", "y", "1"}},
                                            {4, {"y + x + 1", "y + x", "x", "y", "1"}}});
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>(
          {{1, {"x"}}, {2, {"x", "y"}}, {3, {"k"}}, {4, {"x", "y"}}, {5, {"kay"}}});
  REQUIRE(sourceProcessor.getVariables() == varSet);
  REQUIRE(sourceProcessor.getConstants() == constSet);
  REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
  REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test Uses: SP Assignment statement with all operators")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p {a = (x + 1) * y / 5 + 1; kay = five / 9 - var;}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "a", "kay", "five", "var" });
  std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1", "5", "9"});
  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {1, "a"}, {2, "kay"}});
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSPatternMap = std::unordered_map<int,
      std::unordered_set<std::string>>({
        {1, {"x + 1 * y / 5 + 1", "x + 1 * y / 5", "x + 1 * y", "x + 1", "y", "5", "x", "1"}},
        {2, {"five / 9 - var", "five / 9", "five", "9", "var"}}});
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
       std::unordered_set<std::string>>({{1, {"x", "y"}}, {2, {"five", "var"}}});

  REQUIRE(sourceProcessor.getVariables() == varSet);
  REQUIRE(sourceProcessor.getConstants() == constSet);
  REQUIRE(sourceProcessor.getUsesLineRHSPatternMap() == usesLineRHSPatternMap);
  REQUIRE(sourceProcessor.getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test SP Statement type storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p { while (a==1) { if (i != 0) then { read f; } else { print k; a = 1 + w; }}}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, StmtEntity> statementTypesMap = std::unordered_map<int, StmtEntity>(
      {{0, StmtEntity::kProcedure},
       {1, StmtEntity::kWhile},
       {2, StmtEntity::kIf},
       {3, StmtEntity::kRead},
       {4, StmtEntity::kPrint},
       {5, StmtEntity::kAssign}});
  REQUIRE(sourceProcessor.getStatementTypesMap() == statementTypesMap);
}


TEST_CASE(("Test SP Modifies storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p { if (i != 0) then { x = x + 1 + r; } else { read k; a = 1 + w; }}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, std::string> modifiesMap =
      std::unordered_map<int, std::string>(
      {{2, "x"},
         {3, "k"},
         {4, "a"}});

  REQUIRE(sourceProcessor.getModifiesMap() == modifiesMap);
}

TEST_CASE(("Test SP Procedures storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = WriteFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram =
      "procedure p { if (i != 0) then { x = x + 1 + r; }} procedure k { read r; } procedure kay { print k; a = 1 + w; }";
  sourceProcessor.processSource(simpleProgram);
  std::set<std::string> procedures = std::set<std::string>({"p","k","kay"});
  REQUIRE(sourceProcessor.getProcedureLabels() == procedures);
}
