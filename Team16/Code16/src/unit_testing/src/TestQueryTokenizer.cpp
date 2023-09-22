#include <catch.hpp>
#include <regex>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/qps_errors/qps_error.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/string_utils.h"
#include "qps/constants.h"

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
    REQUIRE(strcmp(e.what(), no_select_error.what()) == 0);
  }

  try {
    std::string sample_query_3 = "variable v; Select v; assign v;";
    QueryStructure statements_3 = QueryTokenizer::splitQuery(sample_query_3);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), statement_after_select_error.what()) == 0);
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
    REQUIRE(strcmp(e.what(), invalid_entity_error.what()) == 0);
  }

  try {
    std::string sample_query_3 = "variable; Select v";
    QueryStructure statements_3 = QueryTokenizer::splitQuery(sample_query_3);
    std::vector<std::string> declaration_statements_3 = statements_3.declaration_statements;
    std::vector<Declaration> declarations_3 = QueryTokenizer::extractDeclarations(declaration_statements_3);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), missing_synonym_error.what()) == 0);
  }

  try {
    std::string sample_query_4 = "variable 0v; Select v";
    QueryStructure statements_4 = QueryTokenizer::splitQuery(sample_query_4);
    std::vector<std::string> declaration_statements_4 = statements_4.declaration_statements;
    std::vector<Declaration> declarations_4 = QueryTokenizer::extractDeclarations(declaration_statements_4);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), lexical_error.what()) == 0);
  }

  try {
    std::string sample_query_5 = "variable v, v; Select v";
    QueryStructure statements_5 = QueryTokenizer::splitQuery(sample_query_5);
    std::vector<std::string> declaration_statements_5 = statements_5.declaration_statements;
    std::vector<Declaration> declarations_5 = QueryTokenizer::extractDeclarations(declaration_statements_5);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), repeated_synonym_declaration_error.what()) == 0);
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
    REQUIRE(strcmp(e.what(), undeclared_error.what()) == 0);
  }
}

TEST_CASE("Test get clause index") {
  std::string sample_statement = "such that Uses (a, b) pattern a (c, d)";
  std::vector<size_t> indexes = QueryTokenizer::getClauseIndexes(sample_statement);
  std::vector<size_t> expected_indexes = {0, 22};
  REQUIRE(indexes.size() == 2);
  REQUIRE(indexes == expected_indexes);

  QpsSyntaxError wrong_clause_error = QpsSyntaxError("Invalid clause expressions");
  QpsSyntaxError unexpected_clause_error = QpsSyntaxError("Unexpected clause expression");

  try {
    std::string wrong_clause = "such That Uses (a, b)";
    std::vector<size_t> wrong_clause_indexes = QueryTokenizer::getClauseIndexes(wrong_clause);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), wrong_clause_error.what()) == 0);
  }

  try {
    std::string unexpected_clause = "something such that Uses (a, b)";
    std::vector<size_t> unexpected_clause_indexes = QueryTokenizer::getClauseIndexes(unexpected_clause);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), unexpected_clause_error.what()) == 0);
  }
}

