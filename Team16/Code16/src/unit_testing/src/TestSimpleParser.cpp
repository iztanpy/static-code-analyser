#include "catch.hpp"
#include "SP/sp_tokeniser/token.h"
#include "SP/source_processor.h"
#include "PKB/API/write_facade.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>
#include <set>

// Define your TokenType and Token objects here (if not already defined)
TokenType variableType = TokenType::kLiteralName;
TokenType constantType = TokenType::kLiteralInteger;
TokenType endType = TokenType::kSepSemicolon;
TokenType plusType = TokenType::kOperatorPlus;
TokenType equalType = TokenType::kEntityAssign;
TokenType readType = TokenType::kEntityRead;
Token tokenProc = Token(TokenType::kEntityProcedure, "procedure");
Token tokenProcName = Token(TokenType::kLiteralName, "poo");
Token tokenOpenBrace = Token(TokenType::kSepOpenBrace);
Token tokenCloseBrace = Token(TokenType::kSepCloseBrace);
Token tokenX = Token(variableType, "x");
Token tokenY = Token(variableType, "y");
Token tokenW = Token(variableType, "w");
Token tokenEqual = Token(equalType);
Token tokenX2 = Token(variableType, "x");
Token tokenPlus = Token(plusType);
Token token1 = Token(constantType, "1");
Token tokenEnd = Token(endType);
Token tokenRead = Token(readType);

TEST_CASE("Test Hardcore Next") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = R"(procedure Four {
        if ((a+b)||((x == 1)&&(x==2))) then {
            if (x == 1) then {
                x = 1;
                x = 2;
            }
            x = 3;
            x = 4;
            if (x == 5) then {
                x = 8;
                if (x == 6) then {
                    x = 7;
                    y = 2;
                } else {
                    x = 8;
                    x = 9;
                }
                x = 9;
            } else {
                while (x == 0) {
                    x = 1;
                    x = 2;
                }
            }
        }
        x = 2;
      })";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<int>> r = {
      {1, {2}},
      {2, {3}},
      {3, {4}},
      {4, {5}},
      {5, {6}},
      {6, {7}},
      {7, {8,15}},
      {8, {9}},
      {9, {10,12}},
      {10, {11}},
      {12, {13}},
      {11, {14}},
      {13, {14}},
      {14, {18}},
      {15, {16,18}},
      {16, {17}},
      {17, {15}},

    };

    REQUIRE(cfg::getNextStatementNumberHashmap() == r);
}

TEST_CASE("Test Sample Next") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(procedure 
                               Second
                     {
        x = 0;
        i = 5;
        while
           (i
             !=0) {
            x =
              x + 2*y;
            read a;
            i = i 
                  - 1;
        }
        if (x==1) then {
            x = x+1;
        } else {
            z = 1;
        }
        z = z 
             + x + i;
        y = z 
                + 2;
        x = x * y + z;
      })";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, std::unordered_set<int>> correct_res = {
      {1, {2}},
      {2, {3}},
      {3, {4,7}},
      {4, {5}},
      {5, {6}},
      {6, {3}},
      {7, {8,9}},
      {8, {10}},
      {9, {10}},
      {10, {11}},
      {11, {12}},
    };

    REQUIRE(cfg::getNextStatementNumberHashmap() == correct_res);
}

TEST_CASE("Test Complicated Next") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = R"(procedure Second {
        while (x==0) {
            if (x==1) 
                                            then {
                help 
                 =
                   help +1
                ; 
            } else {
                if (x == 2)
               then {
                     help =
                     help +1; 
                     help = help +2; 
                     help = help +3;
                } 
                test = test + 1; 
            }
        }
        a = a + b; 
      })";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<int>> r = {
      {1, {2,9}},
      {2, {3,4}},
      {3, {1}},
      {4, {5}},
      {5, {6}},
      {6, {7}},
      {7, {8}},
      {8, {1}},
    };

    REQUIRE(cfg::getNextStatementNumberHashmap() == r);
}

TEST_CASE("Test call sn rs.") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
    procedure p {
      call b;
      call c;
    }
    procedure b {
      read a;
    }
    procedure c {
      read a; 
    }
     )";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, std::string> callerCalleeHashmap = {
       {1, "b"},
       {2, "c"},
    };


    std::unordered_map<int, std::string> res = sourceProcessor.getVisitor().getCallStatementNumberEntityHashmap();
    REQUIRE(res == callerCalleeHashmap);
}

TEST_CASE("Test caller callee rs.") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
    procedure p {
      call b;
    }
    procedure b {
      read a;
    }
     )";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<std::string, std::unordered_set<std::string>> callerCalleeHashmap = {
       {"p", {"b"}},
    };
    std::unordered_map<std::string, std::unordered_set<std::string>> res = sourceProcessor.getVisitor().getCallerCalleeHashmap();

    REQUIRE(res == callerCalleeHashmap);
}


TEST_CASE("Test storing of procedure line numbers.") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
    procedure p {
      x = 1; 
      x = 2;
      x = 3;
    }
    procedure q {
      x = 1; 
      x = 2; 
      x = 3;
    }
     )";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<std::string, std::pair<int,int>> procedureStatementNumberHashmap = {
       {"p", {1,3}},
       {"q", {4,6}},
    };
    std::unordered_map<std::string, std::pair<int, int>> res = sourceProcessor.getVisitor().getProcedureLineNumberHashmap();
    REQUIRE(res == procedureStatementNumberHashmap);
}

TEST_CASE("Test invalid else statement at the end.") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
    procedure p {
      x = 1; 
    }
    procedure s {
      x = 1; 
    }
     )";
    sourceProcessor.processSource(simpleProgram3);
    REQUIRE(1 == 1);
}

