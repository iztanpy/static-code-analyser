#include <catch.hpp>
#include "qps/query_parser/query_tokenizer/query_tokenizer.h"
#include "qps/qps_errors/qps_error.h"
#include "qps/qps_errors/qps_syntax_error.h"
#include "qps/qps_errors/qps_semantic_error.h"
#include "utils/string_utils.h"
#include "qps/query_parser/QueryUtil.h"

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
  REQUIRE(declarations[0] == expected_declarations[0]);
  REQUIRE(declarations[1] == expected_declarations[1]);

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
      {"((x)+(y))", PQLTokenType::PARTIALEXPR}
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
      {"(x)", PQLTokenType::EXACTEXPR}
  };
  REQUIRE(args_3.first.type == expected_args_3.first.type);
  REQUIRE(args_3.first.text == expected_args_3.first.text);
  REQUIRE(args_3.second.text == expected_args_3.second.text);
  REQUIRE(args_3.second.type == expected_args_3.second.type);

  std::vector<Declaration> error_declarations = {
      {"s", DesignEntity::STMT}
  };

  std::string more_that_2_arg = "stmt s; assign a; Select s pattern a(s, b, c)";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(more_that_2_arg),
                    QpsSyntaxError);

  std::string not_declared_arg = "if f; Select f pattern f(a, _, _)";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(not_declared_arg),
                    QpsSemanticError);
}

TEST_CASE("Test extract clause tokens") {
  std::string select_statement = "Select v such that Uses(a, v)";
  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };

  std::vector<QueryToken> such_that_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Uses", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };
  std::vector<std::vector<QueryToken>>
      results = QueryTokenizer::extractClauseTokens(select_statement, declarations);
  std::vector<std::vector<QueryToken>> expected = {
      {such_that_tokens},
      {},
      {}
  };

  for (int i = 0; i < such_that_tokens.size(); i++) {
    REQUIRE(results[0][i].text == expected[0][i].text);
    REQUIRE(results[0][i].type == expected[0][i].type);
  }
  REQUIRE(results[1].empty());
  REQUIRE(results[2].empty());

  std::string select_statement_2 = "Select v pattern a (v, \"x\")";
  std::vector<Declaration> declarations_2 = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };

  std::vector<QueryToken> pattern_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"(x)", PQLTokenType::EXACTEXPR}
  };
  std::vector<std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(select_statement_2, declarations_2);
  std::vector<std::vector<QueryToken>> expected_2 = {
      {},
      {pattern_tokens},
      {}
  };

  for (int i = 0; i < pattern_tokens.size(); i++) {
    REQUIRE(results_2[1][i].text == expected_2[1][i].text);
    REQUIRE(results_2[1][i].type == expected_2[1][i].type);
  }
  REQUIRE(results_2[0].empty());
  REQUIRE(results_2[2].empty());

  std::vector<Declaration> error_declarations = {
      {"v", DesignEntity::VARIABLE}
  };

  std::string undeclared_pattern_statement = "variable  v; Select v pattern a (a, \"v\")";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(undeclared_pattern_statement), QpsSemanticError);
}

TEST_CASE("Test extract one select and on pattern") {
  std::string select_statement = "Select v such that Uses(a, v) pattern a(v, _)";
  std::vector<Declaration> declarations = {
      {"v", DesignEntity::VARIABLE},
      {"a", DesignEntity::ASSIGN}
  };
  std::vector<QueryToken> such_that_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Uses", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM}
  };
  std::vector<QueryToken> pattern_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results = QueryTokenizer::extractClauseTokens(select_statement, declarations);

  std::vector<std::vector<QueryToken>> expected = {
      {such_that_tokens},
      {pattern_tokens},
      {}
  };

  for (int i = 0; i < such_that_tokens.size(); i++) {
    REQUIRE(results[0][i].text == expected[0][i].text);
    REQUIRE(results[0][i].type == expected[0][i].type);
  }

  for (int i = 0; i < such_that_tokens.size(); i++) {
    REQUIRE(results[1][i].text == expected[1][i].text);
    REQUIRE(results[1][i].type == expected[1][i].type);
  }
}

