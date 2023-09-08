
#include "SP/SPTokeniser.h"
#include "SP/Token.h"
#include "SP/TokenTypes.h"
#include "catch.hpp"

using namespace std;

TEST_CASE(("Test Simple Program")) {
    SPtokeniser tokeniser;

    string simpleProgram = "count = 0; while ((x != 0 ) && (y != 0)){call readPoint; }";
    std::vector<struct Token> tokens_simple = tokeniser.tokenise(simpleProgram);

    // check type 
    REQUIRE(tokens_simple[0].tokenType == kLiteralName);
    REQUIRE(tokens_simple[1].tokenType == kEntityAssign);
    REQUIRE(tokens_simple[2].tokenType == kLiteralInteger);
    REQUIRE(tokens_simple[3].tokenType == kEntityWhile);
    REQUIRE(tokens_simple[4].tokenType == kSepOpenParen);
    REQUIRE(tokens_simple[5].tokenType == kSepOpenParen);
    REQUIRE(tokens_simple[6].tokenType == kLiteralName);
    REQUIRE(tokens_simple[7].tokenType == kOperatorNotEqual);
    REQUIRE(tokens_simple[8].tokenType == kLiteralInteger);
    REQUIRE(tokens_simple[9].tokenType == kSepCloseParen);
    REQUIRE(tokens_simple[10].tokenType == kOperatorLogicalAnd);
    REQUIRE(tokens_simple[11].tokenType == kSepOpenParen);
    REQUIRE(tokens_simple[12].tokenType == kLiteralName);
    REQUIRE(tokens_simple[13].tokenType == kOperatorNotEqual);
    REQUIRE(tokens_simple[14].tokenType == kLiteralInteger);
    REQUIRE(tokens_simple[15].tokenType == kSepCloseParen);
    REQUIRE(tokens_simple[16].tokenType == kSepCloseParen);
    REQUIRE(tokens_simple[17].tokenType == kEntityCall);
    REQUIRE(tokens_simple[18].tokenType == kLiteralName);

    REQUIRE(tokens_simple[0].lineNumber == 1);
    REQUIRE(tokens_simple[1].lineNumber == 1);
    REQUIRE(tokens_simple[2].lineNumber == 1);

    REQUIRE(tokens_simple[3].lineNumber == 2);
    REQUIRE(tokens_simple[4].lineNumber == 2);
    REQUIRE(tokens_simple[5].lineNumber == 2);
    REQUIRE(tokens_simple[6].lineNumber == 2);
    REQUIRE(tokens_simple[7].lineNumber == 2);
    REQUIRE(tokens_simple[8].lineNumber == 2);
    REQUIRE(tokens_simple[9].lineNumber == 2);
    REQUIRE(tokens_simple[10].lineNumber == 2);
    REQUIRE(tokens_simple[11].lineNumber == 2);
    REQUIRE(tokens_simple[12].lineNumber == 2);
    REQUIRE(tokens_simple[13].lineNumber == 2);
    REQUIRE(tokens_simple[14].lineNumber == 2);
    REQUIRE(tokens_simple[15].lineNumber == 2);
    REQUIRE(tokens_simple[16].lineNumber == 2);

    REQUIRE(tokens_simple[17].value == "call");

    REQUIRE(tokens_simple[17].lineNumber == 3);
    REQUIRE(tokens_simple[18].lineNumber == 3);

}


TEST_CASE("Test Line Numbers") {
    SPtokeniser tokeniser;
    std::vector<struct Token> tokens = tokeniser.tokenise("cenX; cenY; cenA;");
    REQUIRE(tokens[0].value == "cenX");
    REQUIRE(tokens[1].value == "cenY");
    REQUIRE(tokens[2].value == "cenA");

    REQUIRE(tokens[0].lineNumber == 1);
    REQUIRE(tokens[1].lineNumber == 2);
    REQUIRE(tokens[2].lineNumber == 3);
}


TEST_CASE("Test Delimiters") {
    SPtokeniser tokeniser;
    std::vector<struct Token> tokens = tokeniser.tokenise("cenX;");
    REQUIRE(tokens[0].tokenType == kLiteralName);
    REQUIRE(tokens[0].value == "cenX");
}