TEST_CASE("Test follows relation one level nesting") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
    procedure p {
       x = 0;
	   if (x == 0) then {
            s = s + 1;
            x = x + 1;
	   }
       else = else + 1; 
       call c; 
    }
    procedure c {
       x = 2; 
    }
     )";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, int> followStatementNumberHashmap = {
        {1, 2},
        {3, 4},
        {2, 5},
        {5, 6}
    };
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
       {2, {3,4}},
    };

    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getVisitor().getParentStatementNumberMap();

    REQUIRE(sourceProcessor.getVisitor().getFollowStatementNumberMap() == followStatementNumberHashmap);
    REQUIRE(sourceProcessor.getVisitor().getParentStatementNumberMap() == parentStatementNumberHashmap);
}

TEST_CASE("Test simple_parser") { // line 0: x = x + 1
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade = write_facade(*pkb_ptr);
    simple_parser parser(new ASTVisitor());
    std::vector<Token> my_tokens{tokenProc, tokenProcName, tokenOpenBrace, tokenX, tokenEqual, tokenX2, tokenPlus, token1,
                                 tokenEnd, tokenCloseBrace};
    REQUIRE(parser.parse(my_tokens) == 10);
}

TEST_CASE("Test key and variable same name one level nesting") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = "procedure p { x = x + 1; x = x + 2; x = x + 3; }";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, int> followStatementNumberHashmap = {
        {1, 2},
        {2, 3},
    };
    std::unordered_map<int, int> res = sourceProcessor.getVisitor().getFollowStatementNumberMap();
    REQUIRE(sourceProcessor.getVisitor().getFollowStatementNumberMap() == followStatementNumberHashmap);
}

TEST_CASE("Test follows & parent relation two level nesting") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
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

    std::unordered_map<int, int> res = sourceProcessor.getVisitor().getFollowStatementNumberMap();
    REQUIRE(sourceProcessor.getVisitor().getFollowStatementNumberMap() == followStatementNumberHashmap);
    REQUIRE(sourceProcessor.getVisitor().getParentStatementNumberMap() == parentStatementNumberHashmap);
}


TEST_CASE("Test if else nested loop retrieval") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = "procedure p { if (number > 0) then { if (x == 1) then { x = x + 1; } } else { x = x + 1; } }";
    sourceProcessor.processSource(simpleProgram3);
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap2 = {
        {1, {2, 4}},
        {2, {3}}
    };
    std::unordered_map<int, std::unordered_set<int>> res2 = sourceProcessor.getVisitor().getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap2 == res2);
}

TEST_CASE("Test if else loop retrieval") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p { if (number > 0) then { x = x + y; } else { x = 3; } }";
    sourceProcessor.processSource(simpleProgram2);
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2, 3}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getVisitor().getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}

TEST_CASE("Test if-else-while nested retrieval") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
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
    std::unordered_map<int, std::unordered_set<int>> actualParentStatementNumberHashmap = sourceProcessor.getVisitor().getParentStatementNumberMap();

    // Check if the actual map matches the expected map
    REQUIRE(expectedParentStatementNumberHashmap == actualParentStatementNumberHashmap);

}

TEST_CASE(("Test multiple while while nested retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p {  while (number > 0) { while (x > 0) { while (a > b) {x = x + 1; } } }}";
    sourceProcessor.processSource(simpleProgram2);
    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2}},
        {2, {3}},
        {3, {4}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getVisitor().getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}

TEST_CASE(("Test SP storing of assignment statements")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram =
        "procedure p {x = x + 1; read r; while (i == 0) { read f;} } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
        { {1, "x"}, {5, "y"} });
    std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap = std::unordered_map<int,
        std::unordered_set<std::string>>({ {1, {"(x)", "(1)", "((x)+(1))"}},
                                          {5, {"(y)", "(x)", "((y)+(x))", "(((y)+(x))+(1))", "(1)"}} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
        std::unordered_set<std::string>>({ {1, {"x"}}, {3, {"i"}}, {5, {"x", "y"}} });
    REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test SP Uses: nested while and if/then/else")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram =
      "procedure p {x = x + 1; read r; while (i == 0) { if( (a==3)||(b>1) ) then{ read f;} else{ read k; }} } "
      "procedure wee { while(!(c==1)){y = y + x + 1;}}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {1, "x"}, {8, "y"} });
  std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap = std::unordered_map<int,
      std::unordered_set<std::string>>({ {1, {"(x)", "(1)", "((x)+(1))"}},
                                            {8, {"(y)", "(x)", "((y)+(x))", "(((y)+(x))+(1))", "(1)"}} });
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>({
          {1, {"x"}},
          {3, {"i"}},
          {4, {"a", "b"}},
          {7, {"c"}},
          {8, {"x", "y"}}
      });
  REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}


TEST_CASE(("Test Single While Loop Retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p {  while (number > 0)  { digit = number + 10; sum = sum + digit; } }";
    sourceProcessor.processSource(simpleProgram2);

    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2, 3}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getVisitor().getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}


TEST_CASE(("Test 2 Nested While Loops Retrieval")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p {  while (number > 0)  { while (test > 2) { test = 0; a= x+y; } } }";
    sourceProcessor.processSource(simpleProgram2);

    std::unordered_map<int, std::unordered_set<int>> parentStatementNumberHashmap = {
        {1, {2}},
        {2, {3,4}}
    };
    std::unordered_map<int, std::unordered_set<int>> res = sourceProcessor.getVisitor().getParentStatementNumberMap();
    REQUIRE(parentStatementNumberHashmap == res);
}


TEST_CASE(("Test Conditional Tokens Retrieval1")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = R"(
        procedure Main {
            while (! ((x > temp)&&( (x == temp) || ((x != temp) && ((x < temp) || ((x <= temp) && (x>= (0+(0-(0*(0/(0%(((e))))))))))))))){
                x = 1;
            }
        }
    )";
    sourceProcessor.processSource(simpleProgram2);

    std::unordered_map<int, std::unordered_set<std::string>> whileControlVarMap = {
        {1, {"x", "temp", "e"}}
    };
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getVisitor().getWhileControlVarMap();
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap =
        std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x", "temp", "e"}}});

    REQUIRE(whileControlVarMap == res);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
    REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrieval2")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram3 = "procedure q { while (    (  ((((((a + b))/c + e*j)) != 1) && (  (((((z + 2)/0)*u)) > e) || (z > n)   )) || (  !((c > nb ) || (!(z < w)))  )  )  || (!(b > n))  ) { read x; } }";
  sourceProcessor.processSource(simpleProgram3);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrievale2")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = "procedure q { while ( (a > nb) || ( c < j)  ) { read x; } }";
    sourceProcessor.processSource(simpleProgram3);
    REQUIRE(1 == 1);
}
TEST_CASE(("Test Conditional Tokens Retrieval3")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram4 = "procedure r { while (!(x == 1)) { read x; } }";
  sourceProcessor.processSource(simpleProgram4);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrieval4")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram5 = "procedure a { while ((!(x)) || (x == y)) { read x; } }";
  sourceProcessor.processSource(simpleProgram5);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrieval5")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram6 = "procedure b {   while (1>= 1%((0-1))) { read x; } }";
  sourceProcessor.processSource(simpleProgram6);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrieval6")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram7 = "procedure s { while (1>= 1%((1)) ) { read x; } }";
  sourceProcessor.processSource(simpleProgram7);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrieval7")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram8 = "procedure z { while (! ((1==0) && (1==0))) { read x; } }";
  sourceProcessor.processSource(simpleProgram8);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Conditional Tokens Retrieval8")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram9 = "procedure v { while (x>=(0+0)) { read x; } }";
  sourceProcessor.processSource(simpleProgram9);
  REQUIRE(1 == 1);
}

