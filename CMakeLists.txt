cmake_minimum_required(VERSION 3.15)
project(LogicAnalyzer VERSION 1.0.0 LANGUAGES CXX C)

option(BUILD_SAMPLE "Build C/C++ usage examples" ON)
option(BUILD_JS_INTERFACE "Build JS addon" ON)

if (BUILD_JS_INTERFACE)
    find_package(Git)

    if (GIT_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%an
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PROJECT_AUTHOR
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    else ()
        set(PROJECT_AUTHOR "")
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 17)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -fno-limit-debug-info")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Wpedantic -Wconversion")
endif ()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")
endif ()
add_subdirectory(src)
if (BUILD_JS_INTERFACE)
    add_subdirectory(JSLinkage)
endif ()
if (BUILD_SAMPLE)
    add_subdirectory(src/samples)
endif ()

include_directories(include)