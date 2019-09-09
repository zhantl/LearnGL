file(GLOB_RECURSE cxx_files *.cpp *.c)
message("file list" ${cxx_files})
add_library(gltest_lib ${cxx_files})
target_link_libraries(gltest_lib 
                      freetype
                      glfw3 
                      assimp-vc140-mt 
                    )
