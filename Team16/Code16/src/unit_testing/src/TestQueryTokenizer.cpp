#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/qps_errors/qps_error.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/string_utils.h"

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
  } catch (const QpsError& e) {
    REQUIRE(strcmp(e.what(), no_select_error.what()) == 0);
  }

  try {
    std::string sample_query_3 = "variable v; Select v; assign v;";
    QueryStructure statements_3 = QueryTokenizer::splitQuery(sample_query_3);
  } catch (const QpsError& e) {
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
  } catch (const QpsError& e) {
    REQUIRE(strcmp(e.what(), invalid_entity_error.what()) == 0);
  }

  try {
    std::string sample_query_3 = "variable; Select v";
    QueryStructure statements_3 = QueryTokenizer::splitQuery(sample_query_3);
    std::vector<std::string> declaration_statements_3 = statements_3.declaration_statements;
    std::vector<Declaration> declarations_3 = QueryTokenizer::extractDeclarations(declaration_statements_3);
  } catch (const QpsError& e) {
    REQUIRE(strcmp(e.what(), missing_synonym_error.what()) == 0);
  }

  try {
    std::string sample_query_4 = "variable 0v; Select v";
    QueryStructure statements_4 = QueryTokenizer::splitQuery(sample_query_4);
    std::vector<std::string> declaration_statements_4 = statements_4.declaration_statements;
    std::vector<Declaration> declarations_4 = QueryTokenizer::extractDeclarations(declaration_statements_4);
  } catch (const QpsError& e) {
    REQUIRE(strcmp(e.what(), lexical_error.what()) == 0);
  }

  try {
    std::string sample_query_5 = "variable v, v; Select v";
    QueryStructure statements_5 = QueryTokenizer::splitQuery(sample_query_5);
    std::vector<std::string> declaration_statements_5 = statements_5.declaration_statements;
    std::vector<Declaration> declarations_5 = QueryTokenizer::extractDeclarations(declaration_statements_5);
  } catch (const QpsError& e) {
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
  } catch (const QpsError& e) {
    REQUIRE(strcmp(e.what(), undeclared_error.what()) == 0);
  }
}

TEST_CASE("Test get clause index") {
  std::string sample_statement = "such that Uses(a, b) pattern a(c, d)";
  std::vector<size_t> indexes = QueryTokenizer::getClauseIndexes(sample_statement);
  std::vector<size_t> expected_indexes = {0, 21};
  REQUIRE(indexes.size() == 2);
  REQUIRE(indexes == expected_indexes);

  QpsSyntaxError wrong_clause_error = QpsSyntaxError("Invalid clause expressions");
  QpsSyntaxError unexpected_clause_error = QpsSyntaxError("Unexpected clause expression");

  try {
    std::string wrong_clause = "such That Uses(a, b)";
    std::vector<size_t> wrong_clause_indexes = QueryTokenizer::getClauseIndexes(wrong_clause);
  } catch (const QpsError& e) {
    REQUIRE(strcmp(e.what(), wrong_clause_error.what()) == 0);
  }

  try {
    std::string unexpected_clause = "something such that Uses(a, b)";
    std::vector<size_t> unexpected_clause_indexes = QueryTokenizer::getClauseIndexes(unexpected_clause);
  } catch (const QpsError& e) {
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

  std::string more_that_2_arg = "(a, b, c)";
  REQUIRE_THROWS_AS(QueryTokenizer::getRelRefArgs(more_that_2_arg, error_declarations), QpsSyntaxError);

  std::string not_declared_arg = "(a, b)";
  REQUIRE_THROWS_AS(QueryTokenizer::getRelRefArgs(not_declared_arg, error_declarations), QpsSemanticError);
}

TEST_CASE("Test get pattern arguments") {
  std::string sample_arg = "(s, _\"x+y\"_)";
  std::vector<Declaration> declarations = {
      {"s", DesignEntity::STMT},
  };

  std::pair<QueryToken, QueryToken> args = QueryTokenizer::getPatternArgs(sample_arg,
                                                                          declarations,
                                                                          PQLTokenType::SYNONYM);
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

  std::pair<QueryToken, QueryToken> args_2 = QueryTokenizer::getPatternArgs(sample_arg_2,
                                                                            declarations_2,
                                                                            PQLTokenType::SYNONYM);
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
  std::pair<QueryToken, QueryToken> args_3 = QueryTokenizer::getPatternArgs(sample_arg_3,
                                                                            declarations_3,
                                                                            PQLTokenType::SYNONYM);
  std::pair<QueryToken, QueryToken> expected_args_3 = {
      {"s", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::EXACTEXPR}
  };
  REQUIRE(args_3.first.type == expected_args_3.first.type);
  REQUIRE(args_3.first.text == expected_args_3.first.text);
  REQUIRE(args_3.second.text == expected_args_3.second.text);
  REQUIRE(args_3.second.type == expected_args_3.second.type);

  std::vector<Declaration> error_declarations = {
      {"s", DesignEntity::STMT}
  };

  std::string more_that_2_arg = "(s, b, c)";
  REQUIRE_THROWS_AS(QueryTokenizer::getPatternArgs(more_that_2_arg, error_declarations, PQLTokenType::SYNONYM),
                    QpsSyntaxError);

  std::string not_declared_arg = "(a, _)";
  REQUIRE_THROWS_AS(QueryTokenizer::getPatternArgs(not_declared_arg, error_declarations, PQLTokenType::SYNONYM),
                    QpsSemanticError);
}

TEST_CASE("Test extract clause tokens") {
  std::string select_statement = "Select v such that Uses(a, v)";
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
      {"x", PQLTokenType::EXACTEXPR}
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

  std::string unknown_clause_statement = "Select v something else (v, \"x\")";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(unknown_clause_statement, error_declarations), QpsSyntaxError);

  std::string undeclared_pattern_statement = "Select v pattern a (a, v)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(undeclared_pattern_statement, error_declarations),
                    QpsSemanticError);
}

