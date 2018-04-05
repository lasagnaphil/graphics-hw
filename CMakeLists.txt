cmake_minimum_required(VERSION 3.5.1)
set(PROJECT_NAME gengine)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)

file(GLOB_RECURSE PROJECT_HEADERS src/*.h)
file(GLOB_RECURSE PROJECT_SOURCES glad/glad.c src/*.cpp)
file(GLOB_RECURSE PROJECT_SHADERS
        src/shaders/*.comp
        src/shaders/*.frag
        src/shaders/*.geom
        src/shaders/*.vert)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SHADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
        ${SDL2_INCLUDE_DIR}
        ${GLM_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

# GLAD
set(GLAD_DIR "deps/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# YAML
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
set(EVENT__DISABLE_TESTS ON)
add_subdirectory("deps/yaml-cpp")
target_link_libraries(${PROJECT_NAME} yaml-cpp)

# STB
set(STB_DIR "deps/stb")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_DIR})

# std::optional support for versions before C++17
set(OPTIONAL_LITE_DIR "deps/optional-lite")
target_include_directories(${PROJECT_NAME} PRIVATE ${OPTIONAL_LITE_DIR})

# set build directory
file(COPY ${CMAKE_SOURCE_DIR}/resources
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/shaders
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
