file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/qps/QueryEvaluator.cpp
        src/qps/QueryEvaluator.h
        src/qps/ParsedQuery.cpp
        src/qps/ParsedQuery.h
        src/qps/Declaration.cpp
        src/qps/Declaration.h
        src/qps/DesignEntity.cpp
        src/qps/DesignEntity.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