TEST_CASE("Test extract one select and on pattern") {
  std::string select_statement = "Select v such that Uses(a, v) pattern a(v, _)";
  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };
  std::vector<QueryToken> such_that_tokens = {
      {"Uses", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };
  std::vector<QueryToken> pattern_tokens = {
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results = QueryTokenizer::extractClauseTokens(select_statement, declarations);

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>> expected = {
      {such_that_tokens},
      {pattern_tokens}
  };

  REQUIRE(results.first[0].text == expected.first[0].text);
  REQUIRE(results.first[0].type == expected.first[0].type);
  REQUIRE(results.first[1].text == expected.first[1].text);
  REQUIRE(results.first[1].type == expected.first[1].type);
  REQUIRE(results.first[2].text == expected.first[2].text);
  REQUIRE(results.first[2].type == expected.first[2].type);

  REQUIRE(results.second[0].text == expected.second[0].text);
  REQUIRE(results.second[0].type == expected.second[0].type);
  REQUIRE(results.second[1].text == expected.second[1].text);
  REQUIRE(results.second[1].type == expected.second[1].type);
  REQUIRE(results.second[2].text == expected.second[2].text);
  REQUIRE(results.second[2].type == expected.second[2].type);
}

TEST_CASE("Tokenizer and tokenise pattern expressions") {
  std::string sample_query_1 = "Select a1 pattern a1(_, _\"abc + cde % fgh\"_)";
  std::vector<Declaration> declarations = {
      {"a1", DesignEntity::ASSIGN}
  };
  std::vector<QueryToken> pattern_tokens = {
      {"a1", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD},
      {"abc+cde%fgh", PQLTokenType::PARTIALEXPR}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results = QueryTokenizer::extractClauseTokens(sample_query_1, declarations);

  REQUIRE(results.second.size() == 3);
  REQUIRE(results.second[0].type == pattern_tokens[0].type);
  REQUIRE(results.second[0].text == pattern_tokens[0].text);
  REQUIRE(results.second[1].type == pattern_tokens[1].type);
  REQUIRE(results.second[1].text == pattern_tokens[1].text);
  REQUIRE(results.second[2].type == pattern_tokens[2].type);
  REQUIRE(results.second[2].text == pattern_tokens[2].text);

  std::string sample_query_2 = "Select a1 pattern a1(v, \"abc + cde % fgh\")";
  std::vector<Declaration> declarations_2 = {
      {"a1", DesignEntity::ASSIGN},
      {"v", DesignEntity::VARIABLE}
  };
  std::vector<QueryToken> pattern_tokens_2 = {
      {"a1", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"abc+cde%fgh", PQLTokenType::EXACTEXPR}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_2);

  REQUIRE(results_2.second.size() == 3);
  REQUIRE(results_2.second[0].type == pattern_tokens_2[0].type);
  REQUIRE(results_2.second[0].text == pattern_tokens_2[0].text);
  REQUIRE(results_2.second[1].type == pattern_tokens_2[1].type);
  REQUIRE(results_2.second[1].text == pattern_tokens_2[1].text);
  REQUIRE(results_2.second[2].type == pattern_tokens_2[2].type);
  REQUIRE(results_2.second[2].text == pattern_tokens_2[2].text);

  std::string sample_query_3 = "Select a1 pattern a1(v, \"+ cde % fgh\")";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_3, declarations), QpsSyntaxError);
}

