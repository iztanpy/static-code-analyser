file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/SP/SimpleParser.cpp
        src/SP/SimpleParser.h
        src/SP/SPTokeniser.cpp
        src/SP/SPTokeniser.h
        src/SP/TokenTypes.h
        src/SP/Token.h
        src/SP/Visitor.cpp
        src/SP/Visitor.h
        src/SP/DesignExtractor.h
        src/SP/Visitable.h
        src/SP/DesignExtractor.cpp
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


