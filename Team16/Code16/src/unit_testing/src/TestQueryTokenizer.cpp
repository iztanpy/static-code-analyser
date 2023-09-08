#include <catch.hpp>
#include "qps/query_tokenizer.h"

TEST_CASE("Tokenizer can tokenize simple select clause with variable") {
    std::string query = "variable v; Select v";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "v");
}

TEST_CASE("Tokenizer can tokenize simple select clause with assignment") {
    std::string query = "assign a; Select a";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "assign");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "a");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "a");
}

TEST_CASE("Tokenizer can tokenize simple select clause with stmt") {
    std::string query = "stmt s; Select s";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "stmt");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "s");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "s");
}

TEST_CASE("Tokenizer can tokenize simple select clause with read") {
    std::string query = "read r; Select r";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "read");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "r");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "r");
}

TEST_CASE("Tokenizer can tokenize simple select clause with print") {
    std::string query = "print p; Select p";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "print");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "p");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "p");
}

TEST_CASE("Tokenizer can tokenize simple select clause with while") {
    std::string query = "while w; Select w";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "while");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "w");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "w");
}

TEST_CASE("Tokenizer can tokenize simple select clause with if") {
    std::string query = "if w; Select w";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "if");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "w");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "w");
}

TEST_CASE("Tokenizer can tokenize simple select clause with constant") {
    std::string query = "constant c; Select c";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "constant");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "c");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "c");
}

TEST_CASE("Tokenizer can tokenize simple select clause with procedure") {
    std::string query = "procedure p; Select p";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "procedure");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "p");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "p");
}

TEST_CASE("Tokenizer can tokenize simple select clause with multiple variable declarations") {
    std::string query = "variable v1, v2; Select v1";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v1");
    REQUIRE(tokens[2].type == TokenType::SYNONYM);
    REQUIRE(tokens[2].text == "v2");
    REQUIRE(tokens[3].type == TokenType::SELECT);
    REQUIRE(tokens[3].text == "Select");
    REQUIRE(tokens[4].type == TokenType::SYNONYM);
    REQUIRE(tokens[4].text == "v1");
}

TEST_CASE("Tokenizer can tokenize simple select clause with variable + multiple declarations") {
    std::string query = "variable v; assign a; Select v";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::DECLARATION);
    REQUIRE(tokens[2].text == "assign");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "a");
    REQUIRE(tokens[4].type == TokenType::SELECT);
    REQUIRE(tokens[4].text == "Select");
    REQUIRE(tokens[5].type == TokenType::SYNONYM);
    REQUIRE(tokens[5].text == "v");
}

TEST_CASE("Tokenizer can tokenize select clause with new lines") {
    std::string query = "variable\nv\n;\nSelect\nv";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "v");
}

TEST_CASE("Tokenizer can tokenize select clause with multiple spaces") {
    std::string query = "variable      v   ;   Select  v  ";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "v");
}

TEST_CASE("Tokenizer can tokenize select clause with tabs") {
    std::string query = "variable\tv\t;\tSelect\tv";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "v");
}

TEST_CASE("Tokenizer can tokenize simple select + such that clause with variable") {
    std::string query = "variable v; Select v such that";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "v");
    REQUIRE(tokens[4].type == TokenType::SUCH_THAT);
    REQUIRE(tokens[4].text == "such");
    REQUIRE(tokens[5].type == TokenType::SUCH_THAT);
    REQUIRE(tokens[5].text == "that");
}

TEST_CASE("Tokenizer can tokenize simple select + pattern clause with variable") {
    std::string query = "variable v; Select v pattern";
    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
    REQUIRE(tokens[0].type == TokenType::DECLARATION);
    REQUIRE(tokens[0].text == "variable");
    REQUIRE(tokens[1].type == TokenType::SYNONYM);
    REQUIRE(tokens[1].text == "v");
    REQUIRE(tokens[2].type == TokenType::SELECT);
    REQUIRE(tokens[2].text == "Select");
    REQUIRE(tokens[3].type == TokenType::SYNONYM);
    REQUIRE(tokens[3].text == "v");
    REQUIRE(tokens[4].type == TokenType::PATTERN);
    REQUIRE(tokens[4].text == "pattern");
}