include_guard()

set( LIBRARY_NAME js__conn)

add_library( ${LIBRARY_NAME} SHARED jsLogicAnalyzer/jsLogicAnalyzer.cpp utility/builder/json-builder.cpp)

set_target_properties( ${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER jsLogicAnalyzer/jsLogicAnalyzer.hpp)
set_target_properties( ${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER utility/builder/json-builder.hpp)

target_include_directories( ${LIBRARY_NAME} PRIVATE /usr/include/node)
include_directories( ${LIBRARY_NAME} /usr/include/node ${PROJECT_ROOT_DIR}/include)

add_subdirectory( utility)
add_dependencies( ${LIBRARY_NAME} worker)

target_link_directories( ${LIBRARY_NAME} PUBLIC ${PROJECT_INCLUDE_FILES})
target_link_libraries( ${LIBRARY_NAME} PUBLIC worker)