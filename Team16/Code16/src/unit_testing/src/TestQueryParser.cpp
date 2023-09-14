#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/query_parser/query_parser.h"

TEST_CASE("Query Parser can extract declarations from tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v1", PQLTokenType::SYNONYM},
      {"v2", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a1", PQLTokenType::SYNONYM},
      {"a2", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v2", PQLTokenType::SYNONYM}
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
      {"variable", PQLTokenType::DECLARATION},
      {"v", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v", PQLTokenType::SYNONYM},
      {"such that", PQLTokenType::SUCH_THAT},
      {"Uses", PQLTokenType::RELREF},
  };

  std::vector<QueryToken> selectTokens = QueryParser::ExtractSelectTokens(tokens);
  REQUIRE(selectTokens.size() == 1);
  REQUIRE(selectTokens[0].text == "v");
  REQUIRE(selectTokens[0].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract multiple select tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v", PQLTokenType::SYNONYM},
      {"a", PQLTokenType::SYNONYM},
  };

  std::vector<QueryToken> selectTokens = QueryParser::ExtractSelectTokens(tokens);
  REQUIRE(selectTokens.size() == 2);
  REQUIRE(selectTokens[0].text == "v");
  REQUIRE(selectTokens[0].type == PQLTokenType::SYNONYM);
  REQUIRE(selectTokens[1].text == "a");
  REQUIRE(selectTokens[1].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract select clause") {
  std::vector<QueryToken> selectTokens = {
      {"v1", PQLTokenType::SYNONYM},
      {"v2", PQLTokenType::SYNONYM},
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

TEST_CASE("Query Parser can extract such that tokens") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v", PQLTokenType::SYNONYM},
      {"such that", PQLTokenType::SUCH_THAT},
      {"Uses", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };

  std::vector<QueryToken> suchThatTokens = QueryParser::ExtractSuchThatTokens(tokens);
  REQUIRE(suchThatTokens.size() == 3);
  REQUIRE(suchThatTokens[0].text == "Uses");
  REQUIRE(suchThatTokens[0].type == PQLTokenType::RELREF);
  REQUIRE(suchThatTokens[1].text == "a");
  REQUIRE(suchThatTokens[1].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[2].text == "v");
  REQUIRE(suchThatTokens[2].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can extract multiple such that tokens") {
  std::string sample_query = "variable v; assign a; stmt s; Select v such that Uses(a, v) Parent(s, a)";
  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(sample_query);

  std::vector<QueryToken> suchThatTokens = QueryParser::ExtractSuchThatTokens(tokens);
  REQUIRE(suchThatTokens.size() == 6);
  REQUIRE(suchThatTokens[0].text == "Uses");
  REQUIRE(suchThatTokens[0].type == PQLTokenType::RELREF);
  REQUIRE(suchThatTokens[1].text == "a");
  REQUIRE(suchThatTokens[1].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[2].text == "v");
  REQUIRE(suchThatTokens[2].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[3].text == "Parent");
  REQUIRE(suchThatTokens[3].type == PQLTokenType::RELREF);
  REQUIRE(suchThatTokens[4].text == "s");
  REQUIRE(suchThatTokens[4].type == PQLTokenType::SYNONYM);
  REQUIRE(suchThatTokens[5].text == "a");
  REQUIRE(suchThatTokens[5].type == PQLTokenType::SYNONYM);
}

TEST_CASE("Query Parser can return a parsed query") {
  std::vector<QueryToken> tokens = {
      {"variable", PQLTokenType::DECLARATION},
      {"v1", PQLTokenType::SYNONYM},
      {"v2", PQLTokenType::SYNONYM},
      {"assign", PQLTokenType::DECLARATION},
      {"a1", PQLTokenType::SYNONYM},
      {"a2", PQLTokenType::SYNONYM},
      {"Select", PQLTokenType::SELECT},
      {"v2", PQLTokenType::SYNONYM}
  };
  ParsedQuery parsedQuery = QueryParser::ParseTokenizedQuery(tokens);
  SelectClause expected;
  expected.synonym = "v2";
  expected.design_entity = DesignEntity::VARIABLE;
  REQUIRE(parsedQuery.select.equals(expected));
}