TEST_CASE("Tokenizer and tokenise pattern expressions") {
  std::string sample_query_1 = "Select a1 pattern a1(_, _\"abc + cde % fgh\"_)";
  std::vector<Declaration> declarations = {
      {"a1", DesignEntity::ASSIGN}
  };
  std::vector<QueryToken> pattern_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"a1", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD},
      {"((abc)+((cde)%(fgh)))", PQLTokenType::PARTIALEXPR}
  };

  std::vector<std::vector<QueryToken>>
      results = QueryTokenizer::extractClauseTokens(sample_query_1, declarations);

  REQUIRE(results[1].size() == 4);
  for (int i = 0; i < pattern_tokens.size(); i++) {
    REQUIRE(results[1][i].type == pattern_tokens[i].type);
    REQUIRE(results[1][i].text == pattern_tokens[i].text);
  }

  std::string sample_query_2 = "Select a1 pattern a1(v, \"abc + cde % fgh\")";
  std::vector<Declaration> declarations_2 = {
      {"a1", DesignEntity::ASSIGN},
      {"v", DesignEntity::VARIABLE}
  };
  std::vector<QueryToken> pattern_tokens_2 = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"a1", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"((abc)+((cde)%(fgh)))", PQLTokenType::EXACTEXPR}
  };

  std::vector<std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_2);

  REQUIRE(results_2[1].size() == 4);

  for (int i = 0; i < pattern_tokens_2.size(); i++) {
    REQUIRE(results_2[1][i].type == pattern_tokens_2[i].type);
    REQUIRE(results_2[1][i].text == pattern_tokens_2[i].text);
  }

  std::string sample_query_3 = "Select a1 pattern a1(v, \"+ cde % fgh\")";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_3, declarations), QpsSyntaxError);
}

TEST_CASE("Tokenizer can tokenize Calls and Calls*") {
  std::string sample_query_1 = "Select p such that Calls(p, _)";

  std::vector<Declaration> declarations_1 = {
      {"p", DesignEntity::PROCEDURE},
  };

  std::vector<QueryToken> such_that_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Calls", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1[0].size() == 4);
  for (int i = 0; i < such_that_tokens.size(); i++) {
    REQUIRE(results_1[0][i].text == such_that_tokens[i].text);
    REQUIRE(results_1[0][i].type == such_that_tokens[i].type);
  }

  std::string sample_query_2 = "Select p such that Calls*(p, \"Third\")";

  std::vector<QueryToken> such_that_tokens_2 = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Calls*", PQLTokenType::RELREF},
      {"p", PQLTokenType::SYNONYM},
      {"Third", PQLTokenType::IDENT}
  };

  std::vector<std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1);

  REQUIRE(results_2[0].size() == 4);
  for (int i = 0; i < such_that_tokens_2.size(); i++) {
    REQUIRE(results_2[0][i].text == such_that_tokens_2[i].text);
    REQUIRE(results_2[0][i].type == such_that_tokens_2[i].type);
  }
}

TEST_CASE("Tokenizer can tokenize Next and Next*") {
  std::string sample_query_1 = "Select s1 such that Next(s1, _)";

  std::vector<Declaration> declarations_1 = {
      {"s1", DesignEntity::STMT},
  };

  std::vector<QueryToken> such_that_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Next", PQLTokenType::RELREF},
      {"s1", PQLTokenType::SYNONYM},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1[0].size() == 4);
  for (int i = 0; i < such_that_tokens.size(); i++) {
    REQUIRE(results_1[0][i].text == such_that_tokens[i].text);
    REQUIRE(results_1[0][i].type == such_that_tokens[i].type);
  }

  std::string sample_query_2 = "Select s1 such that Next*(2, 9)";

  std::vector<QueryToken> such_that_tokens_2 = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Next*", PQLTokenType::RELREF},
      {"2", PQLTokenType::INTEGER},
      {"9", PQLTokenType::INTEGER}
  };

  std::vector<std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1);

  REQUIRE(results_2[0].size() == 4);
  for (int i = 0; i < such_that_tokens_2.size(); i++) {
    REQUIRE(results_2[0][i].text == such_that_tokens_2[i].text);
    REQUIRE(results_2[0][i].type == such_that_tokens_2[i].type);
  }
}

