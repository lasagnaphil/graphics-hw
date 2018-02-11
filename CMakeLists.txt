cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME gengine)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)

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

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${ASSIMP_LIBRARY})

# GLAD
set(GLAD_DIR "deps/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# Tweeny
set(TWEENY_BUILD_EXAMPLES OFF)
set(TWEENY_BUILD_DOCUMENTATION OFF)
add_subdirectory("deps/tweeny")
target_link_libraries(${PROJECT_NAME} tweeny)

# STB
set(STB_DIR "deps/stb")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_DIR})

# set build directory
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/" DESTINATION "build/resources/")
INSTALL(DIRECTORY "${CMAKE_SOURCE_DIR}/shaders/" DESTINATION "build/shaders/")
