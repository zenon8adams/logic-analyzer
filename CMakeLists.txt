cmake_minimum_required(VERSION 3.15)
project(LogicAnalyzer)

set( CMAKE_CXX_STANDARD 17)
set( PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR} )

add_subdirectory( src)
add_subdirectory( JSLinkage )
add_subdirectory( src/samples )

include_directories( include )

add_executable( logic-analyzer src/samples/usage.cpp)
add_dependencies( logic-analyzer analyzer)
target_link_libraries( logic-analyzer PUBLIC analyzer )