TEST_CASE("Tokenizer can tokenize Affects") {
  std::string sample_query_1 = "Select a1 such that Affects(a1, a2)";

  std::vector<Declaration> declarations_1 = {
      {"a1", DesignEntity::STMT},
      {"a2", DesignEntity::STMT}
  };

  std::vector<QueryToken> such_that_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Affects", PQLTokenType::RELREF},
      {"a1", PQLTokenType::SYNONYM},
      {"a2", PQLTokenType::SYNONYM}
  };

  std::vector<std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1[0].size() == 4);
  for (int i = 0; i < such_that_tokens.size(); i++) {
    REQUIRE(results_1[0][i].text == such_that_tokens[i].text);
    REQUIRE(results_1[0][i].type == such_that_tokens[i].type);
  }
}

TEST_CASE("Tokenizer can tokenize while pattern") {
  std::string sample_query_1 = "Select w pattern w (\"x\", _)";

  std::vector<Declaration> declarations_1 = {
      {"w", DesignEntity::WHILE_LOOP},
  };

  std::vector<QueryToken> pattern_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w", PQLTokenType::PATTERN_WHILE},
      {"x", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1[1].size() == 4);
  for (int i = 0; i < pattern_tokens.size(); i++) {
    REQUIRE(results_1[1][i].text == pattern_tokens[i].text);
    REQUIRE(results_1[1][i].type == pattern_tokens[i].type);
  }

  // Invalid syntax for RHS pattern
  std::string sample_query_2 = "while w; Select w pattern w (\"x\", \"x\")";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query_2), QpsSemanticError);

  // Undeclared while synonym throws semantic error
  std::string sample_query_3 = "Select y pattern y (\"x\", _)";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query_3), QpsSemanticError);
}

TEST_CASE("Tokenizer can tokenize if pattern") {
  std::string sample_query_1 = "Select ifs pattern ifs(_,_,_)";

  std::vector<Declaration> declarations_1 = {
      {"ifs", DesignEntity::IF_STMT},
  };

  std::vector<QueryToken> pattern_tokens = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"ifs", PQLTokenType::PATTERN_IF},
      {"_", PQLTokenType::WILDCARD},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1[1].size() == 4);
  for (int i = 0; i < pattern_tokens.size(); i++) {
    REQUIRE(results_1[1][i].text == pattern_tokens[i].text);
    REQUIRE(results_1[1][i].type == pattern_tokens[i].type);
  }

  // Not enough arguments for if pattern
  std::string sample_query_2 = "if ifs; Select ifs pattern ifs(_,_)";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query_2), QpsSemanticError);

  // Undeclared if synonym
  std::string sample_query_3 = "Select if pattern if(\"x\",_,_)";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query_3), QpsSemanticError);
}

