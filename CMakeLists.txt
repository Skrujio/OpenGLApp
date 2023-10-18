cmake_minimum_required(VERSION 3.27.7)

project(app C CXX)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(thirdparty/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(thirdparty/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(
                    # include/
                    thirdparty/assimp/include/
                    thirdparty/glad/include/
                    thirdparty/glfw/include/
                    thirdparty/glm/include/
                    thirdparty/stb/include/)

file(GLOB THIRDPARTY_SOURCES thirdparty/glad/src/glad.c)
file(GLOB PROJECT_SOURCES src/*.cpp)
# file(GLOB PROJECT_HEADERS include/*.h)
file(GLOB PROJECT_SHADERS shaders/*.vert
                          shaders/*.geom
                          shaders/*.frag
                          shaders/*.vs
                          shaders/*.gs
                          shaders/*.fs)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    # ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
    ${THIRDPARTY_SOURCES})

target_link_libraries(${PROJECT_NAME}
    assimp
    glfw
    ${GLFW_LIBRARIES}
    ${GLAD_LIBRARIES}
)

add_definitions(-DGLFW_INCLUDE_NONE)