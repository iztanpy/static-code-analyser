file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs}
        src/TestClauseDirector.cpp
        src/TestQueryTokenizer.cpp
        src/TestQueryParser.cpp
        src/PKB/PKB.cpp
        src/TestTokeniser.cpp
        src/TestSimpleParser.cpp
        src/test_constraint_table.cpp
        src/PKB/FollowsStoreTest.cpp
        src/TestWhileParser.cpp
        src/TestSemanticValidator.cpp
        src/TestStringUtil.cpp
        src/TestWhileParser.cpp
)

target_link_libraries(unit_testing spa)

