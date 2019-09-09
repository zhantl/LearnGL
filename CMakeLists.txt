cmake_minimum_required(VERSION 3.11.3)

project(vscode_test)

include_directories("./include")
link_directories("./libs")

add_subdirectory(src)

add_executable(vscode_test main.cpp)
target_link_libraries(vscode_test gltest_lib)

                    