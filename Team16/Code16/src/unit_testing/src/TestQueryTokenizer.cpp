#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/qps_errors/qps_error.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/string_utils.h"
//
//TEST_CASE("Tokenizer can tokenize simple select clause with variable") {
//  std::string query = "variable v; Select v";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with assignment") {
//  std::string query = "assign a; Select a";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "assign");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "a");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "a");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with stmt") {
//  std::string query = "stmt s; Select s";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "stmt");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "s");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "s");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with read") {
//  std::string query = "read r; Select r";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "read");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "r");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "r");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with print") {
//  std::string query = "print p; Select p";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "print");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "p");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "p");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with while") {
//  std::string query = "while w; Select w";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "while");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "w");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "w");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with if") {
//  std::string query = "if w; Select w";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "if");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "w");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "w");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with constant") {
//  std::string query = "constant c; Select c";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "constant");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "c");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "c");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with procedure") {
//  std::string query = "procedure p; Select p";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "procedure");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "p");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "p");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with multiple variable declarations") {
//  std::string query = "variable v1, v2; Select v1";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v1");
//  REQUIRE(tokens[2].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[2].text == "v2");
//  REQUIRE(tokens[3].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[3].text == "Select");
//  REQUIRE(tokens[4].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[4].text == "v1");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select clause with variable + multiple declarations") {
//  std::string query = "variable v; assign a; Select v";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[2].text == "assign");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "a");
//  REQUIRE(tokens[4].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[4].text == "Select");
//  REQUIRE(tokens[5].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[5].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize select clause with new lines") {
//  std::string query = "variable\nv\n;\nSelect\nv";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize select clause with multiple spaces") {
//  std::string query = "variable      v   ;   Select  v  ";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize select clause with tabs") {
//  std::string query = "variable\tv\t;\tSelect\tv";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + such that clause with variable") {
//  std::string query = "variable v; assign a; Select a such that Uses(a, v)";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 11);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[2].text == "assign");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "a");
//  REQUIRE(tokens[4].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[4].text == "Select");
//  REQUIRE(tokens[5].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[5].text == "a");
//  REQUIRE(tokens[6].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[6].text == "such");
//  REQUIRE(tokens[7].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[7].text == "that");
//  REQUIRE(tokens[8].type == PQLTokenType::RELREF);
//  REQUIRE(tokens[8].text == "Uses");
//  REQUIRE(tokens[9].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[9].text == "a");
//  REQUIRE(tokens[10].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[10].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + such that clause with Integer") {
//  std::string query = "variable v; Select v such that Uses(1, v)";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 9);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//  REQUIRE(tokens[4].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[4].text == "such");
//  REQUIRE(tokens[5].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[5].text == "that");
//  REQUIRE(tokens[6].type == PQLTokenType::RELREF);
//  REQUIRE(tokens[6].text == "Uses");
//  REQUIRE(tokens[7].type == PQLTokenType::INTEGER);
//  REQUIRE(tokens[7].text == "1");
//  REQUIRE(tokens[8].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[8].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + such that clause with wildcard") {
//  std::string query = "variable v; Select v such that Uses(_, v)";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 9);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//  REQUIRE(tokens[4].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[4].text == "such");
//  REQUIRE(tokens[5].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[5].text == "that");
//  REQUIRE(tokens[6].type == PQLTokenType::RELREF);
//  REQUIRE(tokens[6].text == "Uses");
//  REQUIRE(tokens[7].type == PQLTokenType::WILDCARD);
//  REQUIRE(tokens[7].text == "_");
//  REQUIRE(tokens[8].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[8].text == "v");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + such that clause with IDENT") {
//  std::string query = "variable v; Select v such that Uses(\"main\", v)";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 9);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[2].text == "Select");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "v");
//  REQUIRE(tokens[4].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[4].text == "such");
//  REQUIRE(tokens[5].type == PQLTokenType::SUCH_THAT);
//  REQUIRE(tokens[5].text == "that");
//  REQUIRE(tokens[6].type == PQLTokenType::RELREF);
//  REQUIRE(tokens[6].text == "Uses");
//  REQUIRE(tokens[7].type == PQLTokenType::IDENT);
//  REQUIRE(tokens[7].text == "main"); // quotations should be removed
//  REQUIRE(tokens[8].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[8].text == "v");
//}
//
//TEST_CASE("Tokenizer cannot tokenize simple select + such that clause with invalid syntax") {
//  std::string query = "variable v; assign a; Select a such Uses(a, v)";
//  QpsSyntaxError syntaxError = QpsSyntaxError("Syntax error");
//  try {
//    std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  } catch (const QpsError & e) {
//    REQUIRE(e.what() == syntaxError.what());
//  }
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + pattern clause syn_assign (entref, exprSpec)") {
//  std::string query = "variable v; assign a; Select v pattern a (v, _\"x+y\"_)";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 10);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[2].text == "assign");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "a");
//  REQUIRE(tokens[4].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[4].text == "Select");
//  REQUIRE(tokens[5].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[5].text == "v");
//  REQUIRE(tokens[6].type == PQLTokenType::PATTERN);
//  REQUIRE(tokens[6].text == "pattern");
//  REQUIRE(tokens[7].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[7].text == "a");
//  REQUIRE(tokens[8].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[8].text == "v");
//  REQUIRE(tokens[9].type == PQLTokenType::PARTIALEXPR);
//  REQUIRE(tokens[9].text == "x+y");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + pattern clause (entref, expr)") {
//  std::string query = "variable v; assign a; Select v pattern a (v, \"x+y\")";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 10);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[2].text == "assign");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "a");
//  REQUIRE(tokens[4].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[4].text == "Select");
//  REQUIRE(tokens[5].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[5].text == "v");
//  REQUIRE(tokens[6].type == PQLTokenType::PATTERN);
//  REQUIRE(tokens[6].text == "pattern");
//  REQUIRE(tokens[7].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[7].text == "a");
//  REQUIRE(tokens[8].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[8].text == "v");
//  REQUIRE(tokens[9].type == PQLTokenType::IDENT);
//  REQUIRE(tokens[9].text == "x+y");
//}
//
//TEST_CASE("Tokenizer can tokenize simple select + pattern clause (_, _expr_)") {
//  std::string query = "variable v; assign a; Select v pattern a (_, _\"x+y\"_)";
//  std::vector<QueryToken> tokens = QueryTokenizer::tokenize(query);
//  REQUIRE(tokens.size() == 10);
//  REQUIRE(tokens[0].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[0].text == "variable");
//  REQUIRE(tokens[1].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[1].text == "v");
//  REQUIRE(tokens[2].type == PQLTokenType::DECLARATION);
//  REQUIRE(tokens[2].text == "assign");
//  REQUIRE(tokens[3].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[3].text == "a");
//  REQUIRE(tokens[4].type == PQLTokenType::SELECT);
//  REQUIRE(tokens[4].text == "Select");
//  REQUIRE(tokens[5].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[5].text == "v");
//  REQUIRE(tokens[6].type == PQLTokenType::PATTERN);
//  REQUIRE(tokens[6].text == "pattern");
//  REQUIRE(tokens[7].type == PQLTokenType::SYNONYM);
//  REQUIRE(tokens[7].text == "a");
//  REQUIRE(tokens[8].type == PQLTokenType::WILDCARD);
//  REQUIRE(tokens[8].text == "_");
//  REQUIRE(tokens[9].type == PQLTokenType::PARTIALEXPR);
//  REQUIRE(tokens[9].text == "x+y");
//}