TEST_CASE("Tokenizer can tokenize Calls and Calls*") {
  std::string sample_query_1 = "Select p such that Calls(p, _)";

  std::vector<Declaration> declarations_1 = {
      {"p", DesignEntity::PROCEDURE},
  };

  std::vector<QueryToken> such_that_tokens = {
      {"Calls", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1.first.size() == 3);
  REQUIRE(results_1.first[0].text == such_that_tokens[0].text);
  REQUIRE(results_1.first[0].type == such_that_tokens[0].type);
  REQUIRE(results_1.first[1].text == such_that_tokens[1].text);
  REQUIRE(results_1.first[1].type == such_that_tokens[1].type);
  REQUIRE(results_1.first[2].text == such_that_tokens[2].text);
  REQUIRE(results_1.first[2].type == such_that_tokens[2].type);

  std::string sample_query_2 = "Select p such that Calls*(p, \"Third\")";

  std::vector<QueryToken> such_that_tokens_2 = {
      {"Calls*", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"Third", PQLTokenType::IDENT}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1);

  REQUIRE(results_2.first.size() == 3);
  REQUIRE(results_2.first[0].text == such_that_tokens_2[0].text);
  REQUIRE(results_2.first[0].type == such_that_tokens_2[0].type);
  REQUIRE(results_2.first[1].text == such_that_tokens_2[1].text);
  REQUIRE(results_2.first[1].type == such_that_tokens_2[1].type);
  REQUIRE(results_2.first[2].text == such_that_tokens_2[2].text);
  REQUIRE(results_2.first[2].type == such_that_tokens_2[2].type);
}

TEST_CASE("Tokenizer can tokenize Next and Next*") {
  std::string sample_query_1 = "Select s1 such that Next(s1, _)";

  std::vector<Declaration> declarations_1 = {
      {"s1", DesignEntity::STMT},
  };

  std::vector<QueryToken> such_that_tokens = {
      {"Next", PQLTokenType::RELREF},
      {"s1", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1.first.size() == 3);
  REQUIRE(results_1.first[0].text == such_that_tokens[0].text);
  REQUIRE(results_1.first[0].type == such_that_tokens[0].type);
  REQUIRE(results_1.first[1].text == such_that_tokens[1].text);
  REQUIRE(results_1.first[1].type == such_that_tokens[1].type);
  REQUIRE(results_1.first[2].text == such_that_tokens[2].text);
  REQUIRE(results_1.first[2].type == such_that_tokens[2].type);

  std::string sample_query_2 = "Select s1 such that Next*(2, 9)";

  std::vector<QueryToken> such_that_tokens_2 = {
      {"Next*", PQLTokenType::RELREF},
      {"2", PQLTokenType::INTEGER},
      {"9", PQLTokenType::INTEGER}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1);

  REQUIRE(results_2.first.size() == 3);
  REQUIRE(results_2.first[0].text == such_that_tokens_2[0].text);
  REQUIRE(results_2.first[0].type == such_that_tokens_2[0].type);
  REQUIRE(results_2.first[1].text == such_that_tokens_2[1].text);
  REQUIRE(results_2.first[1].type == such_that_tokens_2[1].type);
  REQUIRE(results_2.first[2].text == such_that_tokens_2[2].text);
  REQUIRE(results_2.first[2].type == such_that_tokens_2[2].type);
}

TEST_CASE("Tokenizer can tokenize Affects") {
  std::string sample_query_1 = "Select a1 such that Affects(a1, a2)";

  std::vector<Declaration> declarations_1 = {
      {"a1", DesignEntity::STMT},
      {"a2", DesignEntity::STMT}
  };

  std::vector<QueryToken> such_that_tokens = {
      {"Affects", PQLTokenType::RELREF},
      {"a1", PQLTokenType::SYNONYM},
      {"a2", PQLTokenType::SYNONYM}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1.first.size() == 3);
  REQUIRE(results_1.first[0].text == such_that_tokens[0].text);
  REQUIRE(results_1.first[0].type == such_that_tokens[0].type);
  REQUIRE(results_1.first[1].text == such_that_tokens[1].text);
  REQUIRE(results_1.first[1].type == such_that_tokens[1].type);
  REQUIRE(results_1.first[2].text == such_that_tokens[2].text);
  REQUIRE(results_1.first[2].type == such_that_tokens[2].type);
}

TEST_CASE("Tokenizer can tokenize while pattern") {
  std::string sample_query_1 = "Select w pattern w (\"x\", _)";

  std::vector<Declaration> declarations_1 = {
      {"w", DesignEntity::WHILE_LOOP},
  };

  std::vector<QueryToken> pattern_tokens = {
      {"w", PQLTokenType::PATTERN_WHILE},
      {"x", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1.second.size() == 3);
  REQUIRE(results_1.second[0].text == pattern_tokens[0].text);
  REQUIRE(results_1.second[0].type == pattern_tokens[0].type);
  REQUIRE(results_1.second[1].text == pattern_tokens[1].text);
  REQUIRE(results_1.second[1].type == pattern_tokens[1].type);
  REQUIRE(results_1.second[2].text == pattern_tokens[2].text);
  REQUIRE(results_1.second[2].type == pattern_tokens[2].type);

  // Invalid syntax for RHS pattern
  std::string sample_query_2 = "Select w pattern w (\"x\", \"x\")";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1), QpsSemanticError);

  // Undeclared while synonym throws semantic error
  std::string sample_query_3 = "Select y pattern y (\"x\", _)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_3, declarations_1), QpsSemanticError);
}

TEST_CASE("Tokenizer can tokenize if pattern") {
  std::string sample_query_1 = "Select ifs pattern ifs(_,_,_)";

  std::vector<Declaration> declarations_1 = {
      {"ifs", DesignEntity::IF_STMT},
  };

  std::vector<QueryToken> pattern_tokens = {
      {"ifs", PQLTokenType::PATTERN_IF},
      {"_", PQLTokenType::WILDCARD},
      {"_", PQLTokenType::WILDCARD}
  };

  std::pair<std::vector<QueryToken>, std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1.second.size() == 3);
  REQUIRE(results_1.second[0].text == pattern_tokens[0].text);
  REQUIRE(results_1.second[0].type == pattern_tokens[0].type);
  REQUIRE(results_1.second[1].text == pattern_tokens[1].text);
  REQUIRE(results_1.second[1].type == pattern_tokens[1].type);
  REQUIRE(results_1.second[2].text == pattern_tokens[2].text);
  REQUIRE(results_1.second[2].type == pattern_tokens[2].type);

  // Not enough arguments for if pattern
  std::string sample_query_2 = "Select ifs pattern ifs(_,_)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1), QpsSemanticError);

  // Undeclared if synonym
  std::string sample_query_3 = "Select if pattern if(\"x\",_,_)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_3, declarations_1), QpsSemanticError);
}

TEST_CASE("Debug") {
  std::string sample_query_1 = "Select w pattern w( v, \"x\")";
  std::vector<Declaration> declarations_1 = {
      {"w", DesignEntity::WHILE_LOOP},
      {"v", DesignEntity::VARIABLE}
  };

  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1), QpsSemanticError);
}
