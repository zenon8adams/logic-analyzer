include_guard()

set(LIBRARY logic_analyzer)

add_library(${LIBRARY} SHARED jsLogicAnalyzer/jsLogicAnalyzer.cpp
        utility/builder/json-builder.cpp)

set_target_properties(${LIBRARY} PROPERTIES PUBLIC_HEADER jsLogicAnalyzer/jsLogicAnalyzer.hpp)
set_target_properties(${LIBRARY} PROPERTIES PUBLIC_HEADER utility/builder/json-builder.hpp)

execute_process(COMMAND npm install nan
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

execute_process(COMMAND node -e "require('nan')"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
string(PREPEND NODE_ADDON_API_DIR "${CMAKE_BINARY_DIR}/")

target_include_directories(${LIBRARY} PRIVATE ${PROJECT_ROOT_DIR}/include
        ${NODE_ADDON_API_DIR})
find_path(NODE_INCLUDE_PATH NAMES node.h HINTS /usr/ /usr/local/ PATH_SUFFIXES include/node REQUIRED)
target_include_directories(${LIBRARY} SYSTEM PRIVATE ${NODE_INCLUDE_PATH})

add_subdirectory(utility)
add_dependencies(${LIBRARY} worker)

target_link_libraries(${LIBRARY} PUBLIC worker analyzer)
set_target_properties(${LIBRARY} PROPERTIES PREFIX "" SUFFIX ".node")

## Set properties to inject into Nodejs-Module required to load the addon
set(LIBRARY_LOCATION "${CMAKE_CURRENT_BINARY_DIR}")
configure_file(jsLogicAnalyzer/template/index.js.in jsLogicAnalyzer/${LIBRARY}/index.js)
configure_file(jsLogicAnalyzer/template/package.json.in jsLogicAnalyzer/${LIBRARY}/package.json)

add_custom_command(
        TARGET ${LIBRARY}
        POST_BUILD
        COMMAND npm install "file:${CMAKE_CURRENT_BINARY_DIR}/jsLogicAnalyzer/${LIBRARY}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/jsLogicAnalyzer"
        VERBATIM)
