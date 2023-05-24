include_guard()

set( LIBRARY logic_analyzer)

add_library( ${LIBRARY} SHARED jsLogicAnalyzer/jsLogicAnalyzer.cpp 
             utility/builder/json-builder.cpp)

set_target_properties( ${LIBRARY} PROPERTIES PUBLIC_HEADER jsLogicAnalyzer/jsLogicAnalyzer.hpp)
set_target_properties( ${LIBRARY} PROPERTIES PUBLIC_HEADER utility/builder/json-builder.hpp)

execute_process( COMMAND npm install nan
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

execute_process( COMMAND node -e "require('nan')"
                 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                 OUTPUT_VARIABLE NODE_ADDON_API_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
string( PREPEND NODE_ADDON_API_DIR "${CMAKE_BINARY_DIR}/")

target_include_directories( ${LIBRARY} PRIVATE ${PROJECT_ROOT_DIR}/include 
                            /usr/include/node ${NODE_ADDON_API_DIR})
add_subdirectory( utility)
add_dependencies( ${LIBRARY} worker)

target_link_directories( ${LIBRARY} PUBLIC ${PROJECT_INCLUDE_FILES})
target_link_libraries( ${LIBRARY} PUBLIC worker analyzer)

set_target_properties(${LIBRARY} PROPERTIES PREFIX "" SUFFIX ".node")

set(LIBRARY_LOCATION "${CMAKE_CURRENT_BINARY_DIR}")
configure_file(jsLogicAnalyzer/template/index.js.in jsLogicAnalyzer/${LIBRARY}/index.js)
configure_file(jsLogicAnalyzer/template/package.json.in jsLogicAnalyzer/${LIBRARY}/package.json)

add_custom_command(
  TARGET ${LIBRARY}
  POST_BUILD
  COMMAND npm install "file:${CMAKE_CURRENT_BINARY_DIR}/jsLogicAnalyzer/${LIBRARY}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/jsLogicAnalyzer"
  VERBATIM)