TEST_CASE("Test get relationship reference arguments") {
  std::string sample_arg = "(a, v)";
  std::vector<Declaration> declarations = {
      {"a", DesignEntity::ASSIGN},
      {"v", DesignEntity::VARIABLE}
  };

  std::pair<QueryToken, QueryToken> args = QueryTokenizer::getRelRefArgs(sample_arg, declarations);
  std::pair<QueryToken, QueryToken> expected_args = {
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };
  REQUIRE(args.first.type == expected_args.first.type);
  REQUIRE(args.first.text == expected_args.first.text);
  REQUIRE(args.second.text == expected_args.second.text);
  REQUIRE(args.second.type == expected_args.second.type);

  std::string sample_arg_2 = "(a, _)";
  std::vector<Declaration> declarations_2 = {
      {"a", DesignEntity::ASSIGN},
  };

  std::pair<QueryToken, QueryToken> args_2 = QueryTokenizer::getRelRefArgs(sample_arg_2, declarations_2);
  std::pair<QueryToken, QueryToken> expected_args_2 = {
      {"a", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };
  REQUIRE(args_2.first.type == expected_args_2.first.type);
  REQUIRE(args_2.first.text == expected_args_2.first.text);
  REQUIRE(args_2.second.text == expected_args_2.second.text);
  REQUIRE(args_2.second.type == expected_args_2.second.type);

  std::string sample_arg_3 = "(a, \"x\")";
  std::vector<Declaration> declarations_3 = {
      {"a", DesignEntity::ASSIGN},
  };
  std::pair<QueryToken, QueryToken> args_3 = QueryTokenizer::getRelRefArgs(sample_arg_3, declarations_3);
  std::pair<QueryToken, QueryToken> expected_args_3 = {
      {"a", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::IDENT}
  };
  REQUIRE(args_3.first.type == expected_args_3.first.type);
  REQUIRE(args_3.first.text == expected_args_3.first.text);
  REQUIRE(args_3.second.text == expected_args_3.second.text);
  REQUIRE(args_3.second.type == expected_args_3.second.type);

  QpsSyntaxError more_than_2_error = QpsSyntaxError("More than 2 synonyms in relation reference");
  QpsSemanticError not_declared_error = QpsSemanticError("RHS synonym not declared");
  std::vector<Declaration> error_declarations = {
      {"a", DesignEntity::ASSIGN}
  };

  try {
    std::string more_that_2_arg = "(a, b, c)";
    std::pair<QueryToken, QueryToken>
        more_than_2_error_args = QueryTokenizer::getRelRefArgs(more_that_2_arg, error_declarations);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), more_than_2_error.what()) == 0);
  }

  try {
    std::string not_declared_arg = "(a, b)";
    std::pair<QueryToken, QueryToken>
        not_declared_error_args = QueryTokenizer::getRelRefArgs(not_declared_arg, error_declarations);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), not_declared_error.what()) == 0);
  }
}

TEST_CASE("Test get pattern arguments") {
  std::string sample_arg = "(s, _\"x+y\"_)";
  std::vector<Declaration> declarations = {
      {"s", DesignEntity::STMT},
  };

  std::pair<QueryToken, QueryToken> args = QueryTokenizer::getPatternArgs(sample_arg, declarations);
  std::pair<QueryToken, QueryToken> expected_args = {
      {"s", PQLTokenType::SYNONYM},
      {"x+y", PQLTokenType::PARTIALEXPR}
  };
  REQUIRE(args.first.type == expected_args.first.type);
  REQUIRE(args.first.text == expected_args.first.text);
  REQUIRE(args.second.text == expected_args.second.text);
  REQUIRE(args.second.type == expected_args.second.type);

  std::string sample_arg_2 = "(s, _)";
  std::vector<Declaration> declarations_2 = {
      {"s", DesignEntity::STMT},
  };

  std::pair<QueryToken, QueryToken> args_2 = QueryTokenizer::getPatternArgs(sample_arg_2, declarations_2);
  std::pair<QueryToken, QueryToken> expected_args_2 = {
      {"s", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };
  REQUIRE(args_2.first.type == expected_args_2.first.type);
  REQUIRE(args_2.first.text == expected_args_2.first.text);
  REQUIRE(args_2.second.text == expected_args_2.second.text);
  REQUIRE(args_2.second.type == expected_args_2.second.type);

  std::string sample_arg_3 = "(s, \"x\")";
  std::vector<Declaration> declarations_3 = {
      {"s", DesignEntity::STMT},
  };
  // TODO (Su Mian): Need to factor in the case for "x+y"
  std::pair<QueryToken, QueryToken> args_3 = QueryTokenizer::getPatternArgs(sample_arg_3, declarations_3);
  std::pair<QueryToken, QueryToken> expected_args_3 = {
      {"s", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::IDENT}
  };
  REQUIRE(args_3.first.type == expected_args_3.first.type);
  REQUIRE(args_3.first.text == expected_args_3.first.text);
  REQUIRE(args_3.second.text == expected_args_3.second.text);
  REQUIRE(args_3.second.type == expected_args_3.second.type);

  QpsSyntaxError more_than_2_error = QpsSyntaxError("More than 2 arguments in pattern clause");
  QpsSemanticError not_declared_error = QpsSemanticError("LHS synonym not declared");
  std::vector<Declaration> error_declarations = {
      {"s", DesignEntity::STMT}
  };

  try {
    std::string more_that_2_arg = "(s, b, c)";
    std::pair<QueryToken, QueryToken>
        more_than_2_error_args = QueryTokenizer::getPatternArgs(more_that_2_arg, error_declarations);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), more_than_2_error.what()) == 0);
  }

  try {
    std::string not_declared_arg = "(a, _)";
    std::pair<QueryToken, QueryToken>
        more_than_2_error_args = QueryTokenizer::getPatternArgs(not_declared_arg, error_declarations);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), not_declared_error.what()) == 0);
  }
}

