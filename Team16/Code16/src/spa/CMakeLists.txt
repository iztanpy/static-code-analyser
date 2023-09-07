file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/qps/query_evaluator.cpp
        src/qps/query_evaluator.h
        src/qps/parsed_query.cpp
        src/qps/parsed_query.h
        src/qps/declaration.cpp
        src/qps/declaration.h
        src/qps/design_entity.cpp
        src/qps/design_entity.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


