file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/qps/query_parser/parsed_query.h
        src/qps/declaration.h
        src/qps/design_entity.cpp
        src/qps/design_entity.h
        src/qps/query_parser/clause_builder/clause_director.cpp
        src/qps/query_parser/clause_builder/clause_director.h
        src/qps/clauses/select_clause.h
        src/qps/query_parser/clause_builder/clause_builder.h
        src/qps/query_parser/clause_builder/select_clause_builder.cpp
        src/qps/query_parser/clause_builder/select_clause_builder.h
        src/qps/query_parser/query_tokenizer/query_tokenizer.cpp
        src/qps/query_parser/query_tokenizer/query_tokenizer.h
        src/qps/query_parser/query_parser.cpp
        src/qps/query_parser/query_parser.h
        src/qps/clauses/clause.h
        src/qps/clauses/select_clause.cpp
        src/qps/query_evaluator/constraint_solver/constraint.h
        src/PKB/pkb.h
        src/qps/clauses/suchthat_clauses/suchthat_clause.cpp
        src/qps/clauses/suchthat_clauses/suchthat_clause.h
        src/qps/query_evaluator/query_evaluator.cpp
        src/qps/query_evaluator/query_evaluator.h
        src/utils/hash_utils.h
        src/PKB/pkb.cpp
        src/PKB/pkb.h
        src/PKB/API/write_facade.cpp
        src/PKB/API/write_facade.h
        src/PKB/API/read_facade.h
        src/PKB/API/read_facade.cpp
        src/SP/sp_parser/simple_parser.h
        src/SP/sp_parser/simple_parser.cpp
        src/SP/sp_tokeniser/sp_tokeniser.cpp
        src/SP/sp_tokeniser/sp_tokeniser.h
        src/SP/sp_tokeniser/token_types.h
        src/SP/visitor.cpp
        src/SP/visitor.h
        src/SP/design_extractor.h
        src/SP/design_extractor.cpp
        src/SP/tnode.cpp
        src/SP/sp_tokeniser/token.h
        src/PKB/pkb.cpp
        src/qps/qps.cpp
        src/qps/qps.h
        src/qps/query_evaluator/constraint_solver/constraint_table.cpp
        src/qps/query_evaluator/constraint_solver/constraint_table.h
        src/utils/error.cpp
        src/utils/error.h
        src/qps/qps_errors/qps_error.h
        src/qps/qps_errors/qps_semantic_error.cpp
        src/qps/qps_errors/qps_semantic_error.h
        src/qps/qps_errors/qps_syntax_error.cpp
        src/qps/qps_errors/qps_syntax_error.h
        src/qps/rel_ref.cpp
        src/qps/rel_ref.h
        src/qps/query_parser/clause_builder/suchthat_clause_builder.cpp
        src/qps/query_parser/clause_builder/suchthat_clause_builder.h
        src/PKB/Stores/variable_store.cpp
        src/PKB/Stores/variable_store.h
        src/PKB/Stores/constant_store.cpp
        src/PKB/Stores/constant_store.h
        src/PKB/Stores/assign_store.cpp
        src/PKB/Stores/assign_store.h
        src/PKB/Stores/statement_store.cpp
        src/PKB/Stores/statement_store.h
        src/PKB/Stores/assign_store.cpp
        src/PKB/Stores/relation_store.cpp
        src/PKB/Stores/relation_store.h
        src/PKB/Stores/follows_store.cpp
        src/PKB/Stores/follows_store.h
        src/PKB/Stores/call_store.h
        src/PKB/Stores/call_store.cpp
        src/utils/error.cpp
        src/utils/error.h
        src/qps/qps_errors/qps_error.h
        src/qps/qps_errors/qps_semantic_error.cpp
        src/qps/qps_errors/qps_semantic_error.h
        src/qps/qps_errors/qps_syntax_error.cpp
        src/qps/qps_errors/qps_syntax_error.h
        src/qps/rel_ref.cpp
        src/qps/rel_ref.h
        src/qps/query_parser/clause_builder/suchthat_clause_builder.cpp
        src/qps/query_parser/clause_builder/suchthat_clause_builder.h
        src/PKB/Stores/parent_store.cpp
        src/PKB/Stores/parent_store.h
        src/utils/entity_types.h
        src/SP/sp_parser/assignment_parser.cpp
        src/SP/sp_parser/assignment_parser.h
        src/SP/sp_parser/read_parser.cpp
        src/SP/sp_parser/read_parser.h
        src/SP/sp_parser/procedure_parser.cpp
        src/SP/sp_parser/procedure_parser.h
        src/SP/sp_parser/parser.h
        src/SP/source_processor.h
        src/SP/source_processor.cpp
        src/SP/sp_parser/while_parser.cpp
        src/SP/sp_parser/while_parser.h
        src/SP/sp_parser/print_parser.cpp
        src/SP/sp_parser/print_parser.h
        src/SP/sp_parser/if_parser.h
        src/SP/sp_parser/if_parser.cpp
        src/SP/sp_parser/call_parser.cpp
        src/SP/sp_parser/call_parser.h
        src/SP/sp_parser/parser_factory.h
        src/SP/sp_parser/parser_factory.cpp
        src/SP/sp_parser/call_parser.cpp
        src/SP/sp_parser/call_parser.h
        src/SP/sp_parser/parser.cpp
        src/SP/sp_parser/else_parser.cpp
        src/SP/sp_parser/else_parser.h
        src/SP/sp_parser/close_brace_parser.cpp
        src/SP/sp_parser/close_brace_parser.h
        src/SP/sp_cfg/cfg.cpp
        src/SP/sp_cfg/cfg.h
        src/SP/sp_cfg/cfg_node.h
        src/qps/clauses/pattern_clauses/pattern_clause.cpp
        src/qps/clauses/pattern_clauses/pattern_clause.h
        src/qps/query_parser/clause_builder/pattern_clause_builder.cpp
        src/qps/query_parser/clause_builder/pattern_clause_builder.h
        src/utils/string_utils.cpp
        src/utils/string_utils.h
        src/utils/lexical_utils.cpp
        src/utils/lexical_utils.h
        src/qps/constants.h
        src/qps/query_parser/QueryUtil.cpp
        src/qps/query_parser/QueryUtil.h
        src/SP/utils/parse_utils.cpp
        src/SP/utils/parse_utils.h
        src/utils/clauses_types.h
        src/qps/clauses/suchthat_clauses/suchthat_clauses_all.h
        src/qps/clauses/suchthat_clauses/uses.cpp
        src/qps/clauses/suchthat_clauses/uses.h
        src/qps/clauses/suchthat_clauses/modifies.cpp
        src/qps/clauses/suchthat_clauses/modifies.h
        src/qps/clauses/suchthat_clauses/parent.cpp
        src/qps/clauses/suchthat_clauses/parent.h
        src/qps/clauses/suchthat_clauses/follows.cpp
        src/qps/clauses/suchthat_clauses/follows.h
        src/qps/clauses/suchthat_clauses/suchthat_validator.cpp
        src/qps/clauses/suchthat_clauses/suchthat_validator.h
        src/qps/query_evaluator/uses_evaluator.cpp
        src/qps/query_evaluator/uses_evaluator.h
        src/qps/query_evaluator/modifies_evaluator.cpp
        src/qps/query_evaluator/modifies_evaluator.h
        src/qps/query_evaluator/follows_evaluator.cpp
        src/qps/query_evaluator/follows_evaluator.h
        src/qps/query_evaluator/parent_evaluator.cpp
        src/qps/query_evaluator/parent_evaluator.h
        src/qps/clauses/pattern_clauses/pattern_validator.cpp
        src/qps/clauses/pattern_clauses/pattern_validator.h
        src/PKB/Stores/procedure_store.cpp
        src/PKB/Stores/procedure_store.cpp
        src/qps/query_evaluator/evaluator_util.cpp
        src/qps/query_evaluator/evaluator_util.h
        src/qps/query_evaluator/assign_pattern_evaluator.cpp
        src/qps/query_evaluator/assign_pattern_evaluator.h
        src/PKB/Stores/if_store.cpp
        src/PKB/Stores/if_store.h
        src/PKB/Stores/while_store.cpp
        src/PKB/Stores/while_store.h
        src/qps/qps_validator/qps_validator_handler.h
        src/qps/qps_validator/qps_validator.cpp
        src/qps/qps_validator/qps_validator.h
        src/qps/qps_validator/statement_syntax_handler.cpp
        src/qps/qps_validator/statement_syntax_handler.h
        src/qps/qps_validator/declaration_statement_syntax_handler.cpp
        src/qps/qps_validator/declaration_statement_syntax_handler.h
        src/qps/qps_validator/declaration_synonym_syntax_handler.cpp
        src/qps/qps_validator/declaration_synonym_syntax_handler.h
        src/qps/qps_validator/declaration_synonym_semantic_handler.cpp
        src/qps/qps_validator/declaration_synonym_semantic_handler.h
        src/qps/qps_validator/select_statement_syntax_handler.cpp
        src/qps/qps_validator/select_statement_syntax_handler.h
        src/qps/qps_validator/select_synonym_syntax_handler.cpp
        src/qps/qps_validator/select_synonym_syntax_handler.h
        src/qps/qps_validator/select_synonym_semantic_handler.cpp
        src/qps/qps_validator/select_synonym_semantic_handler.h
        src/qps/qps_validator/clause_args_syntax_handler.cpp
        src/qps/qps_validator/clause_args_syntax_handler.h
        src/qps/qps_validator/clause_synonym_semantic_handler.cpp
        src/qps/qps_validator/clause_synonym_semantic_handler.h
        src/qps/clauses/suchthat_clauses/calls.cpp
        src/qps/clauses/suchthat_clauses/calls.h
        src/qps/query_evaluator/calls_evaluator.cpp
        src/qps/query_evaluator/calls_evaluator.h
        src/qps/clauses/suchthat_clauses/next.cpp
        src/qps/clauses/suchthat_clauses/next.h
        src/qps/clauses/suchthat_clauses/affects.cpp
        src/qps/clauses/suchthat_clauses/affects.h
        src/qps/qps_validator/pattern_syn_semantic_handler.cpp
        src/qps/qps_validator/pattern_syn_semantic_handler.h
        src/qps/qps_validator/select_tuple_synonym_semantic_handler.cpp
        src/qps/qps_validator/select_tuple_synonym_semantic_handler.h
        src/qps/query_evaluator/constraint_solver/clause_group.cpp
        src/qps/query_evaluator/constraint_solver/clause_group.h
        src/qps/query_evaluator/constraint_solver/clause_grouper.cpp
        src/qps/query_evaluator/constraint_solver/clause_grouper.h
        src/PKB/Stores/next_store.cpp
        src/PKB/Stores/next_store.h
        src/qps/query_evaluator/if_evaluator.cpp
        src/qps/query_evaluator/if_evaluator.h
        src/qps/query_evaluator/while_evaluator.cpp
        src/qps/query_evaluator/while_evaluator.h
        src/qps/query_evaluator/next_evaluator.h
        src/qps/query_evaluator/next_evaluator.cpp
        src/qps/clauses/with_clauses/with_clause.h
        src/qps/clauses/with_clauses/with_clause.cpp
        src/qps/query_parser/clause_builder/with_clause_builder.cpp
        src/qps/query_parser/clause_builder/with_clause_builder.h
        src/qps/qps_validator/WithClauseSyntaxHandler.cpp
        src/qps/qps_validator/WithClauseSyntaxHandler.h
        src/qps/clauses/attr_ref.h
        src/qps/clauses/attr_ref.cpp
        src/qps/query_evaluator/with_evaluator.cpp
        src/qps/query_evaluator/with_evaluator.h
        src/utils/algorithm_utils.h
        src/qps/query_evaluator/affects_evaluator.cpp
        src/qps/query_evaluator/affects_evaluator.h
        src/qps/clauses/pattern_clauses/if.cpp
        src/qps/clauses/pattern_clauses/if.h
        src/qps/clauses/pattern_clauses/while.cpp
        src/qps/clauses/pattern_clauses/while.h
        src/qps/clauses/pattern_clauses/assign.cpp
        src/qps/clauses/pattern_clauses/assign.h
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