TEST_CASE("Test extract clause tokens") {
  std::string select_statement = "Select v such that Uses (a, v)";
  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };

  std::vector<QueryToken> such_that_tokens = {
      {"Uses", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };
  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results = QueryTokenizer::extractClauseTokens(select_statement, declarations);
  std::pair<std::vector<QueryToken>, std::vector<QueryToken>> expected = {
      {such_that_tokens},
      {}
  };

  REQUIRE(results.first[0].text == expected.first[0].text);
  REQUIRE(results.first[0].type == expected.first[0].type);
  REQUIRE(results.first[1].text == expected.first[1].text);
  REQUIRE(results.first[1].type == expected.first[1].type);
  REQUIRE(results.first[2].text == expected.first[2].text);
  REQUIRE(results.first[2].type == expected.first[2].type);
  REQUIRE(results.second.empty());

  std::string select_statement_2 = "Select v pattern a (v, \"x\")";
  std::vector<Declaration> declarations_2 = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };

  std::vector<QueryToken> pattern_tokens = {
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::IDENT}
  };
  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(select_statement_2, declarations_2);
  std::pair<std::vector<QueryToken>, std::vector<QueryToken>> expected_2 = {
      {},
      {pattern_tokens}
  };

  REQUIRE(results_2.second[0].text == expected_2.second[0].text);
  REQUIRE(results_2.second[0].type == expected_2.second[0].type);
  REQUIRE(results_2.second[1].text == expected_2.second[1].text);
  REQUIRE(results_2.second[1].type == expected_2.second[1].type);
  REQUIRE(results_2.second[2].text == expected_2.second[2].text);
  REQUIRE(results_2.second[2].type == expected_2.second[2].type);
  REQUIRE(results_2.first.empty());

  QpsSyntaxError unknown_clause_error = QpsSyntaxError("Invalid clause expressions");
  QpsSyntaxError undeclared_pattern_error = QpsSyntaxError("Invalid argument for RHS of pattern clause");
  std::vector<Declaration> error_declarations = {
      {"v", DesignEntity::VARIABLE}
  };

  try {
    std::string unknown_clause_statement = "Select v something else (v, \"x\")";
    std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
        unknown_clause_result = QueryTokenizer::extractClauseTokens(unknown_clause_statement, error_declarations);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), unknown_clause_error.what()) == 0);
  }

  try {
    std::string undeclared_pattern_statement = "Select v pattern a (a, v)";
    std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
        unknown_clause_result = QueryTokenizer::extractClauseTokens(undeclared_pattern_statement, error_declarations);
  } catch (const QpsError & e) {
    REQUIRE(strcmp(e.what(), undeclared_pattern_error.what()) == 0);
  }

}

TEST_CASE("Dummy") {
  std::string str = "such that Uses(a, v)";
  bool test = std::regex_search(str, qps_constants::kSuchThatClauseRegex);
  REQUIRE(test == true);
}