TEST_CASE(("Test Print Parser")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram2 = "procedure p { print k; }";
    REQUIRE(1 == 1);
}


TEST_CASE("Test design_extractor only using variables and constants") { // x = x + 1 + w
    std::shared_ptr<TNode> nodePlus2 = std::make_shared<PlusTNode>(1);
    std::shared_ptr<TNode> nodew = std::make_shared<VariableTNode>(1, tokenW.value);
    std::shared_ptr<TNode> nodeX = std::make_shared<VariableTNode>(1, tokenX.value);
    std::shared_ptr<TNode> nodeEqual = std::make_shared<AssignTNode>(1);
    std::shared_ptr<TNode> nodeX2 = std::make_shared<VariableTNode>(1, tokenX.value);
    std::shared_ptr<TNode> nodePlus = std::make_shared<PlusTNode>(1);
    std::shared_ptr<TNode> node1 = std::make_shared<ConstantTNode>(1, token1.value);

    nodeEqual->addChild(nodeX);
    nodePlus->addChild(nodeX2);
    nodePlus->addChild(node1);
    nodePlus2->addChild(nodePlus);
    nodePlus2->addChild(nodew);
    nodeEqual->addChild(nodePlus2);

    auto* visitor = new ASTVisitor();
    design_extractor::extractDesign(nodeEqual, visitor);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "w" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });

    REQUIRE(visitor->getVariables() == varSet);
    REQUIRE(visitor->getConstants() == constSet);
}

TEST_CASE(("Test SP single procedure")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"(x)", "(1)", "((x)+(1))"}} });
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}} });
    std::unordered_map<std::string, std::unordered_set<std::string>> constUseMap = std::unordered_map<std::string, std::unordered_set<std::string>>({ {"x", constSet} });

    REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
    REQUIRE(sourceProcessor.getVisitor().getVariables() == varSet);
    REQUIRE(sourceProcessor.getVisitor().getConstants() == constSet);
}

TEST_CASE(("Test SP multi procedure with keyword names")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1;} procedure procedure {y = y + x; w = w + 1;}";
    sourceProcessor.processSource(simpleProgram);


    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "w" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap =
        std::unordered_map<int, std::unordered_set<std::string>>({
          {1, {"(x)", "(1)", "((x)+(1))"}}, {2, {"(y)", "(x)", "((y)+(x))"}}, {3, {"(w)", "(1)", "((w)+(1))"}}
        });
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>({{1, "x"}, {2, "y"}, {3, "w"} });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int, std::unordered_set<std::string>>({{1, {"x"}}, {2, {"x", "y"}}, {3, {"w"}}});

    REQUIRE(sourceProcessor.getVisitor().getVariables() == varSet);
    REQUIRE(sourceProcessor.getVisitor().getConstants() == constSet);
    REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
    REQUIRE(sourceProcessor.getVisitor().getVariables() == varSet);
    REQUIRE(sourceProcessor.getVisitor().getConstants() == constSet);
}

TEST_CASE(("Test SP storing of Uses: Assign")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1;} procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);

    std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y" });
    std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
        { {1, "x"}, {2, "y"}, {3, "y"} });
    std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap = std::unordered_map<int,
        std::unordered_set<std::string>>({
                                          {1, {"((x)+(1))", "(1)", "(x)"}},
                                          {2, {"(((y)+(x))+(1))", "((y)+(x))", "(x)", "(y)", "(1)"}},
                                          {3, {"(((y)+(x))+(1))", "((y)+(x))", "(x)", "(y)", "(1)"}},
        });
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>({{1, {"x"}}, {2, {"x", "y"}}, {3, {"x", "y"}}});
    REQUIRE(sourceProcessor.getVisitor().getVariables() == varSet);
    REQUIRE(sourceProcessor.getVisitor().getConstants() == constSet);
    REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test SP to PKB <line, RHS patterns>, <line, LHS var>")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; read k;} procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap =
        std::unordered_map<int, std::unordered_set<std::string>>(
        { {1, {"(x)", "(1)", "((x)+(1))"}},
          {2, {"(x)", "(y)", "(1)", "(((y)+(x))+(1))", "((y)+(x))"}},
          {4, {"(y)", "(x)", "(1)", "(((y)+(x))+(1))", "((y)+(x))"}} });
    REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
}

