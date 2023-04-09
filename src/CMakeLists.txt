include_guard()

include_directories( ${PROJECT_ROOT_DIR}/include )

set( LIBRARY_NAME analyzer)

set( PROJECT_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/binary-counter.cpp
                       ${CMAKE_CURRENT_LIST_DIR}/context.cpp
                       ${CMAKE_CURRENT_LIST_DIR}/logical-expression.cpp
                       ${CMAKE_CURRENT_LIST_DIR}/postfix-gen.cpp
                       ${CMAKE_CURRENT_LIST_DIR}/utility.cpp )

set( PROJECT_INCLUDE_FILES ${PROJECT_ROOT_DIR}/include/binary-counter.hpp
                           ${PROJECT_ROOT_DIR}/include/context.hpp
                           ${PROJECT_ROOT_DIR}/include/logical-expression.hpp
                           ${PROJECT_ROOT_DIR}/include/postfix-gen.hpp
                           ${PROJECT_ROOT_DIR}/include/utility.hpp )
add_subdirectory( model )

add_library( ${LIBRARY_NAME} SHARED ${PROJECT_SRC_FILES} )
target_link_directories( ${LIBRARY_NAME} PUBLIC ${PROJECT_INCLUDE_FILES})
set_target_properties( ${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER ${PROJECT_INCLUDE_FILES} )

add_custom_command( TARGET ${LIBRARY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND}
                    -E copy $<TARGET_FILE:${LIBRARY_NAME}> 
                    "${CMAKE_SOURCE_DIR}/JSLinkage/jsLogicAnalyzer/build/")
