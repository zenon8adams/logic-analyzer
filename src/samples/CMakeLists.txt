include_guard()

add_executable(logic-analyzer-cc usage.cpp)
target_include_directories(logic-analyzer-cc PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(logic-analyzer-cc PUBLIC analyzer)

add_executable(logic-analyzer-c usage.c)
target_include_directories(logic-analyzer-c PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(logic-analyzer-c PUBLIC analyzer)