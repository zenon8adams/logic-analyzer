include_guard()
set(LIBRARY worker)

set(PROJECT_SRC_FILES streaming-worker.cpp streaming-worker-adapter.cpp)
set(PROJECT_INCLUDE_FILES streaming-worker.hpp streaming-worker-adapter.hpp pcqueue.hpp)

add_library(${LIBRARY} STATIC ${PROJECT_SRC_FILES})

target_include_directories(${LIBRARY} PUBLIC ${NODE_ADDON_API_DIR} ${CMAKE_SOURCE_DIR}/include)
find_path(NODE_INCLUDE_PATH NAMES node.h HINTS /usr/ /usr/local/ PATH_SUFFIXES include/node REQUIRED)
target_include_directories(${LIBRARY} SYSTEM PRIVATE ${NODE_INCLUDE_PATH})

target_link_directories(${LIBRARY} PUBLIC ${PROJECT_INCLUDE_FILES})
set_target_properties(${LIBRARY} PROPERTIES POSITION_INDEPENDENT_CODE ON)