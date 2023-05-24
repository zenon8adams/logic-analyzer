cmake_minimum_required(VERSION 3.15)
project(LogicAnalyzer VERSION 1.0.0 LANGUAGES CXX)

find_package(Git)

if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%an
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_AUTHOR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(PROJECT_AUTHOR "")
endif()

set( CMAKE_CXX_STANDARD 17)
set( PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR} )

add_subdirectory( src)
add_subdirectory( JSLinkage )
add_subdirectory( src/samples )

include_directories( include )

add_executable( logic-analyzer src/samples/usage.cpp)
add_dependencies( logic-analyzer analyzer)
target_link_libraries( logic-analyzer PUBLIC analyzer )