TEST_CASE("Test split query") {
  std::string sample_query_1 = "variable v; Select v";
  QueryStructure statements = QueryTokenizer::splitQuery(sample_query_1);
  std::vector<std::string> declaration_statements = {"variable v"};
  std::string select_statement = "Select v";
  QueryStructure expected_statements = {declaration_statements, select_statement};
  REQUIRE(statements.declaration_statements == expected_statements.declaration_statements);
  REQUIRE(statements.select_statement == expected_statements.select_statement);

  //TEST negative test cases
  QpsSyntaxError no_select_error = QpsSyntaxError("No select statement");
  QpsSyntaxError statement_after_select_error = QpsSyntaxError("Statements after select statement are invalid");
  try {
    std::string sample_query_2 = "variable v;";
    QueryStructure statements_2 = QueryTokenizer::splitQuery(sample_query_2);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == no_select_error.what());
  }

  try {
    std::string sample_query_3 = "variable v; Select v; assign v;";
    QueryStructure statements_3 = QueryTokenizer::splitQuery(sample_query_3);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == statement_after_select_error.what());
  }
}

TEST_CASE("Test extract declarations") {
  std::string sample_query_1 = "variable v; assign a; Select v";
  QueryStructure statements = QueryTokenizer::splitQuery(sample_query_1);
  std::vector<std::string> declaration_statements = statements.declaration_statements;
  std::vector<Declaration> declarations = QueryTokenizer::extractDeclarations(declaration_statements);
  REQUIRE(declarations.size() == 2);
  std::vector<Declaration> expected_declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };
  REQUIRE(declarations[0].equals(expected_declarations[0]));
  REQUIRE(declarations[1].equals(expected_declarations[1]));

  QpsSyntaxError invalid_entity_error = QpsSyntaxError("Design entity is not valid");
  QpsSyntaxError missing_synonym_error = QpsSyntaxError("Missing synonyms for design entity");
  QpsSyntaxError lexical_error = QpsSyntaxError("Synonym does not follow lexical rules");
  QpsSemanticError repeated_synonym_declaration_error = QpsSemanticError("Repeated synonym declaration");

  try {
    std::string sample_query_2 = "variable v; random r; Select v";
    QueryStructure statements_2 = QueryTokenizer::splitQuery(sample_query_2);
    std::vector<std::string> declaration_statements_2 = statements_2.declaration_statements;
    std::vector<Declaration> declarations_2 = QueryTokenizer::extractDeclarations(declaration_statements_2);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == invalid_entity_error.what());
  }

  try {
    std::string sample_query_3 = "variable; Select v";
    QueryStructure statements_3 = QueryTokenizer::splitQuery(sample_query_3);
    std::vector<std::string> declaration_statements_3 = statements_3.declaration_statements;
    std::vector<Declaration> declarations_3 = QueryTokenizer::extractDeclarations(declaration_statements_3);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == invalid_entity_error.what());
  }

  try {
    std::string sample_query_4 = "variable 0v; Select v";
    QueryStructure statements_4 = QueryTokenizer::splitQuery(sample_query_4);
    std::vector<std::string> declaration_statements_4 = statements_4.declaration_statements;
    std::vector<Declaration> declarations_4 = QueryTokenizer::extractDeclarations(declaration_statements_4);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == lexical_error.what());
  }

  try {
    std::string sample_query_5 = "variable v, v; Select v";
    QueryStructure statements_5 = QueryTokenizer::splitQuery(sample_query_5);
    std::vector<std::string> declaration_statements_5 = statements_5.declaration_statements;
    std::vector<Declaration> declarations_5 = QueryTokenizer::extractDeclarations(declaration_statements_5);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == repeated_synonym_declaration_error.what());
  }
}

