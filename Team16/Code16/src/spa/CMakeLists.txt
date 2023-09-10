file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/qps/parsed_query.h
        src/qps/declaration.cpp
        src/qps/declaration.h
        src/qps/design_entity.cpp
        src/qps/design_entity.h
        src/qps/clause_director.cpp
        src/qps/clause_director.h
        src/qps/select_clause.h
        src/qps/clause_builder.h
        src/qps/select_clause_builder.cpp
        src/qps/select_clause_builder.h
        src/qps/query_tokenizer.cpp
        src/qps/query_tokenizer.h
        src/qps/query_parser.cpp
        src/qps/query_parser.h
        src/qps/clause.h
        src/qps/select_clause.cpp
        src/qps/query_evaluator/constraint_solver.cpp
        src/qps/query_evaluator/constraint_solver.h
        src/qps/query_evaluator/constraint.h
        src/qps/query_evaluator/select_evaluator.cpp
        src/qps/query_evaluator/select_evaluator.h
        src/qps/query_evaluator/suchthat_evaluator.cpp
        src/qps/query_evaluator/suchthat_evaluator.h
        src/PKB/pkb.h
        src/qps/query_evaluator/usesS_evaluator.cpp
        src/qps/query_evaluator/usesS_evaluator.h
        src/qps/suchthat_clause.cpp
        src/qps/suchthat_clause.h
        src/qps/query_evaluator/query_evaluator.cpp
        src/qps/query_evaluator/query_evaluator.h
        src/utils/hash_utils.h
        src/PKB/PKB.cpp
        src/PKB/PKB.h
        src/PKB/API/WriteFacade.cpp
        src/PKB/API/WriteFacade.h
        src/PKB/API/ReadFacade.h
        src/PKB/API/ReadFacade.cpp
        src/SP/SimpleParser.cpp
        src/SP/SimpleParser.h
        src/SP/SPTokeniser.cpp
        src/SP/SPTokeniser.h
        src/SP/TokenTypes.h
        src/SP/Visitor.cpp
        src/SP/Visitor.h
        src/SP/DesignExtractor.h
        src/SP/DesignExtractor.cpp
        src/SP/TNode.cpp
        src/SP/Token.h
        src/PKB/PKB.cpp
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