TEST_CASE(("Test SP read")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    auto writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram = "procedure p {a = x + 1; y = y + x + 1; } procedure wee { y = y + x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap =
        std::unordered_map<int, std::unordered_set<std::string>>(
            {{1, {"(x)", "(1)", "((x)+(1))"}},
             {2, {"(x)", "(y)", "(1)", "(((y)+(x))+(1))", "((y)+(x))"}},
             {3, {"(y)", "(x)", "(1)", "(((y)+(x))+(1))", "((y)+(x))"}}});
    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
            {{1, "a"}, {2, "y"}, {3, "y"}});
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap();
    std::unordered_map<int, std::string> res2 = sourceProcessor.getVisitor().getUsesLineLHSMap();
    REQUIRE(res == assignLinePartialRHSPatternMap);
    REQUIRE(res2 == usesLineLHSMap);
}


TEST_CASE(("Test SP to PKB LineUsesVar")) {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade = write_facade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);

    std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; } procedure wee { w = x + 1;}";
    sourceProcessor.processSource(simpleProgram);
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap =
        std::unordered_map<int, std::unordered_set<std::string>>(
            {{1, {"x"}}, {2, {"x", "y"}}, {3, {"x"}}});
    std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getVisitor().getUsesLineRHSVarMap();
    REQUIRE(res == usesLineRHSVarMap);
}

TEST_CASE(("Test SP storing of Uses: Print")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p {x = x + 1; y = y + x + 1; print k;} procedure wee {y =y +x + 1;print kay;}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "k", "kay" });
  std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1" });
  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {1, "x"}, {2, "y"}, {4, "y"} });
  std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap = std::unordered_map<int,
      std::unordered_set<std::string>>({ {1, {"((x)+(1))", "(1)", "(x)"}},
                                            {2, {"(((y)+(x))+(1))", "((y)+(x))", "(x)", "(y)", "(1)"}},
                                            {4, {"(((y)+(x))+(1))", "((y)+(x))", "(x)", "(y)", "(1)"}}});
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>(
          {{1, {"x"}}, {2, {"x", "y"}}, {3, {"k"}}, {4, {"x", "y"}}, {5, {"kay"}}});
  REQUIRE(sourceProcessor.getVisitor().getVariables() == varSet);
  REQUIRE(sourceProcessor.getVisitor().getConstants() == constSet);
  REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test Uses: SP Assignment statement with all operators")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p {a = (x + 1) * y / 5 + 1; kay = five / 9 - var;}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_set<std::string> varSet = std::unordered_set<std::string>({ "x", "y", "a", "kay", "five", "var" });
  std::unordered_set<std::string> constSet = std::unordered_set<std::string>({ "1", "5", "9"});
  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {1, "a"}, {2, "kay"}});
  std::unordered_map<int, std::unordered_set<std::string>> assignLinePartialRHSPatternMap = std::unordered_map<int,
      std::unordered_set<std::string>>({
        {1, {"(((((x)+(1))*(y))/(5))+(1))", "((((x)+(1))*(y))/(5))", "(((x)+(1))*(y))", "((x)+(1))", "(y)", "(5)", "(x)", "(1)"}},
        {2, {"(((five)/(9))-(var))", "((five)/(9))", "(five)", "(9)", "(var)"}}});
  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
       std::unordered_set<std::string>>({{1, {"x", "y"}}, {2, {"five", "var"}}});

  std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap();

  REQUIRE(sourceProcessor.getVisitor().getVariables() == varSet);
  REQUIRE(sourceProcessor.getVisitor().getConstants() == constSet);
  REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == assignLinePartialRHSPatternMap);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}

TEST_CASE(("Test Uses: SP Assignment statement full RHS pattern")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram =
      "procedure p {a = (x + 1) * y / 5 + 1; kay = five / (9 - var);} procedure a {p = ((p * p));}";
  sourceProcessor.processSource(simpleProgram);
  std::unordered_map<int, std::string> fullRHSMap
      = std::unordered_map<int, std::string>({
        {1, "(((((x)+(1))*(y))/(5))+(1))"},
        {2, "((five)/((9)-(var)))"},
        {3, "((p)*(p))"}});
  std::unordered_map<int, std::string> fullRHSMap2 = sourceProcessor.getVisitor().getAssignLineFullRHSMap();
  REQUIRE(sourceProcessor.getVisitor().getAssignLineFullRHSMap() == fullRHSMap);
}

TEST_CASE(("Test Uses: SP Assignment statement partial RHS pattern")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = R"(
      procedure p {
        a = ((x + 1) * ((y / 5 + 1)));
        while (i == x + 1) {
          if ((i == i) && (y >= (x + 1) * wee % 5)) then {
            kay = five / 9 - var;
          }
        }
      }
      procedure a {
          p = ((p * p));
          x = y + (1 + z);
      }
  )";
  sourceProcessor.processSource(simpleProgram);
  std::unordered_map<int, std::unordered_set<std::string>> partialRHSMap =
      std::unordered_map<int, std::unordered_set<std::string>>({
        {1, {"(x)", "(1)", "(5)", "(y)", "((x)+(1))", "((y)/(5))", "(((y)/(5))+(1))",
             "(((x)+(1))*(((y)/(5))+(1)))"}},
        {4, {"(five)", "(9)", "(var)", "((five)/(9))", "(((five)/(9))-(var))"}},
        {5, {"(p)", "((p)*(p))"}},
        {6, {"(y)", "(1)", "(z)", "((1)+(z))", "((y)+((1)+(z)))"}}
      });
  std::unordered_map<int, std::unordered_set<std::string>> res = sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap();

  REQUIRE(sourceProcessor.getVisitor().getAssignLinePartialRHSPatternMap() == partialRHSMap);
}

