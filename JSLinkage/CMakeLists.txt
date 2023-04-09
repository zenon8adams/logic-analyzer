include_guard()

set( LIBRARY_NAME js__conn)

add_library( ${LIBRARY_NAME} SHARED jsLogicAnalyzer/jsLogicAnalyzer.cpp 
             utility/builder/json-builder.cpp)

set_target_properties( ${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER jsLogicAnalyzer/jsLogicAnalyzer.hpp)
set_target_properties( ${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER utility/builder/json-builder.hpp)

execute_process( COMMAND node -e "require('nan')"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                 OUTPUT_VARIABLE NODE_ADDON_API_DIR)
string( PREPEND NODE_ADDON_API_DIR "${CMAKE_CURRENT_LIST_DIR}/")

target_include_directories( ${LIBRARY_NAME} PRIVATE ${PROJECT_ROOT_DIR}/include 
                            /usr/include/node ${NODE_ADDON_API_DIR})
add_subdirectory( utility)
add_dependencies( ${LIBRARY_NAME} worker)

target_link_directories( ${LIBRARY_NAME} PUBLIC ${PROJECT_INCLUDE_FILES})
target_link_libraries( ${LIBRARY_NAME} PUBLIC worker)

add_custom_command( TARGET ${LIBRARY_NAME} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND}
                    -E make_directory build
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/jsLogicAnalyzer/")

add_custom_command( TARGET ${LIBRARY_NAME} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND}
                    -E copy $<TARGET_FILE:${LIBRARY_NAME}> 
                    "${CMAKE_CURRENT_SOURCE_DIR}/jsLogicAnalyzer/build/")

add_custom_command( TARGET ${LIBRARY_NAME} POST_BUILD 
                    COMMAND node-gyp configure
                    COMMAND node-gyp build
                    WORKING_DIRECTORY 
                    "${CMAKE_CURRENT_SOURCE_DIR}/jsLogicAnalyzer")
