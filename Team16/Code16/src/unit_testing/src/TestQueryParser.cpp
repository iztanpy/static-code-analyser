#include <catch.hpp>
#include "qps/query_tokenizer.h"
#include "qps/query_parser.h"

TEST_CASE("Query Parser can extract declarations from tokens") {
    std::vector<QueryToken> tokens = {
            {"variable", TokenType::DECLARATION},
            {"v1",        TokenType::SYNONYM},
            {"v2", TokenType::SYNONYM},
            {"assign", TokenType::DECLARATION},
            {"a1",        TokenType::SYNONYM},
            {"a2", TokenType::SYNONYM},
            {"Select",   TokenType::SELECT},
            {"v2",        TokenType::SYNONYM}
    };
    std::vector<Declaration> declarations = QueryParser::ExtractDeclarations(tokens);
    REQUIRE(declarations.size() == 4);
    REQUIRE(declarations[0].synonym == "v1");
    REQUIRE(declarations[0].design_entity == DesignEntity::VARIABLE);
    REQUIRE(declarations[1].synonym == "v2");
    REQUIRE(declarations[1].design_entity == DesignEntity::VARIABLE);
    REQUIRE(declarations[2].synonym == "a1");
    REQUIRE(declarations[2].design_entity == DesignEntity::ASSIGN);
    REQUIRE(declarations[3].synonym == "a2");
    REQUIRE(declarations[3].design_entity == DesignEntity::ASSIGN);
}

TEST_CASE("Query Parser can extract select tokens") {
    std::vector<QueryToken> tokens = {
            {"variable", TokenType::DECLARATION},
            {"v",        TokenType::SYNONYM},
            {"Select",   TokenType::SELECT},
            {"v",        TokenType::SYNONYM}
    };

    std::vector<QueryToken> selectTokens = QueryParser::ExtractSelectTokens(tokens);
    REQUIRE(selectTokens.size() == 1);
    REQUIRE(selectTokens[0].text == "v");
    REQUIRE(selectTokens[0].type == TokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract multiple select tokens") {
    std::vector<QueryToken> tokens = {
            {"variable", TokenType::DECLARATION},
            {"v", TokenType::SYNONYM},
            {"assign", TokenType::DECLARATION},
            {"a",TokenType::SYNONYM},
            {"Select", TokenType::SELECT},
            {"v", TokenType::SYNONYM},
            {"a", TokenType::SYNONYM},
    };

    std::vector<QueryToken> selectTokens = QueryParser::ExtractSelectTokens(tokens);
    REQUIRE(selectTokens.size() == 2);
    REQUIRE(selectTokens[0].text == "v");
    REQUIRE(selectTokens[0].type == TokenType::SYNONYM);
    REQUIRE(selectTokens[1].text == "a");
    REQUIRE(selectTokens[1].type == TokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract select clause") {
    std::vector<QueryToken> selectTokens = {
            {"v1", TokenType::SYNONYM},
            {"v2", TokenType::SYNONYM},
        };
    std::vector<Declaration> declarations = {
            {"v1", DesignEntity::VARIABLE},
            {"v2", DesignEntity::VARIABLE}
    };
    std::vector<SelectClause> selectClause = QueryParser::ExtractSelectClauses(selectTokens, declarations);
    REQUIRE(selectClause.size() == 2);
    REQUIRE(selectClause[0].design_entity == DesignEntity::VARIABLE);
    REQUIRE(selectClause[0].synonym == "v1");
}

TEST_CASE("Query Parser can return a parsed query") {
    std::vector<QueryToken> tokens = {
            {"variable", TokenType::DECLARATION},
            {"v1",        TokenType::SYNONYM},
            {"v2", TokenType::SYNONYM},
            {"assign", TokenType::DECLARATION},
            {"a1",        TokenType::SYNONYM},
            {"a2", TokenType::SYNONYM},
            {"Select",   TokenType::SELECT},
            {"v2",        TokenType::SYNONYM}
    };
    ParsedQuery parsedQuery = QueryParser::ParseTokenizedQuery(tokens);
    SelectClause expected;
    expected.synonym = "v2";
    expected.design_entity = DesignEntity::VARIABLE;
    REQUIRE(parsedQuery.select.equals(expected));
}