TEST_CASE("Tokeniser can tokenise multiple clauses") {
  std::string sample_query_1 = "Select w pattern w (\"x\", _) and w1 (\"y\", _)";

  std::vector<Declaration> declarations_1 = {
      {"w", DesignEntity::WHILE_LOOP},
      {"w1", DesignEntity::WHILE_LOOP},
  };

  std::vector<QueryToken> pattern_tokens =  {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w", PQLTokenType::PATTERN_WHILE},
      {"x", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD},
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w1", PQLTokenType::PATTERN_WHILE},
      {"y", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  REQUIRE(results_1[1].size() == 8);

  for (int i = 0; i < pattern_tokens.size(); i++) {
    REQUIRE(results_1[1][i].text == pattern_tokens[i].text);
    REQUIRE(results_1[1][i].type == pattern_tokens[i].type);
  }

  std::string sample_query_2 = "Select w pattern w (\"x\", _) and w1 (\"y\", _) and w2 (\"z\", _)";

  std::vector<Declaration> declarations_2 = {
      {"w", DesignEntity::WHILE_LOOP},
      {"w1", DesignEntity::WHILE_LOOP},
      {"w2", DesignEntity::WHILE_LOOP}
  };

  std::vector<QueryToken> pattern_tokens_2 =  {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w", PQLTokenType::PATTERN_WHILE},
      {"x", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD},
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w1", PQLTokenType::PATTERN_WHILE},
      {"y", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD},
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w2", PQLTokenType::PATTERN_WHILE},
      {"z", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_2);

  REQUIRE(results_2[1].size() == 12);

  for (int i = 0; i < pattern_tokens_2.size(); i++) {
    REQUIRE(results_2[1][i].text == pattern_tokens_2[i].text);
    REQUIRE(results_2[1][i].type == pattern_tokens_2[i].type);
  }

  std::string sample_query_3 = "Select w pattern w (\"x\", _) such that Parent* (w, a) and Modifies (a, \"x\")";

  std::vector<Declaration> declarations_3 = {
      {"w", DesignEntity::WHILE_LOOP},
      {"a", DesignEntity::ASSIGN},
  };

  std::vector<QueryToken> such_that_tokens_3 = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Parent*", PQLTokenType::RELREF},
      {"w", PQLTokenType::SYNONYM},
      {"a", PQLTokenType::SYNONYM},
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"Modifies", PQLTokenType::RELREF},
      {"a", PQLTokenType::SYNONYM},
      {"x", PQLTokenType::IDENT}
  };

  std::vector<QueryToken> pattern_tokens_3 =  {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"w", PQLTokenType::PATTERN_WHILE},
      {"x", PQLTokenType::IDENT},
      {"_", PQLTokenType::WILDCARD}
  };

  std::vector<std::vector<QueryToken>>
      results_3 = QueryTokenizer::extractClauseTokens(sample_query_3, declarations_3);

  REQUIRE(results_3[0].size() == 8);
  REQUIRE(results_3[1].size() == 4);

  for (int i = 0; i < such_that_tokens_3.size(); i++) {
    REQUIRE(results_3[0][i].text == such_that_tokens_3[i].text);
    REQUIRE(results_3[0][i].type == such_that_tokens_3[i].type);
  }

  for (int i = 0; i < pattern_tokens_3.size(); i++) {
    REQUIRE(results_3[1][i].text == pattern_tokens_3[i].text);
    REQUIRE(results_3[1][i].type == pattern_tokens_3[i].type);
  }

  std::string sample_query_4 = "Select w pattern w (\"x\", _) and pattern w1 (\"y\", _) and w2 (\"z\", _)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_4, declarations_3), QpsSyntaxError);

  std::string sample_query_5 = "Select w such that Parent* (w, a) and such that Modifies (a, \"x\")";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_5, declarations_3), QpsSyntaxError);
}

TEST_CASE("Tokenizer can tokenize select tuple") {
  std::string sample_query_1 = "Select <s1, s2, s3> such that Parent (s1, s2)";
  std::vector<Declaration> declarations_1 = {
      {"s1", DesignEntity::STMT},
      {"s2", DesignEntity::STMT},
      {"s3", DesignEntity::STMT}
  };
  std::vector<QueryToken> select_tokens = {
      {"s1", PQLTokenType::SYNONYM},
      {"s2", PQLTokenType::SYNONYM},
      {"s3", PQLTokenType::SYNONYM}
  };

  std::vector<QueryToken> results_1 = QueryTokenizer::extractSelectToken(sample_query_1, declarations_1);
  for (int i = 0; i < results_1.size(); ++i) {
    REQUIRE(results_1[i].type == select_tokens[i].type);
    REQUIRE(results_1[i].text == select_tokens[i].text);
  }

  std::string sample_query_2 = "Select <s1,, s3> such that Parent (s1, s2)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractSelectToken(sample_query_2, declarations_1), QpsSyntaxError);

  std::string sample_query_3 = "Select s1, s2, s3> such that Parent (s1, s2)";
  REQUIRE_THROWS_AS(QueryTokenizer::extractSelectToken(sample_query_3, declarations_1), QpsSyntaxError);
}

