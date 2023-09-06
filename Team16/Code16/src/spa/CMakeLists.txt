file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
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
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


