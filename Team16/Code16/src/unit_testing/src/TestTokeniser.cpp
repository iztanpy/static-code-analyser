#include "catch.hpp"

#include "SP/sp_tokeniser/sp_tokeniser.h"
#include "SP/sp_tokeniser/token.h"

using namespace std;

TEST_CASE(("Test Simple Program")) {
    SPtokeniser tokeniser;

    string simpleProgram = "count = 0; while ((x != 0 ) && (y != 0)){call readPoint; }";
    std::vector<struct Token> tokens_simple = tokeniser.tokenise(simpleProgram);

    // check type
    REQUIRE(tokens_simple[0].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[1].tokenType == TokenType::kEntityAssign);
    REQUIRE(tokens_simple[2].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[3].tokenType == TokenType::kSepSemicolon);

    REQUIRE(tokens_simple[4].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[5].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_simple[6].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_simple[7].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[8].tokenType == TokenType::kOperatorNotEqual);
    REQUIRE(tokens_simple[9].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[10].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_simple[11].tokenType == TokenType::kOperatorLogicalAnd);
    REQUIRE(tokens_simple[12].tokenType == TokenType::kSepOpenParen);
    REQUIRE(tokens_simple[13].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[14].tokenType == TokenType::kOperatorNotEqual);
    REQUIRE(tokens_simple[15].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[16].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_simple[17].tokenType == TokenType::kSepCloseParen);
    REQUIRE(tokens_simple[18].tokenType == TokenType::kSepOpenBrace);
    REQUIRE(tokens_simple[19].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[20].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[21].tokenType == TokenType::kSepSemicolon);
}

TEST_CASE("Test Delimiters") {
    SPtokeniser tokeniser;
    std::vector<struct Token> tokens = tokeniser.tokenise("cenX;");

    REQUIRE(tokens[0].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens[1].tokenType == TokenType::kSepSemicolon);
    REQUIRE(tokens[0].value == "cenX");
    REQUIRE(tokens[1].value == ";");
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
    REQUIRE(tokens_normal[5].tokenType == TokenType::kSepSemicolon);
    REQUIRE(tokens_normal[5].value == ";");
    REQUIRE(end(tokens_normal) - begin(tokens_normal) == 6);
 

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

// uncomment to check termination
TEST_CASE(("Test syntax error")) {
    SPtokeniser tokeniser;

    string missingBraces = "procedure p  x = 1; y = 1 + 2 + 3; } procedure x { read r;} ";
    string extraBraces = "procedure p  x = 1; y = 1 + 2 + 3; } }}}procedure x { read r;}  }}}{ ";
    string missingParan = "procedure p  if(x = 1{ y = 1 + 2 + 3;} } procedure x { read r;} ";
    string extraParan = "procedure p  if(x = 1)))))){ y = 1 + 2 + 3;} } procedure x { read r;} ";

//    std::vector<struct Token> tokens_simple1 = tokeniser.tokenise(missingBraces);
//    std::vector<struct Token> tokens_simple2 = tokeniser.tokenise(extraBraces);
//    std::vector<struct Token> tokens_simple3 = tokeniser.tokenise(missingParan);
//    std::vector<struct Token> tokens_simple4 = tokeniser.tokenise(extraParan);
//    std::vector<struct Token> tokens_simple = tokeniser.tokenise(missingBraces);
}

TEST_CASE(("Test procedure")) {
    SPtokeniser tokeniser;

    string simpleProgram = "procedure p { x = 1; y = 1 + 2 + 3; } procedure x { read r;} ";
    std::vector<struct Token> tokens_simple = tokeniser.tokenise(simpleProgram);

    // check type
    REQUIRE(tokens_simple[0].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[1].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[2].tokenType == TokenType::kSepOpenBrace);

    REQUIRE(tokens_simple[3].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[4].tokenType == TokenType::kEntityAssign);
    REQUIRE(tokens_simple[5].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[6].tokenType == TokenType::kSepSemicolon);

    REQUIRE(tokens_simple[7].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[8].tokenType == TokenType::kEntityAssign);
    REQUIRE(tokens_simple[9].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[10].tokenType == TokenType::kOperatorPlus);
    REQUIRE(tokens_simple[11].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[12].tokenType == TokenType::kOperatorPlus);
    REQUIRE(tokens_simple[13].tokenType == TokenType::kLiteralInteger);
    REQUIRE(tokens_simple[14].tokenType == TokenType::kSepSemicolon);
    REQUIRE(tokens_simple[15].tokenType == TokenType::kSepCloseBrace);

 
    REQUIRE(tokens_simple[16].tokenType == TokenType::kLiteralName);

    REQUIRE(tokens_simple[17].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[18].tokenType == TokenType::kSepOpenBrace);

    REQUIRE(tokens_simple[19].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[20].tokenType == TokenType::kLiteralName);
    REQUIRE(tokens_simple[21].tokenType == TokenType::kSepSemicolon);
    REQUIRE(tokens_simple[22].tokenType == TokenType::kSepCloseBrace);
    REQUIRE(tokens_simple.size() == 23);
}