cmake_minimum_required(VERSION 3.15)
project(LogicAnalyzer)

set( CMAKE_CXX_STANDARD 17)
set( PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR} )

include_directories( ${CMAKE_JS_INC})

add_subdirectory(src)
add_subdirectory( src/samples )
include_directories( include )

target_link_directories( analyzer PUBLIC ${CMAKE_BINARY_DIR}/src )
target_link_libraries( analyzer ${CMAKE_JS_LIB} )

execute_process( COMMAND node -p "require('node-addon-api').include"
                 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                 OUTPUT_VARIABLE NODE_ADDON_API_DIR )
string( REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string( REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