TEST_CASE("Tokenizer can tokenize select attr ref") {
  std::string sample_query_1 = "Select <a1.stmt#, a2> such that Affects (a1, a2)";
  std::vector<Declaration> declarations_1 = {
      {"a1", DesignEntity::ASSIGN},
      {"a2", DesignEntity::ASSIGN}
  };
  std::vector<QueryToken> expected_select_tokens = {
      {"a1", PQLTokenType::WITH_STMTNO},
      {"a2", PQLTokenType::SYNONYM},
  };

  std::vector<QueryToken> results_1 = QueryTokenizer::extractSelectToken(sample_query_1, declarations_1);
  for (int i = 0; i < results_1.size(); ++i) {
    REQUIRE(results_1[i].type == expected_select_tokens[i].type);
    REQUIRE(results_1[i].text == expected_select_tokens[i].text);
  }
}

TEST_CASE("Tokenizer can tokenize select BOOLEAN") {
  std::string sample_query_1 = "Select BOOLEAN such that Parent (s1, s2)";
  std::vector<Declaration> declarations_1 = {
      {"s1", DesignEntity::STMT},
      {"s2", DesignEntity::STMT},
      {"s3", DesignEntity::STMT}
  };
  std::vector<QueryToken> select_tokens = {
      {"BOOLEAN", PQLTokenType::SELECT_BOOLEAN},
  };

  std::vector<QueryToken> results_1 = QueryTokenizer::extractSelectToken(sample_query_1, declarations_1);
  REQUIRE(results_1.size() == 1);
  REQUIRE(results_1[0].type == PQLTokenType::SYNONYM);
  REQUIRE(results_1[0].text == "BOOLEAN");

  std::string sample_query_2 = "stmt s1, s2; Select OOLEAN such that Parent (s1, s2)";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query_2), QpsSemanticError);
}

TEST_CASE("Test add parentheses for expressions") {
  std::string sample_query_1 = "x+y+z";
  std::string processed_expr = QueryUtil::addParentheses(sample_query_1);
  REQUIRE(processed_expr == "(((x)+(y))+(z))");

  std::string sample_query_2 = "(((x)))";
  std::string processed_expr_2 = QueryUtil::addParentheses(sample_query_2);
  REQUIRE(processed_expr_2 == "(x)");

  std::string sample_query_3 = "(x+y*z)";
  std::string processed_expr_3 = QueryUtil::addParentheses(sample_query_3);
  REQUIRE(processed_expr_3 == "((x)+((y)*(z)))");

  std::string sample_query_4 = "(((x+y*z)))";
  std::string processed_expr_4 = QueryUtil::addParentheses(sample_query_4);
  REQUIRE(processed_expr_4 == "((x)+((y)*(z)))");

  std::string sample_query_5 = "(x+y*(z-y*x)) ";
  std::string processed_expr_5 = QueryUtil::addParentheses(sample_query_5);
  REQUIRE(processed_expr_5 == "((x)+((y)*((z)-((y)*(x)))))");

  std::string sample_query_6 = "(((x+y))) ";
  std::string processed_expr_6 = QueryUtil::addParentheses(sample_query_6);
  REQUIRE(processed_expr_6 == "((x)+(y))");

  std::string sample_query_7 = "(((x))+y*((z)-y*(x))) ";
  std::string processed_expr_7 = QueryUtil::addParentheses(sample_query_7);
  REQUIRE(processed_expr_7 == "((x)+((y)*((z)-((y)*(x)))))");

  std::string sample_query_8 = "k-1+i*3/2";
  std::string processed_expr_8 = QueryUtil::addParentheses(sample_query_8);
  REQUIRE(processed_expr_8 == "(((k)-(1))+(((i)*(3))/(2)))");

  std::string sample_query_9 = "x%10";
  std::string processed_expr_9 = QueryUtil::addParentheses(sample_query_9);
  REQUIRE(processed_expr_9 == "((x)%(10))");

  std::string sample_query_10 = "abc+cde%fgh";
  std::string processed_expr_10 = QueryUtil::addParentheses(sample_query_10);
  REQUIRE(processed_expr_10 == "((abc)+((cde)%(fgh)))");
}

