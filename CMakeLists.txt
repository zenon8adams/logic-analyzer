cmake_minimum_required(VERSION 3.15)
project(LogicAnalyzer)

set( CMAKE_CXX_STANDARD 17)
set( PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR} )

add_subdirectory( JSLinkage )
add_subdirectory( src)
add_subdirectory( src/samples )

include_directories( include )

add_executable( logic-analyzer src/samples/usage.cpp)
add_dependencies( logic-analyzer analyzer)
target_link_libraries( logic-analyzer PUBLIC analyzer )

#[[execute_process( COMMAND node -p "require('node-addon-api').include"
                 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                 OUTPUT_VARIABLE NODE_ADDON_API_DIR )
string( REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string( REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})]]