TEST_CASE(("Test SP Statement type storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram =
      "procedure p { while (a==1) { if (i != 0) then { read f; } else { print k; a = 1 + w; }}}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, StmtEntity> statementTypesMap = std::unordered_map<int, StmtEntity>(
      {{1, StmtEntity::kWhile},
       {2, StmtEntity::kIf},
       {3, StmtEntity::kRead},
       {4, StmtEntity::kPrint},
       {5, StmtEntity::kAssign}});
  REQUIRE(sourceProcessor.getVisitor().getStatementTypesMap() == statementTypesMap);
}


TEST_CASE(("Test SP Modifies storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = "procedure p { if (i != 0) then { x = x + 1 + r; } else { read k; a = 1 + w; }}";
  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, std::string> modifiesMap =
      std::unordered_map<int, std::string>(
      {{2, "x"},
         {3, "k"},
         {4, "a"}});
  std::unordered_map<int, std::string> modifiesMap2 =sourceProcessor.getVisitor().getModifiesMap();
  REQUIRE(sourceProcessor.getVisitor().getModifiesMap() == modifiesMap);
}

TEST_CASE(("Test SP Procedures storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram =
      "procedure p { if (i != 0) then { x = x + 1 + r; }} procedure k { read r; } procedure kay { print k; a = w; }";
  sourceProcessor.processSource(simpleProgram);
  std::set<std::string> procedures = std::set<std::string>({"p","k","kay"});
  REQUIRE(sourceProcessor.getVisitor().getProcedureLabels() == procedures);
}

//TEST_CASE(("Test SP valid SIMPLE - keywords as names")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i != 0) then { else = else + 1; } call q; call procedure;} procedure jj { call alot; } ";
//  sourceProcessor.processSource(simpleProgram);
//}

TEST_CASE(("Test SP: CFG storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = R"(
      procedure Second {
        x = 0;
        i = 5;
        while (i!=0) {
            x = x + 2*y;
            call Third;
            i = i - 1;
        }
        if (x==1) then {
            x = x+1;
        }  else {
            z = 1;
        }
        z = z + x + i;
        y = z + 2;
        x = x * y + z;
      }
      procedure Third {
        a = a + 1;
        b = b * 7 + y;
        b = b * 7 + y;
        while (a > b) {
          read a;
          while (c > b) {
            read r;
          }
        }
      }
  )";
  sourceProcessor.processSource(simpleProgram);

  // check root name in map
  std::shared_ptr<cfg_node> rootSecond = cfg::getCfgNodeHashmap().at("Second");
  // check 12
  REQUIRE(rootSecond->getStmtNumberSet() == std::set<int>({1, 2}));
  REQUIRE(rootSecond->getChildren().size() == 1);
  // check 3
  REQUIRE(rootSecond->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({3}));
  REQUIRE(rootSecond->getChildren().begin()->get()->getChildren().size() == 2);
  // check 456 & 7
  std::set<int> statementNumberSet456 = std::set<int>({4, 5, 6});
  std::set<int> statementNumberSet7 = std::set<int>({7});
  std::shared_ptr<cfg_node> node7;
  std::shared_ptr<cfg_node> node456;
  bool is456Found = false;
  bool is7Found = false;
  for (auto& it : rootSecond->getChildren().begin()->get()->getChildren()) {
    if (it->getStmtNumberSet() == statementNumberSet456) {
      node456 = it;
      is456Found = !is456Found;
    } else if (it->getStmtNumberSet() == statementNumberSet7) {
      node7 = it;
      is7Found = !is7Found;
    }
  }
  REQUIRE((is456Found && is7Found));
  // check 456 -> 3
  REQUIRE(node456->getChildren().size() == 1);
  REQUIRE(node456->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({3}));
  // check 7 -> 8 & 9
  REQUIRE(node7->getChildren().size() == 2);
  std::set<int> statementNumberSet8 = std::set<int>({8});
  std::set<int> statementNumberSet9 = std::set<int>({9});
  std::shared_ptr<cfg_node> node8;
  std::shared_ptr<cfg_node> node9;
  bool is8Found = false;
  bool is9Found = false;
  for (auto& it : node7->getChildren()) {
      if (it->getStmtNumberSet() == statementNumberSet8) {
          node8 = it;
          is8Found = !is8Found;
      } else if (it->getStmtNumberSet() == statementNumberSet9) {
          node9 = it;
          is9Found = !is9Found;
      }
  }
  REQUIRE((is8Found && is9Found));
  // check 8 -> 10, 11, 12
  REQUIRE(node8->getChildren().size() == 1);
  REQUIRE(node8->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({10, 11, 12}));
  // check 9 -> 10, 11, 12
  REQUIRE(node9->getChildren().size() == 1);
  REQUIRE(node9->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({10, 11, 12}));
  // check 10, 11, 12 -> empty
  REQUIRE(node9->getChildren().begin()->get()->getChildren().size() == 0);

  // PROCEDURE THIRD CHECK
  // check root name in map
  std::shared_ptr<cfg_node> rootThird = cfg::getCfgNodeHashmap().at("Third");
  // check 13, 14, 15
  REQUIRE(rootThird->getStmtNumberSet() == std::set<int>({13, 14, 15}));
  REQUIRE(rootThird->getChildren().size() == 1);
  // check 16
  REQUIRE(rootThird->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({16}));
  REQUIRE(rootThird->getChildren().begin()->get()->getChildren().size() == 2);
  // check 16 -> 17 && 16 -> END
  std::shared_ptr<cfg_node> node17;
  std::shared_ptr<cfg_node> nodeEnd;
  bool is17Found = false;
  bool isEndFound = false;
  for (auto& it : rootThird->getChildren().begin()->get()->getChildren()) {
      if (it->getStmtNumberSet() == std::set<int>({17})) {
          node17 = it;
          is17Found = !is17Found;
      } else if (it->getStmtNumberSet().empty()) {
          nodeEnd = it;
          isEndFound = !isEndFound;
      }
  }
  REQUIRE((is17Found && isEndFound));
  // check 17 -> 18
  REQUIRE(node17->getChildren().size() == 1);
  REQUIRE(node17->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({18}));
  // check 18 -> 19 && 18 -> END
  std::shared_ptr<cfg_node> node19;
  std::shared_ptr<cfg_node> nodeEnd2;
  bool is19Found = false;
  bool isEnd2Found = false;
  for (auto& it : node17->getChildren().begin()->get()->getChildren()) {
      if (it->getStmtNumberSet() == std::set<int>({19})) {
          node19 = it;
          is19Found = !is19Found;
      } else if (it->getStmtNumberSet().empty()) {
          nodeEnd2 = it;
          isEnd2Found = !isEnd2Found;
      }
  }
  REQUIRE((is19Found && isEnd2Found));
  // check 19 -> 18
  REQUIRE(node19->getChildren().size() == 1);
  REQUIRE(node19->getChildren().begin()->get()->getStmtNumberSet() == std::set<int>({18}));
}

TEST_CASE(("Test SP: nested if/while CFG storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  auto writeFacade = write_facade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = R"(
      procedure Second {
        x = 0;
        i = 5;
        while (i!=0) {
          x = x + 2*y;
          print b;
          i = i - 1;
          if (x==1) then {
              x = x+1;
              while (else > 1) {
                if (else == 1) then {
                  x = x + 1;
                }
                else = else + 1;
              }
          } else {
              z = 1;
          }
        }
        else = else * else;
        z = z + x + i;
        y = z + 2;
        x = x * y + z;
      }
  )";
  sourceProcessor.processSource(simpleProgram);
  std::unordered_map<int, std::shared_ptr<cfg_node> > cfgLegend = cfg::getStmtNumberToCfgNodeHashmap();

  // check root name in map
  std::shared_ptr<cfg_node> rootSecond = cfg::getCfgNodeHashmap().at("Second");
  // check 12
  REQUIRE(rootSecond->getStmtNumberSet() == std::set < int > ({ 1, 2 }));
  REQUIRE(rootSecond->getChildren().size() == 1);
  REQUIRE(cfgLegend.at(1) == rootSecond);
  REQUIRE(cfgLegend.at(2) == rootSecond);
    // check 3
  REQUIRE(rootSecond->getChildren().begin()->get()->getStmtNumberSet() == std::set < int > ({ 3 }));
  REQUIRE(rootSecond->getChildren().begin()->get()->getChildren().size() == 2);
  REQUIRE(cfgLegend.at(3) == *rootSecond->getChildren().begin());
  // check 3 -> 4 5 6 and 14 15 16 17
  std::shared_ptr<cfg_node> node456;
  std::shared_ptr<cfg_node> node14151617;
  bool is456Found = false;
  bool is14151617Found = false;
  for (auto& it : rootSecond->getChildren().begin()->get()->getChildren()) {
    if (it->getStmtNumberSet() == std::set < int > ({ 4, 5, 6 })) {
      node456 = it;
      is456Found = !is456Found;
    } else if (it->getStmtNumberSet() == std::set < int > ({ 14, 15, 16, 17 })) {
      node14151617 = it;
      is14151617Found = !is14151617Found;
    }
  }
  REQUIRE((is456Found && is14151617Found));
  REQUIRE((cfgLegend.at(4) == node456 && cfgLegend.at(5) == node456 && cfgLegend.at(6) == node456));
  // check 456 -> 7
  REQUIRE(node456->getChildren().size() == 1);
  REQUIRE(node456->getChildren().begin()->get()->getStmtNumberSet() == std::set < int > ({ 7 }));
  REQUIRE(cfgLegend.at(7) == *node456->getChildren().begin());
  // check 7 -> 8 and 13
  REQUIRE(node456->getChildren().begin()->get()->getChildren().size() == 2);
  std::shared_ptr<cfg_node> node8;
  std::shared_ptr<cfg_node> node13;
  bool is8Found = false;
  bool is13Found = false;
  for (auto& it : node456->getChildren().begin()->get()->getChildren()) {
    if (it->getStmtNumberSet() == std::set < int > ({ 8 })) {
      node8 = it;
      is8Found = !is8Found;
    } else if (it->getStmtNumberSet() == std::set < int > ({ 13 })) {
      node13 = it;
      is13Found = !is13Found;
    }
  }
  REQUIRE((is8Found && is13Found));
  REQUIRE(cfgLegend.at(8) == node8);
  REQUIRE(cfgLegend.at(13) == node13);
  // check 8 -> 9
  std::shared_ptr<cfg_node> node9;
  REQUIRE(node8->getChildren().size() == 1);
  REQUIRE(node8->getChildren().begin()->get()->getStmtNumberSet() == std::set < int > ({ 9 }));
  for (auto& it : node8->getChildren()) {
    node9 = it;
  }
  REQUIRE(cfgLegend.at(9) == node9);
  // check 9 -> 10 and empty (end while)
  REQUIRE(node9->getChildren().size() == 2);
  std::shared_ptr<cfg_node> node10;
  std::shared_ptr<cfg_node> nodeEndWhile;
  bool is10Found = false;
  bool isEndWhileFound = false;
  for (auto& it : node9->getChildren()) {
    if (it->getStmtNumberSet() == std::set < int > ({ 10 })) {
      node10 = it;
      is10Found = !is10Found;
    } else if (it->getStmtNumberSet().empty()) {
      nodeEndWhile = it;
      isEndWhileFound = !isEndWhileFound;
    }
  }
  REQUIRE((is10Found && isEndWhileFound));
  REQUIRE(cfgLegend.at(10) == node10);
  // check 10 -> 11
  REQUIRE(node10->getChildren().size() == 1);
  REQUIRE(node10->getChildren().begin()->get()->getStmtNumberSet() == std::set < int > ({ 11 }));
  REQUIRE(cfgLegend.at(11) == *node10->getChildren().begin());
  // check 11 -> 12
  REQUIRE(node10->getChildren().begin()->get()->getChildren().size() == 1);
  std::shared_ptr<cfg_node> node12;
  bool is12Found = false;
  for (auto& it : node10->getChildren().begin()->get()->getChildren()) {
    if (it->getStmtNumberSet() == std::set < int > ({ 12 })) {
      node12 = it;
      is12Found = !is12Found;
    }
  }
  REQUIRE(is12Found);
  REQUIRE(node12->getChildren().size() == 1);
  REQUIRE(node12->getChildren().begin()->get()->getStmtNumberSet() == std::set < int > ({ 9 }));
  REQUIRE(node12->getChildren().begin()->get()->getChildren().size() == 2);
  bool isEndWhileAfter9 = false;
  bool is10After9 = false;
  for (auto& it : node12->getChildren().begin()->get()->getChildren()) {
    if (it->getStmtNumberSet().empty()) {
      isEndWhileAfter9 = !isEndWhileAfter9;
    } else if (it->getStmtNumberSet() == std::set < int > ({ 10 })) {
      is10After9 = !is10After9;
    }
  }
  REQUIRE((isEndWhileAfter9 && is10After9));
  REQUIRE(cfgLegend.at(12) == node12);
  // check 13 -> empty (end if)
  REQUIRE(node13->getChildren().size() == 1);
  REQUIRE(node13->getChildren().begin()->get()->getStmtNumberSet().empty());
  REQUIRE(cfgLegend.at(13) == node13);

  // check empty (end while) -> empty (end if)
  REQUIRE(nodeEndWhile->getChildren().size() == 1);
  REQUIRE(nodeEndWhile->getChildren().begin()->get()->getStmtNumberSet().empty());

  // check empty (end if) -> 3
  REQUIRE(node13->getChildren().begin()->get()->getChildren().size() == 1);
  REQUIRE(nodeEndWhile->getChildren().begin()->get()->getChildren().size() == 1);
  REQUIRE(node13->getChildren().begin()->get()->getChildren().begin()->get()->getStmtNumberSet() ==
      std::set < int > ({ 3 }));
  REQUIRE(nodeEndWhile->getChildren().begin()->get()->getChildren().begin()->get()->getStmtNumberSet() ==
      std::set < int > ({ 3 }));

  // check 14 15 16 18 -> END (no node no nothing!)
  REQUIRE(node14151617->getChildren().size() == 0);
  REQUIRE((cfgLegend.at(14) == node14151617 && cfgLegend.at(15) == node14151617 && cfgLegend.at(16) == node14151617 &&
      cfgLegend.at(17) == node14151617));
}

TEST_CASE(("Test SP Control Variable storage")) {
  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
  write_facade writeFacade(*pkb_ptr);
  SourceProcessor sourceProcessor(&writeFacade);
  std::string simpleProgram = R"(
    procedure A {
        read a;
        print b;
        while (c > d + 3) {
            if (e == f - 2) then {
                call B;
            } else {
                g = h + 4;
            }
        }
        call C;
        i = j * 2;
    }

    procedure B {
        read k;
        print l;
        m = n / 2;
        if (o > p) then {
            print q;
        } else {
            call D;
        }
        r = s - 1;
    }

    procedure C {
        print t;
        u = v + 3;
        while (w < x) {
            if (y > z) then {
                call E;
            } else {
                print aa;
            }
        }
        bb = cc * 2;
    }

    procedure D {
        read dd;
        if (ee != ff) then {
            print gg;
        } else {
            hh = ii + 5;
        }
        while (jj > kk) {
            print ll;
        }
    }

    procedure E {
        mm = nn - 3;
        print oo;
        while (pp < qq) {
            a = b + 4;
        }
        rr = ss + 4;
    }
     )";

  sourceProcessor.processSource(simpleProgram);

  std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
      { {6, "g"}, {8, "i"}, {11, "m"}, {15, "r"}, {17, "u"}, {22, "bb"}, {26, "hh"}, {29, "mm"}, {32, "a"}, {33, "rr"} });

  std::unordered_map<int, std::string> usesLineLHSMap2 = sourceProcessor.getVisitor().getUsesLineLHSMap();

  std::unordered_map<int, std::unordered_set<std::string>> whileMap = sourceProcessor.getVisitor().getWhileControlVarMap();
  std::unordered_map<int, std::unordered_set<std::string>> ifMap = sourceProcessor.getVisitor().getIfControlVarMap();
  std::unordered_map<int, std::unordered_set<std::string>> whileMaperes =
      std::unordered_map<int, std::unordered_set<std::string>>({
        {3, {"c", "d"}}, {18, {"w", "x"}},{27, {"jj", "kk"}}, {31, {"pp", "qq"}}
  });
  std::unordered_map<int, std::unordered_set<std::string>> ifMapers =
      std::unordered_map<int, std::unordered_set<std::string>>({
        {4, {"e", "f"}}, {12, {"o", "p"}}, {19, {"y", "z"}}, {24, {"ee", "ff"}}
  });

  std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap = std::unordered_map<int,
      std::unordered_set<std::string>>({ {2, {"b"}}, {3, {"c", "d"}}, {4, {"e", "f"}}, {6, {"h"}}, {8, {"j"}},
                                         {10, {"l"}}, {11, {"n"}}, {12, {"o", "p"}}, {13, {"q"}}, {15, {"s"}},
                                         {16, {"t"}}, {17, {"v"}}, {18, {"w", "x"}}, {19, {"y", "z"}}, {21, {"aa"}},
                                         {22, {"cc"}}, {24, {"ee", "ff"}}, {25, {"gg"}}, {26, {"ii"}},
                                         {27, {"jj", "kk"}}, {28, {"ll"}}, {29, {"nn"}}, {30, {"oo"}},
                                         {31, {"pp", "qq"}}, {32, {"b"}}, {33, {"ss"}} });
  REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
  REQUIRE(sourceProcessor.getVisitor().getWhileControlVarMap() == whileMaperes);
  REQUIRE(sourceProcessor.getVisitor().getIfControlVarMap() == ifMapers);
  REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}
TEST_CASE("Test complicated conditionals") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
        procedure Four {
            while (   !(  ( (a + b) || (!(   (b / (c + 3)) >= else * else  ))) && ((y) == (1 + ((3 * 1))/ d))  )  ) {
                a = 1;
            }
            if ((x % 2) + y == a + b) then {
                else = else + then;
            } else {
                print apple;
            }

        }
    )";
    sourceProcessor.processSource(simpleProgram3);

    std::unordered_map<int, std::string> usesLineLHSMap = std::unordered_map<int, std::string>(
        { {2, "a"}, {4, "else"} });

    std::unordered_map<int, std::unordered_set<std::string>> whileMap = sourceProcessor.getVisitor().getWhileControlVarMap();
    std::unordered_map<int, std::unordered_set<std::string>> ifMap = sourceProcessor.getVisitor().getIfControlVarMap();
    std::unordered_map<int, std::unordered_set<std::string>> whileMaperes =
        std::unordered_map<int, std::unordered_set<std::string>>({{{1,{"a", "b", "c", "else", "y", "d"}}}});
    std::unordered_map<int, std::unordered_set<std::string>> ifMapers =
        std::unordered_map<int, std::unordered_set<std::string>>({{3, {"x", "y", "a", "b"}}});
    std::unordered_map<int, std::unordered_set<std::string>> usesLineRHSVarMap =
        std::unordered_map<int, std::unordered_set<std::string>>({{
            {1, {"a", "b", "c", "else", "y", "d"}}, {3, {"x", "y", "a", "b"}}, {4, {"else", "then"}}, {5, {"apple"}}}});
    REQUIRE(sourceProcessor.getVisitor().getUsesLineLHSMap() == usesLineLHSMap);
    REQUIRE(sourceProcessor.getVisitor().getWhileControlVarMap() == whileMaperes);
    REQUIRE(sourceProcessor.getVisitor().getIfControlVarMap() == ifMapers);
    REQUIRE(sourceProcessor.getVisitor().getUsesLineRHSVarMap() == usesLineRHSVarMap);
}
TEST_CASE("Test valid complicated conditionals for bug fix") {
    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
    write_facade writeFacade(*pkb_ptr);
    SourceProcessor sourceProcessor(&writeFacade);
    std::string simpleProgram3 = R"(
        procedure Four {
            while (! ((x > temp)&&( (x == temp) || ((x != temp) && ((x < temp) || ((x <= temp) && (x>= (0+(0-(0*(0/(0%(((0))))))))))))))) {
                a = 1;
            }
            if ((x % 2) + y == a + b) then {
                else = else + then;
            } else {
                print apple;
            }

        }
    )";
    sourceProcessor.processSource(simpleProgram3);
    std::string simpleProgram4 = R"(
        procedure Five {
            while (! ((1==0) && (1==0))) {
                a = 1;
            }
            if ((x % 2) + y == a + b) then {
                else = else + then;
            } else {
                print apple;
            }
            if ((a == y) || ((z < 5) && ((c == 3) || (d == 4)))) then {
                a = a+ b; 
            } else {
               read apple;
            }
             while (        (1 + (2 - (3 * (4 / (5 % ((6)))))))   &&(    (x < y)||(  (x > y) && (x == y)  )   )       ) {
                test = test + 1;
            }

       

        }
    )";
    sourceProcessor.processSource(simpleProgram4);
}
// Invalid testcases - uncomment to test for errors
//TEST_CASE(("Test SP invalid SIMPLE - else after opening bracket but not any statement type")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i != 0) then { else; }} ";
//  sourceProcessor.processSource(simpleProgram);
//}
//TEST_CASE(("Test SP invalid SIMPLE - else after closing bracket but not any statement type")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i != 0) then { a = 1; } else; } ";
//  sourceProcessor.processSource(simpleProgram);
//}
//TEST_CASE(("Test SP invalid SIMPLE - else without if")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i != 0) then { else {} } } ";
//  sourceProcessor.processSource(simpleProgram);
//}
//TEST_CASE(("Test SP invalid SIMPLE - only ';' in statement lists")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i == 0) then {;} else {;;;;;} } ";
//  sourceProcessor.processSource(simpleProgram);
//}
//TEST_CASE(("Test SP invalid SIMPLE - empty statement lists")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i == 0) then {} else {} } ";
//  sourceProcessor.processSource(simpleProgram);
//}
//TEST_CASE(("Test SP invalid - integers on left of assignment statements")) {
//  std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//  auto writeFacade = write_facade(*pkb_ptr);
//  SourceProcessor sourceProcessor(&writeFacade);
//  std::string simpleProgram =
//      "procedure p { if (i != 0) then { 1 = else + 1; } call q; call procedure;} procedure jj { call alot; } ";
//  sourceProcessor.processSource(simpleProgram);
//}
//TEST_CASE("Test Multiple Procedures") {
//    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//    write_facade writeFacade(*pkb_ptr);
//    SourceProcessor sourceProcessor(&writeFacade);
//    std::string simpleProgram3 = "procedure p { x = x + 1; x = x + 2; x = x + 3; } procedure p { x = x + 1; x = x + 2; x = x + 3; }";
//    sourceProcessor.processSource(simpleProgram3);
//    // check std log to see if error is logged
//    REQUIRE(1 == 1);
//}
//TEST_CASE("Test Calling Non-existent Procedure") {
//    std::unique_ptr<PKB> pkb_ptr = std::make_unique<PKB>();
//    write_facade writeFacade(*pkb_ptr);
//    SourceProcessor sourceProcessor(&writeFacade);
//    std::string simpleProgram3 = "procedure one { x = x + 2; call two; } procedure two { call one; call q; }";
//    sourceProcessor.processSource(simpleProgram3);
//    // check std log to see if error is logged
//    REQUIRE(1 == 1);
//}