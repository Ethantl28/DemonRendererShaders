cmake_minimum_required(VERSION 3.10...3.29)

set(APPLICATION_NAME "Application")

project(${APPLICATION_NAME} VERSION 1.0.0)

# Application File
# YOU WILL NEED TO ADD YOUR LAYER FILES HERE
set (PROJECT_HEADER_FILES
	"application/include/app.hpp"
        "application/include/startPoint.hpp"
        "application/include/Lab1.hpp"
        "application/include/Lab2.hpp"
        "application/include/grid.hpp"
	"application/include/lab3.hpp"
	"application/include/lab4.hpp"
	"application/include/lab5.hpp"
)

set (PROJECT_SOURCE_FILES
	"application/src/app.cpp"
        "application/src/startPoint.cpp"
        "application/src/Lab1.cpp"
        "application/src/Lab2.cpp"
        "application/src/grid.cpp"
	"application/src/lab3.cpp"
	"application/src/lab4.cpp"
	"application/src/lab5.cpp"
)

# YOU WILL NEED TO ADD YOUR SCRIPT FILES HERE
set (SCRIPT_HEADER_FILES
	"application/scripts/include/camera.hpp"
	"application/scripts/include/rotation.hpp"
)
set (SCRIPT_SOURCE_FILES
	"application/scripts/src/camera.cpp"
	"application/scripts/src/rotation.cpp"
)

