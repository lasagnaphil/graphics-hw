cmake_minimum_required(VERSION 3.5.1)
set(PROJECT_NAME gengine)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE PROJECT_HEADERS src/*.h)
file(GLOB_RECURSE PROJECT_SOURCES glad/glad.c src/*.cpp)
file(GLOB_RECURSE PROJECT_SHADERS
        src/shaders/*.comp
        src/shaders/*.frag
        src/shaders/*.geom
        src/shaders/*.vert)
file(GLOB_RECURSE PROJECT_RESOURCES
        src/resources/* src/imgui.ini)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SHADERS}
        ${PROJECT_RESOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${SDL2_INCLUDE_DIR}
        ${GLM_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIR})

set(EVENT__DISABLE_TESTS ON)

# GLAD
set(GLAD_DIR "deps/glad")
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} glad ${CMAKE_DL_LIBS})

# SDL
set(SDL_DIR "deps/sdl2")
add_subdirectory(${SDL_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${SDL_DIR}/include")
target_link_libraries(${PROJECT_NAME} SDL2-static)

# GLM
set(GLM_DIR "deps/glm")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")

# IMGUI
set(IMGUI_DIR "deps/imgui")
add_library(imgui
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_impl_sdl_gl3.cpp")

target_include_directories(imgui PRIVATE "${SDL_DIR}/include")
target_include_directories(imgui PUBLIC "${IMGUI_DIR}")
target_link_libraries(${PROJECT_NAME} imgui)

# YAML
set(YAML_DIR "deps/yaml-cpp")
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
add_subdirectory(${YAML_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${YAML_DIR}/include")
target_link_libraries(${PROJECT_NAME} yaml-cpp)

# STB
set(STB_DIR "deps/stb")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_DIR})

# std::optional support for versions before C++17
set(OPTIONAL_LITE_DIR "deps/optional-lite")
target_include_directories(${PROJECT_NAME} PRIVATE ${OPTIONAL_LITE_DIR})

