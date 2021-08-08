include_guard()

include_directories( ${PROJECT_ROOT_DIR}/include )

set( PROJECT_SRC_FILES ${PROJECT_SRC_FILES}
                       ${CMAKE_CURRENT_LIST_DIR}/logic-model.cpp
                       ${CMAKE_CURRENT_LIST_DIR}/table-model.cpp
                       PARENT_SCOPE )

set( PROJECT_INCLUDE_FILES ${PROJECT_INCLUDE_FILES}
                           ${PROJECT_ROOT_DIR}/include/logic-model.hpp
                           ${PROJECT_ROOT_DIR}/include/table-model.hpp
                           PARENT_SCOPE )