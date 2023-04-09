include_guard()

set( LIBRARY js__conn)

add_library( ${LIBRARY} SHARED jsLogicAnalyzer/jsLogicAnalyzer.cpp 
             utility/builder/json-builder.cpp)

set_target_properties( ${LIBRARY} PROPERTIES PUBLIC_HEADER jsLogicAnalyzer/jsLogicAnalyzer.hpp)
set_target_properties( ${LIBRARY} PROPERTIES PUBLIC_HEADER utility/builder/json-builder.hpp)

execute_process( COMMAND npm install nan
                 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

execute_process( COMMAND node -e "require('nan')"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                 OUTPUT_VARIABLE NODE_ADDON_API_DIR)
string( PREPEND NODE_ADDON_API_DIR "${CMAKE_CURRENT_LIST_DIR}/")

target_include_directories( ${LIBRARY} PRIVATE ${PROJECT_ROOT_DIR}/include 
                            /usr/include/node ${NODE_ADDON_API_DIR})
add_subdirectory( utility)
add_dependencies( ${LIBRARY} worker)

target_link_directories( ${LIBRARY} PUBLIC ${PROJECT_INCLUDE_FILES})
target_link_libraries( ${LIBRARY} PUBLIC worker)

add_custom_target( CREATE_ADDON_DIR ALL
                   COMMAND ${CMAKE_COMMAND} -E make_directory build_deps
                   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/jsLogicAnalyzer/")

add_dependencies( CREATE_ADDON_DIR ${LIBRARY})

add_custom_target( COPY_ADDON_LIBRARY_${LIBRARY} ALL
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBRARY}> 
                   "${CMAKE_CURRENT_SOURCE_DIR}/jsLogicAnalyzer/build_deps/")

add_dependencies( COPY_ADDON_LIBRARY_${LIBRARY} 
                  ${LIBRARY} CREATE_ADDON_DIR)

add_custom_target( BUILD_ADDON ALL 
                   COMMAND npm install
                   WORKING_DIRECTORY 
                   "${CMAKE_CURRENT_SOURCE_DIR}/jsLogicAnalyzer")

add_dependencies( BUILD_ADDON 
                  COPY_ADDON_LIBRARY_${LIBRARY} 
                  ${LIBARAY_NAME} CREATE_ADDON_DIR)
