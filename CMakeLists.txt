cmake_minimum_required(VERSION 3.27.7)

project(app C CXX)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/assimp)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/thirdparty/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad_gl_core_46 SHARED REPRODUCIBLE API gl:core=4.6)

add_subdirectory(thirdparty/glm)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

include_directories(
    include/
    resource/
    thirdparty/stb/)
    
    file(GLOB PROJECT_SOURCES src/main.cpp)
    file(GLOB PROJECT_HEADERS include/*.h)
    file(GLOB PROJECT_SHADERS shaders/*.vert
    shaders/*.geom
    shaders/*.frag
    shaders/*.vs
    shaders/*.gs
    shaders/*.fs)
    
    add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
    )
    
    target_link_libraries(${PROJECT_NAME}
    assimp
    glfw
    glm::glm
    glad_gl_core_46
    )
    
add_definitions(-DGLFW_INCLUDE_NONE)