# Add excutable target and include directory
add_executable(${APPLICATION_NAME} ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES} ${SCRIPT_SOURCE_FILES} ${SCRIPT_HEADER_FILES})
target_include_directories(${APPLICATION_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/application/include")
target_include_directories(${APPLICATION_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/DemonRenderer/include")
target_include_directories(${APPLICATION_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/application/") # For scripts, might change this

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APPLICATION_NAME})
set_target_properties(${APPLICATION_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Visual Studio filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}../application/src" PREFIX "src" FILES ${PROJECT_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}../application/include" PREFIX "include" FILES ${PROJECT_HEADER_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}../application/scripts" PREFIX "gameScripts" FILES ${SCRIPT_HEADER_FILES} ${SCRIPT_SOURCE_FILES})

set(RENDERER_NAME "DemonRenderer")

# Library Files
set (RENDERER_HEADER_FILES
	"DemonRenderer/include/DemonRenderer.hpp"
	"DemonRenderer/include/core/entryPoint.hpp"
	"DemonRenderer/include/core/application.hpp"
	"DemonRenderer/include/core/log.hpp"
	"DemonRenderer/include/core/timer.hpp"
	"DemonRenderer/include/core/layer.hpp"
	"DemonRenderer/include/core/randomiser.hpp"
    "DemonRenderer/include/events/event.hpp"
    "DemonRenderer/include/events/eventHandler.hpp"
    "DemonRenderer/include/events/events.hpp"
    "DemonRenderer/include/events/keyEvents.hpp"
    "DemonRenderer/include/events/mouseEvents.hpp"
    "DemonRenderer/include/events/windowEvents.hpp"
	"DemonRenderer/include/windows/windowsSystem.hpp"
	"DemonRenderer/include/windows/GLFWSystem.hpp"
	"DemonRenderer/include/windows/window.hpp"
	"DemonRenderer/include/windows/GLFWWindowImpl.hpp"
	"DemonRenderer/include/windows/graphicsContext.hpp"
	"DemonRenderer/include/windows/GLFW_GL_GC.hpp"
	"DemonRenderer/include/buffers/VBO.hpp"
	"DemonRenderer/include/buffers/IBO.hpp"
	"DemonRenderer/include/buffers/VAO.hpp"
	"DemonRenderer/include/buffers/VBOLayout.hpp"
	"DemonRenderer/include/buffers/UBO.hpp"
	"DemonRenderer/include/buffers/UBOLayout.hpp"
	"DemonRenderer/include/buffers/UBOmanager.hpp"
	"DemonRenderer/include/buffers/FBO.hpp"
	"DemonRenderer/include/buffers/FBOlayout.hpp"
	"DemonRenderer/include/buffers/RBO.hpp"
	"DemonRenderer/include/buffers/SSBO.hpp"
	"DemonRenderer/include/assets/shader.hpp"
	"DemonRenderer/include/assets/texture.hpp"
	"DemonRenderer/include/assets/cubeMap.hpp"
	"DemonRenderer/include/assets/managedTexture.hpp"
	"DemonRenderer/include/assets/textureUnitManager.hpp"
	"DemonRenderer/include/assets/mesh.hpp"
	"DemonRenderer/include/rendering/material.hpp"
	"DemonRenderer/include/rendering/camera.hpp"
	"DemonRenderer/include/rendering/lights.hpp"
	"DemonRenderer/include/rendering/renderPass.hpp"
	"DemonRenderer/include/rendering/depthOnlyPass.hpp"
	"DemonRenderer/include/rendering/computePass.hpp"
	"DemonRenderer/include/rendering/scene.hpp"
	"DemonRenderer/include/rendering/renderer.hpp"
	"DemonRenderer/include/rendering/uniformDataTypes.hpp"
	"DemonRenderer/include/gameObjects/actor.hpp"
	"DemonRenderer/include/gameObjects/renderable.hpp"
	"DemonRenderer/include/gameObjects/scriptable.hpp"
	"DemonRenderer/include/gameObjects/transformable.hpp"
)
set (RENDERER_SOURCE_FILES
	"DemonRenderer/src/core/application.cpp"
	"DemonRenderer/src/core/log.cpp"
	"DemonRenderer/src/core/timer.cpp"
	"DemonRenderer/src/core/randomiser.cpp"
	"DemonRenderer/src/windows/GLFWWindowImpl.cpp"
	"DemonRenderer/src/windows/GLFW_GL_GC.cpp"
	"DemonRenderer/src/buffers/VBO.cpp"
	"DemonRenderer/src/buffers/IBO.cpp"
	"DemonRenderer/src/buffers/VAO.cpp"
	"DemonRenderer/src/buffers/VBOLayout.cpp"
	"DemonRenderer/src/buffers/UBO.cpp"
	"DemonRenderer/src/buffers/UBOLayout.cpp"
	"DemonRenderer/src/buffers/UBOmanager.cpp"
	"DemonRenderer/src/buffers/FBO.cpp"
	"DemonRenderer/src/buffers/RBO.cpp"
    "DemonRenderer/src/buffers/SSBO.cpp"
	"DemonRenderer/src/assets/shader.cpp"
	"DemonRenderer/src/assets/texture.cpp"
	"DemonRenderer/src/assets/cubeMap.cpp"
	"DemonRenderer/src/assets/managedTexture.cpp"
	"DemonRenderer/src/assets/textureUnitManager.cpp"
	"DemonRenderer/src/assets/mesh.cpp"
	"DemonRenderer/src/rendering/material.cpp"
	"DemonRenderer/src/rendering/renderer.cpp"
	"DemonRenderer/src/rendering/renderPass.cpp"
	"DemonRenderer/src/rendering/depthOnlyPass.cpp"
)

# Add library target (renderer) and include directory
add_library(${RENDERER_NAME} ${RENDERER_SOURCE_FILES} ${RENDERER_HEADER_FILES})
target_include_directories(${RENDERER_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/DemonRenderer/include")
# Visual Studio filters
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}../DemonRenderer/src" PREFIX "src" FILES ${RENDERER_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}../DemonRenderer/include" PREFIX "include" FILES ${RENDERER_HEADER_FILES})

# Add external libraries
include(FetchContent)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG         v1.x
)


set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY ON CACHE BOOL "" FORCE)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 		1.0.1
)

FetchContent_Declare(
	glfw 
	GIT_REPOSITORY	https://github.com/glfw/glfw.git 
	GIT_TAG			3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/SimonCouplandDMU/DMUGlad.git
  GIT_TAG        gl4.6_core_no_ext
 )

set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY	https://github.com/assimp/assimp.git
	GIT_TAG			v5.4.0
)

FetchContent_Declare(GSL
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG v4.0.0
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(spdlog glm glfw glad assimp GSL)

# IMGUI needs a bit more fiddling with...  

set(IMGUI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-")
set(IMGUI_SOURCE_DIR "${IMGUI_ROOT}src")
FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.4-docking.zip
  SOURCE_DIR "${IMGUI_ROOT}src"
  SUBBUILD_DIR "${IMGUI_ROOT}subbuild"
  BINARY_DIR "${IMGUI_ROOT}build"
)

add_library(imgui_glfw STATIC
  ${IMGUI_SOURCE_DIR}/imgui.cpp
  ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
  ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
  ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
  ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
  ${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set_target_properties(imgui_glfw PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${IMGUI_ROOT}build"
)

target_link_libraries(imgui_glfw PRIVATE glfw OpenGL::GL)

target_include_directories(imgui_glfw
PUBLIC
  ${IMGUI_SOURCE_DIR}
  ${IMGUI_SOURCE_DIR}/backends
)


# Add and create STB image files specifically
set(STB_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/stb_image")
file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h  "${STB_IMAGE_DIR}/stbImage/stb_image.h" STATUS _stat TLS_VERIFY ON LOG _log)
#message("status  ${_stat} log ${_log}")

file(WRITE "${STB_IMAGE_DIR}/stbImage/stb_image.cpp" "#define STB_IMAGE_IMPLEMENTATION\n#include \"stb_image.h\"")

add_library(stb_image_impl STATIC "${STB_IMAGE_DIR}/stbImage/stb_image.cpp")

target_include_directories(stb_image_impl PUBLIC "${STB_IMAGE_DIR}")

# Use C++20 everywhere
set_target_properties(${APPLICATION_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${RENDERER_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(spdlog PROPERTIES CXX_STANDARD 20)
set_target_properties(glm PROPERTIES CXX_STANDARD 20)
set_target_properties(glfw PROPERTIES CXX_STANDARD 20)
set_target_properties(glad PROPERTIES CXX_STANDARD 20)
set_target_properties(assimp PROPERTIES CXX_STANDARD 20)
set_target_properties(imgui_glfw PROPERTIES CXX_STANDARD 20)
set_target_properties(stb_image_impl PROPERTIES CXX_STANDARD 20)


target_link_libraries(${RENDERER_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${RENDERER_NAME} PRIVATE spdlog)
target_link_libraries(${RENDERER_NAME} PRIVATE glm)
target_link_libraries(${RENDERER_NAME} PRIVATE glfw)
target_link_libraries(${RENDERER_NAME} PRIVATE glad)
target_link_libraries(${RENDERER_NAME} PRIVATE assimp)
target_link_libraries(${RENDERER_NAME} PRIVATE Microsoft.GSL::GSL)
target_link_libraries(${RENDERER_NAME} PRIVATE imgui_glfw)
target_link_libraries(${RENDERER_NAME} PRIVATE stb_image_impl)

target_link_libraries(${APPLICATION_NAME} PRIVATE ${RENDERER_NAME})
target_link_libraries(${APPLICATION_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${APPLICATION_NAME} PRIVATE spdlog)
target_link_libraries(${APPLICATION_NAME} PRIVATE glm)
target_link_libraries(${APPLICATION_NAME} PRIVATE glfw)
target_link_libraries(${APPLICATION_NAME} PRIVATE glad)
target_link_libraries(${APPLICATION_NAME} PRIVATE assimp)
target_link_libraries(${APPLICATION_NAME} PRIVATE Microsoft.GSL::GSL)
target_link_libraries(${APPLICATION_NAME} PRIVATE imgui_glfw)
target_link_libraries(${APPLICATION_NAME} PRIVATE stb_image_impl)

set_target_properties(glad PROPERTIES FOLDER "Glad")

set_target_properties(assimp PROPERTIES FOLDER "Assimp")
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "Assimp")
set_target_properties(zlibstatic PROPERTIES FOLDER "Assimp")

set_target_properties(imgui_glfw PROPERTIES FOLDER "GLFW3")

set_target_properties(glm PROPERTIES FOLDER "GLM")

set_target_properties(spdlog PROPERTIES FOLDER "SPDLOG")

set_target_properties(stb_image_impl PROPERTIES FOLDER "stb_image")