TEST_CASE("Test extract select tokens") {
  std::string sample_query_1 = "variable v; assign a; Select v";
  QueryStructure statements = QueryTokenizer::splitQuery(sample_query_1);
  std::vector<std::string> declaration_statements = statements.declaration_statements;
  std::vector<Declaration> declarations = QueryTokenizer::extractDeclarations(declaration_statements);
  std::vector<QueryToken> select_tokens = QueryTokenizer::extractSelectToken(statements.select_statement, declarations);
  std::vector<QueryToken> expected_tokens = {
      {"v", PQLTokenType::SYNONYM}
  };
  REQUIRE(select_tokens.size() == 1);
  REQUIRE(select_tokens[0].text == expected_tokens[0].text);
  REQUIRE(select_tokens[0].type == expected_tokens[0].type);

  QpsSemanticError undeclared_error = QpsSemanticError("Select synonym has not been declared");
  try {
    std::string sample_query_2 = "variable v; Select a";
    QueryStructure statements_2 = QueryTokenizer::splitQuery(sample_query_2);
    std::vector<std::string> declaration_statements_2 = statements_2.declaration_statements;
    std::vector<Declaration> declarations_2 = QueryTokenizer::extractDeclarations(declaration_statements_2);
    std::vector<QueryToken>
        select_tokens_2 = QueryTokenizer::extractSelectToken(statements_2.select_statement, declarations_2);
  } catch (const QpsError & e) {
    REQUIRE(e.what() == undeclared_error.what());
  }
}

TEST_CASE("Dummy") {
  std::string str = "a (b, c)";
  std::string test = string_util::RemoveFirstWord(str);
  REQUIRE(test == "(b, c)");
}