TEST_CASE("Tokeniser can tokenize with clause") {
  std::string sample_query_1 = "Select p such that Calls (\"Second\", p) and Parent (w, c) with c.procName = p.procName";
  std::vector<Declaration> declarations_1 = {
      {"p", DesignEntity::PROCEDURE},
      {"c", DesignEntity::CALL},
      {"w", DesignEntity::WHILE_LOOP}
  };

  std::vector<std::vector<QueryToken>> results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);
  std::vector<QueryToken> expected_with_tokens_1 = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"c", PQLTokenType::WITH_PROCNAME},
      {"p", PQLTokenType::WITH_PROCNAME}
  };

  std::vector<QueryToken> with_tokens_1 = results_1[2];
  REQUIRE(with_tokens_1.size() == 3);
  for (int i = 0; i < with_tokens_1.size(); i++) {
    REQUIRE(with_tokens_1[i].text == expected_with_tokens_1[i].text);
    REQUIRE(with_tokens_1[i].type == expected_with_tokens_1[i].type);
  }

  std::string sample_query_2 = "Select s.stmt# such that Follows* (s, s1) with s1.stmt#=10";
  std::vector<Declaration> declarations_2 = {
      {"s", DesignEntity::STMT},
      {"s1", DesignEntity::STMT},
  };

  std::vector<std::vector<QueryToken>> results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_2);
  std::vector<QueryToken> expected_with_tokens_2 = {
      {"", PQLTokenType::NORMAL_CLAUSE},
      {"s1", PQLTokenType::WITH_STMTNO},
      {"10", PQLTokenType::INTEGER}
  };

  std::vector<QueryToken> with_tokens_2 = results_2[2];
  REQUIRE(with_tokens_2.size() == 3);
  for (int i = 0; i < with_tokens_2.size(); i++) {
    REQUIRE(with_tokens_2[i].text == expected_with_tokens_2[i].text);
    REQUIRE(with_tokens_2[i].type == expected_with_tokens_2[i].type);
  }

  std::string sample_query_3 = "stmt s, s1; Select s.stmt# such that Follows* (s, s1) with s2.stmt#=10";
  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query_3), QpsSemanticError);

  std::string sample_query_4 = "Select s.stmt# such that Follows* (s, s1) with s1.smt#=10";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_4, declarations_2), QpsSyntaxError);

  std::string sample_query_5 = "Select s.stmt# such that Follows* (s, s1) with s1smt#=10";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_5, declarations_2), QpsSyntaxError);
}

