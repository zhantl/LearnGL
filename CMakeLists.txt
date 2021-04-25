cmake_minimum_required(VERSION 3.11.3)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(vscode_test)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories("./include")
link_directories("./libs")

add_subdirectory(src)



execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${EXECUTABLE_OUTPUT_PATH}/res) 
add_executable(vscode_test main.cpp)
target_link_libraries(vscode_test gltest_lib)


