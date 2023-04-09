include_guard()
include_directories( ${PROJECT_ROOT_DIR}/include )

set( LIBRARY_NAME worker)

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)

set( PROJECT_SRC_FILES streaming-worker.cpp streaming-worker-adapter.cpp)
set( PROJECT_INCLUDE_FILES streaming-worker.hpp streaming-worker-adapter.hpp)

add_library( ${LIBRARY_NAME} STATIC ${PROJECT_SRC_FILES})

target_include_directories( ${LIBRARY_NAME} PRIVATE /usr/include/node
                                                    ${NODE_ADDON_API_DIR})

target_link_directories( ${LIBRARY_NAME} PUBLIC ${PROJECT_INCLUDE_DIR})
