include_guard()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(LIBRARY analyzer)

set(PROJECT_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/binary-counter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/context.cpp
        ${CMAKE_CURRENT_LIST_DIR}/logical-expression.cpp
        ${CMAKE_CURRENT_LIST_DIR}/postfix-gen.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utility.cpp)

set(PROJECT_INCLUDE_FILES ${PROJECT_ROOT_DIR}/include/binary-counter.hpp
        ${PROJECT_ROOT_DIR}/include/context.hpp
        ${PROJECT_ROOT_DIR}/include/logical-expression.hpp
        ${PROJECT_ROOT_DIR}/include/postfix-gen.hpp
        ${PROJECT_ROOT_DIR}/include/utility.hpp)
add_subdirectory(model)

add_library(${LIBRARY} SHARED ${PROJECT_SRC_FILES})
target_link_directories(${LIBRARY} PUBLIC ${PROJECT_INCLUDE_FILES})
set_target_properties(${LIBRARY} PROPERTIES PUBLIC_HEADER ${PROJECT_INCLUDE_FILES})