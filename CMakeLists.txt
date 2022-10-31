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
		src/Camera.h
		src/main.cpp
	)
source_group("src" FILES ${SCR_FILE})

set(FT_GL_FILE 
    src/freetype-gl/distance-field.c
    src/freetype-gl/edtaa3func.c
    src/freetype-gl/platform.c
    src/freetype-gl/texture-atlas.c
    src/freetype-gl/texture-font.c
    src/freetype-gl/utf8-utils.c
    src/freetype-gl/vector.c
    )
source_group("src\\freetype-gl" FILES ${FT_GL_FILE})

set(GLAD_FILE 
    src/glad/glad.c
    )
source_group("src\\glad" FILES ${GLAD_FILE})

set(STB_FILE 
    src/stb_image/stb_image.cpp
    )
source_group("src\\stb_image" FILES ${STB_FILE})

set(GRAPHY_FILE
		src/graphics/EventDispatch.cpp
		src/graphics/EventDispatch.hpp
		src/graphics/FileUtils.cpp
		src/graphics/FileUtils.hpp
		src/graphics/Shader.cpp
		src/graphics/Shader.hpp
		src/graphics/Window.cpp
		src/graphics/Window.hpp
	)
source_group("src\\graphics" FILES ${GRAPHY_FILE})

set(MODEL_FILE
        src/graphics/model/AssimpHelper.hpp
		src/graphics/model/Mesh.cpp
		src/graphics/model/Mesh.hpp
		src/graphics/model/Model.cpp
		src/graphics/model/Model.hpp
		src/graphics/model/ModelAnimation.cpp
		src/graphics/model/ModelAnimation.hpp
	)
source_group("src\\graphics\\model" FILES ${MODEL_FILE})

set(BUFFER_FILE
		src/graphics/buffers/FrameBuffer.cpp
		src/graphics/buffers/FrameBuffer.hpp
		src/graphics/buffers/IndexBuffer.cpp
		src/graphics/buffers/IndexBuffer.hpp
	)
source_group("src\\graphics\\buffers" FILES ${BUFFER_FILE})

set(RENDER_FILE 
        src/graphics/render/Cube.cpp
        src/graphics/render/Cube.hpp
        src/graphics/render/Label.cpp
        src/graphics/render/Label.hpp
        src/graphics/render/Node.cpp
        src/graphics/render/Node.hpp
        src/graphics/render/RenderToCube.cpp
        src/graphics/render/RenderToCube.hpp
        src/graphics/render/SkyBox.cpp
        src/graphics/render/SkyBox.hpp
        src/graphics/render/Sphere.cpp
        src/graphics/render/Sphere.hpp
        src/graphics/render/Sprite.cpp
        src/graphics/render/Sprite.hpp
        )
source_group("src\\graphics\\render" FILES ${RENDER_FILE})

set(TEXTURE_FILE 
        src/graphics/texture/Texture2D.cpp
        src/graphics/texture/Texture2D.hpp
        src/graphics/texture/TextureBase.cpp
        src/graphics/texture/TextureBase.hpp
        src/graphics/texture/TextureCube.cpp
        src/graphics/texture/TextureCube.hpp
        )
source_group("src\\graphics\\texture" FILES ${TEXTURE_FILE})


add_executable(${PROJECT_NAME} ${SCR_FILE} ${FT_GL_FILE}
                                ${GLAD_FILE} ${STB_FILE}
                                ${GRAPHY_FILE} ${BUFFER_FILE}
                                ${RENDER_FILE}  ${TEXTURE_FILE}
                                ${MODEL_FILE})

target_link_libraries(${PROJECT_NAME} freetype glfw assimp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

message("command---- ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)