TEST_CASE("Tokenizer can tokenize not clauses") {
  std::string sample_query_1 = "Select r.varName with not p.varName = r.varName and not 5=5 and not \"x\"=\"x\" and not p.varName = \"number\"";
  std::vector<Declaration> declarations_1 = {
      {"r", DesignEntity::READ},
      {"p", DesignEntity::PRINT}
  };
  std::vector<std::vector<QueryToken>> results_1 = QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1);

  std::vector<QueryToken> expected_with_tokens_1 = {
      {"not", PQLTokenType::NOT_CLAUSE},
      {"p", PQLTokenType::WITH_VARNAME},
      {"r", PQLTokenType::WITH_VARNAME},
      {"not", PQLTokenType::NOT_CLAUSE},
      {"5", PQLTokenType::INTEGER},
      {"5", PQLTokenType::INTEGER},
      {"not", PQLTokenType::NOT_CLAUSE},
      {"x", PQLTokenType::IDENT},
      {"x", PQLTokenType::IDENT},
      {"not", PQLTokenType::NOT_CLAUSE},
      {"p", PQLTokenType::WITH_VARNAME},
      {"number", PQLTokenType::IDENT}
  };
  std::vector<QueryToken> with_tokens_1 = results_1[2];
  REQUIRE(with_tokens_1.size() == 12);
  for (int i = 0; i < with_tokens_1.size(); i++) {
    REQUIRE(with_tokens_1[i].text == expected_with_tokens_1[i].text);
    REQUIRE(with_tokens_1[i].type == expected_with_tokens_1[i].type);
  }

  std::string sample_query_2 = "Select r.varName with not p.varName = r.varName such that not Parent*(s,  a) pattern not a(v,_\"digit \"_)";
  std::vector<Declaration> declarations_2 = {
      {"r", DesignEntity::READ},
      {"p", DesignEntity::PRINT},
      {"s", DesignEntity::STMT},
      {"a", DesignEntity::ASSIGN},
      {"v", DesignEntity::VARIABLE}
  };
  std::vector<std::vector<QueryToken>> results_2 = QueryTokenizer::extractClauseTokens(sample_query_2, declarations_2);

  std::vector<QueryToken> expected_with_tokens_2 = {
      {"not", PQLTokenType::NOT_CLAUSE},
      {"p", PQLTokenType::WITH_VARNAME},
      {"r", PQLTokenType::WITH_VARNAME},
  };

  std::vector<QueryToken> expected_such_that_tokens_2 = {
      {"not", PQLTokenType::NOT_CLAUSE},
      {"Parent*", PQLTokenType::RELREF},
      {"s", PQLTokenType::SYNONYM},
      {"a", PQLTokenType::SYNONYM}
  };

  std::vector<QueryToken> expected_pattern_tokens_2 = {
      {"not", PQLTokenType::NOT_CLAUSE},
      {"a", PQLTokenType::SYNONYM},
      {"v", PQLTokenType::SYNONYM},
      {"(digit)", PQLTokenType::PARTIALEXPR}
  };

  std::vector<QueryToken> with_tokens_2 = results_2[2];
  std::vector<QueryToken> such_that_tokens_2 = results_2[0];
  std::vector<QueryToken> pattern_tokens_2 = results_2[1];

  REQUIRE(with_tokens_2.size() == 3);
  REQUIRE(such_that_tokens_2.size() == 4);
  REQUIRE(pattern_tokens_2.size() == 4);
  for (int i = 0; i < with_tokens_2.size(); i++) {
    REQUIRE(with_tokens_2[i].text == expected_with_tokens_2[i].text);
    REQUIRE(with_tokens_2[i].type == expected_with_tokens_2[i].type);
  }
  for (int i = 0; i < such_that_tokens_2.size(); i++) {
    REQUIRE(such_that_tokens_2[i].text == expected_such_that_tokens_2[i].text);
    REQUIRE(such_that_tokens_2[i].type == expected_such_that_tokens_2[i].type);
  }
  for (int i = 0; i < pattern_tokens_2.size(); i++) {
    REQUIRE(pattern_tokens_2[i].text == expected_pattern_tokens_2[i].text);
    REQUIRE(pattern_tokens_2[i].type == expected_pattern_tokens_2[i].type);
  }
}

TEST_CASE("Tokeniser can handle invalid not clauses") {
  // 'not' coming before 'with'
  std::string sample_query_1 = "Select r.varName not with not p.varName = r.varName and not 5=5 and not \"x\"=\"x\" and not p.varName = \"number\"";
  std::vector<Declaration> declarations_1 = {
      {"r", DesignEntity::READ},
      {"p", DesignEntity::PRINT}
  };
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_1, declarations_1), QpsSyntaxError);

  // misspelled not
  std::string sample_query_2 = "Select r.varName with nottt p.varName = r.varName and not 5=5 and not \"x\"=\"x\" and not p.varName = \"number\"";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1), QpsSyntaxError);

  // double not
  std::string sample_query_3 = "Select r.varName with not not p.varName = r.varName and not 5=5 and not \"x\"=\"x\" and not p.varName = \"number\"";
  REQUIRE_THROWS_AS(QueryTokenizer::extractClauseTokens(sample_query_2, declarations_1), QpsSyntaxError);
}

TEST_CASE("debug") {
  std:: string sample_query = "assign a; variable v; Select a.procName pattern a(v,_) such that Affects*(5,a) and Uses(a,v) with v.varName=\"number\"";

  REQUIRE_THROWS_AS(QueryTokenizer::tokenize(sample_query), QpsSemanticError);
}
