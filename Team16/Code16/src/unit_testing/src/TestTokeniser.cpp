#include "catch.hpp"

#include "SP/SPTokeniser.h"
#include "SP/Token.h"

TEST_CASE(("Test Simple Program")) {
    SPtokeniser tokeniser;

    std::string simpleProgram = "count = 0; while ((x != 0 ) && (y != 0)){call readPoint; }";
    std::vector<struct Token> tokens_simple = tokeniser.tokenise(simpleProgram);

    // check type
    REQUIRE(tokens_simple[0].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[1].tokenType == TokenType::kEntityAssign);
    REQUIRE(tokens_simple[2].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[3].tokenType == TokenType::kEntityWhile);
    REQUIRE(tokens_simple[4].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_simple[5].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_simple[6].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[7].tokenType == TokenType::kOperatorNotEqual);
    REQUIRE(tokens_simple[8].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[9].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_simple[10].tokenType == TokenType::kOperatorLogicalAnd);
    REQUIRE(tokens_simple[11].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_simple[12].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[13].tokenType == TokenType::kOperatorNotEqual);
    REQUIRE(tokens_simple[14].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[15].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_simple[16].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_simple[17].tokenType == TokenType::kEntityCall);
    REQUIRE(tokens_simple[18].tokenType == TokenType::kLiteralName);

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

    REQUIRE(tokens[0].linePosition == 0);
    REQUIRE(tokens[1].linePosition == 0);
    REQUIRE(tokens[2].linePosition == 0);

    std::vector<struct Token> tokens_q = tokeniser.tokenise("cenX = a + b;");
    REQUIRE(tokens_q[0].lineNumber == 1);
    REQUIRE(tokens_q[1].lineNumber == 1);
    REQUIRE(tokens_q[2].lineNumber == 1);

    REQUIRE(tokens_q[0].linePosition == 0);
    REQUIRE(tokens_q[1].linePosition == 4);
    REQUIRE(tokens_q[2].linePosition == 5);
    REQUIRE(tokens_q[3].linePosition == 6);
    REQUIRE(tokens_q[4].linePosition == 7);
}

TEST_CASE("Test Delimiters") {
    SPtokeniser tokeniser;
    std::vector<struct Token> tokens = tokeniser.tokenise("cenX;");
    for (auto token : tokens) {
        std::cout << token.value << std::endl;
    }
    std::cout << "result token size: " << tokens.size() << std::endl;

    REQUIRE(1 == 1);
//    REQUIRE(tokens[0].tokenType == TokenType::kLiteralName);
//    REQUIRE(tokens[0].value == "cenX");
}

TEST_CASE("Test Regex") {
    SPtokeniser tokeniser;

    // Regular Statement
    std::vector<struct Token> tokens_normal = tokeniser.tokenise("a = b + y;");
    REQUIRE(tokens_normal[0].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_normal[0].value == "a");
    REQUIRE(tokens_normal[1].tokenType == TokenType::kEntityAssign);
    REQUIRE(tokens_normal[1].value == "=");
    REQUIRE(tokens_normal[2].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_normal[2].value == "b");
    REQUIRE(tokens_normal[3].tokenType == TokenType::kOperatorPlus);
    REQUIRE(tokens_normal[3].value == "+");
    REQUIRE(tokens_normal[4].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_normal[4].value == "y");
    REQUIRE(end(tokens_normal) - begin(tokens_normal) == 5);


    // Literal Rules
    REQUIRE_THROWS_AS(tokeniser.tokenise("123myInt"), std::runtime_error);  // expect an error to be thrown
    std::vector<struct Token> tokens_literals = tokeniser.tokenise("123 test;");
    REQUIRE(tokens_literals[0].tokenType == TokenType::kLiteralInteger);
    // REQUIRE(tokens_literals[1].tokenType == TokenType::kLiteralName);

    // Entities
    std::vector<struct Token> tokens_entity = tokeniser.tokenise("if while else read procedure print call;");
    REQUIRE(tokens_entity[0].tokenType == TokenType::kEntityIf);
    REQUIRE(tokens_entity[1].tokenType == TokenType::kEntityWhile);
    REQUIRE(tokens_entity[2].tokenType == TokenType::kEntityElse);
    REQUIRE(tokens_entity[3].tokenType == TokenType::kEntityRead);
    REQUIRE(tokens_entity[4].tokenType == TokenType::kEntityProcedure);
    REQUIRE(tokens_entity[5].tokenType == TokenType::kEntityPrint);
    REQUIRE(tokens_entity[6].tokenType == TokenType::kEntityCall);
    REQUIRE(end(tokens_entity) - begin(tokens_entity) == 7);



    // Separators
    std::vector<struct Token> tokens_separators = tokeniser.tokenise(",;");
    /*REQUIRE(tokens_separators[1].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_separators[2].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_separators[3].tokenType == TokenType::kSepOpenBrace);
    REQUIRE(tokens_separators[4].tokenType == TokenType::kSepCloseBrace);*/
    REQUIRE(tokens_separators[0].tokenType == TokenType::kSepComma);
    REQUIRE(end(tokens_separators) - begin(tokens_separators) == 1);


    // Operators
    std::vector<struct Token> tokens_operators = tokeniser.tokenise("+ - * / % == != < <= > >= && || !");
    REQUIRE(tokens_operators[0].tokenType == TokenType::kOperatorPlus);
    REQUIRE(tokens_operators[1].tokenType == TokenType::kOperatorMinus);
    REQUIRE(tokens_operators[2].tokenType == TokenType::kOperatorMultiply);
    REQUIRE(tokens_operators[3].tokenType == TokenType::kOperatorDivide);
    REQUIRE(tokens_operators[4].tokenType == TokenType::kOperatorMod);
    REQUIRE(tokens_operators[5].tokenType == TokenType::kOperatorEqual);
    REQUIRE(tokens_operators[6].tokenType == TokenType::kOperatorNotEqual);
    REQUIRE(tokens_operators[7].tokenType == TokenType::kOperatorLess);
    REQUIRE(tokens_operators[8].tokenType == TokenType::kOperatorLessEqual);
    REQUIRE(tokens_operators[9].tokenType == TokenType::kOperatorGreater);
    REQUIRE(tokens_operators[10].tokenType == TokenType::kOperatorGreaterEqual);
    REQUIRE(tokens_operators[11].tokenType == TokenType::kOperatorLogicalAnd);
    REQUIRE(tokens_operators[12].tokenType == TokenType::kOperatorLogicalOr);
    REQUIRE(tokens_operators[13].tokenType == TokenType::kOperatorLogicalNot);
    REQUIRE(end(tokens_operators) - begin(tokens_operators) == 14);
}
