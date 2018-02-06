cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME gengine)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)

file(GLOB PROJECT_HEADERS src/*.h)
file(GLOB PROJECT_SOURCES glad/glad.c src/*.cpp)
file(GLOB PROJECT_SHADERS
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
        ${GLM_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

# GLAD
set(GLAD_DIR "deps/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# Tweeny
set(TWEENY_BUILD_EXAMPLES FALSE)
set(TWEENY_BUILD_DOCUMENTATION FALSE)
add_subdirectory("deps/tweeny")
target_link_libraries(${PROJECT_NAME} tweeny)

# STB
set(STB_DIR "deps/stb")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_DIR})