TEST_CASE("Test Regex") {
    SPtokeniser tokeniser; 

    // Regular Statement
    std::vector<struct Token> tokens_normal = tokeniser.tokenise("a = b + y;");
    REQUIRE(tokens_normal[0].tokenType == kLiteralName);
    REQUIRE(tokens_normal[0].value == "a");
    REQUIRE(tokens_normal[1].tokenType == kEntityAssign);
    REQUIRE(tokens_normal[1].value == "=");
    REQUIRE(tokens_normal[2].tokenType == kLiteralName);
    REQUIRE(tokens_normal[2].value == "b");
    REQUIRE(tokens_normal[3].tokenType == kOperatorPlus);
    REQUIRE(tokens_normal[3].value == "+");
    REQUIRE(tokens_normal[4].tokenType == kLiteralName);
    REQUIRE(tokens_normal[4].value == "y");
    REQUIRE(end(tokens_normal) - begin(tokens_normal) == 5);


    // Literal Rules
    REQUIRE_THROWS_AS(tokeniser.tokenise("123myInt"), std::runtime_error); // expect an error to be thrown
    std::vector<struct Token> tokens_literals = tokeniser.tokenise("123 test;");
    REQUIRE(tokens_literals[0].tokenType == kLiteralInteger);
    //REQUIRE(tokens_literals[1].tokenType == kLiteralName);

    // Entities
    std::vector<struct Token> tokens_entity = tokeniser.tokenise("if while else read procedure print call;");
    REQUIRE(tokens_entity[0].tokenType == kEntityIf);
    REQUIRE(tokens_entity[1].tokenType == kEntityWhile);
    REQUIRE(tokens_entity[2].tokenType == kEntityElse);
    REQUIRE(tokens_entity[3].tokenType == kEntityRead);
    REQUIRE(tokens_entity[4].tokenType == kEntityProcedure);
    REQUIRE(tokens_entity[5].tokenType == kEntityPrint);
    REQUIRE(tokens_entity[6].tokenType == kEntityCall);
    REQUIRE(end(tokens_entity) - begin(tokens_entity) == 7);



    // Separators
    std::vector<struct Token> tokens_separators = tokeniser.tokenise(",;");
    /*REQUIRE(tokens_separators[1].tokenType == kSepOpenParen);
    REQUIRE(tokens_separators[2].tokenType == kSepCloseParen);
    REQUIRE(tokens_separators[3].tokenType == kSepOpenBrace);
    REQUIRE(tokens_separators[4].tokenType == kSepCloseBrace);*/
    REQUIRE(tokens_separators[0].tokenType == kSepComma);
    REQUIRE(end(tokens_separators) - begin(tokens_separators) == 1);


    // Operators
    std::vector<struct Token> tokens_operators = tokeniser.tokenise("+ - * / % == != < <= > >= && || !");
    REQUIRE(tokens_operators[0].tokenType == kOperatorPlus);
    REQUIRE(tokens_operators[1].tokenType == kOperatorMinus);
    REQUIRE(tokens_operators[2].tokenType == kOperatorMultiply);
    REQUIRE(tokens_operators[3].tokenType == kOperatorDivide);
    REQUIRE(tokens_operators[4].tokenType == kOperatorMod);
    REQUIRE(tokens_operators[5].tokenType == kOperatorEqual);
    REQUIRE(tokens_operators[6].tokenType == kOperatorNotEqual);
    REQUIRE(tokens_operators[7].tokenType == kOperatorLess);
    REQUIRE(tokens_operators[8].tokenType == kOperatorLessEqual);
    REQUIRE(tokens_operators[9].tokenType == kOperatorGreater);
    REQUIRE(tokens_operators[10].tokenType == kOperatorGreaterEqual);
    REQUIRE(tokens_operators[11].tokenType == kOperatorLogicalAnd);
    REQUIRE(tokens_operators[12].tokenType == kOperatorLogicalOr);
    REQUIRE(tokens_operators[13].tokenType == kOperatorLogicalNot);
    REQUIRE(end(tokens_operators) - begin(tokens_operators) == 14);
   
}


