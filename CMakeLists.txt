cmake_minimum_required(VERSION 3.11.3)
project(vscode_test)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/glfw-3.3)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/assimp-3.3.1)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/freetype-2.10.0)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

include_directories("./include")

set(SCR_FILE
		src/camera.h
		src/main.cpp
	)
source_group("src" FILES ${SCR_FILE})

file(GLOB FT_GL_FILE src/freetype-gl/*.c)
source_group("src\\freetype-gl" FILES ${FT_GL_FILE})

file(GLOB GLAD_FILE src/glad/glad.c)
source_group("src\\glad" FILES ${GLAD_FILE})

file(GLOB STB_FILE src/stb_image/stb_image.cpp)
source_group("src\\stb_image" FILES ${STB_FILE})

set(GRAPHY_FILE
		src/graphics/event_dispatch.hpp
		src/graphics/event_dispatch.cpp
        src/graphics/fileUtils.cpp
        src/graphics/mesh.cpp
        src/graphics/model.cpp
        src/graphics/shader.cpp
        src/graphics/window.cpp
	)
source_group("src\\graphics" FILES ${GRAPHY_FILE})

set(BUFFER_FILE
		src/graphics/buffers/framebuffer.hpp
		src/graphics/buffers/framebuffer.cpp
        src/graphics/buffers/indexbuffer.hpp
        src/graphics/buffers/indexbuffer.cpp
	)
source_group("src\\graphics\\buffers" FILES ${BUFFER_FILE})

file(GLOB RENDER_FILE 
        src/graphics/render/*.cpp
        src/graphics/render/*.hpp
        )
source_group("src\\graphics\\render" FILES ${RENDER_FILE})

file(GLOB TEXTURE_FILE 
        src/graphics/texture/*.cpp
        src/graphics/texture/*.hpp
        )
source_group("src\\graphics\\texture" FILES ${TEXTURE_FILE})


add_executable(${PROJECT_NAME} ${SCR_FILE} ${FT_GL_FILE}
                                ${GLAD_FILE} ${STB_FILE}
                                ${GRAPHY_FILE} ${BUFFER_FILE}
                                ${RENDER_FILE}  ${TEXTURE_FILE})

target_link_libraries(${PROJECT_NAME} freetype glfw assimp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

message("command---- ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)


