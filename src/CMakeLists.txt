cmake_minimum_required(VERSION 3.11.3)

file(GLOB_RECURSE cxx_files *.cpp *.c)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
add_library(gltest_lib ${cxx_files})
target_link_libraries(gltest_lib 
                      freetype
                      glfw3 
                      assimp-vc140-mt 
                    )
