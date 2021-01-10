cmake_minimum_required(VERSION 3.19)
project(Othello)
set(CMAKE_MODULE_PATH "/usr/lib/cmake/glm/;${CMAKE_MODULE_PATH}")
set(CMAKE_CXX_STANDARD 14)

# Some magic
if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    option(BUILD_HTML "Build html also!" ON)

    set(EMSCRIPTEN TURE)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
endif()

message(STATUS "Cmake prefix path: ${CMAKE_PREFIX_PATH}")

# Target
file(GLOB Othello_src "*/*.h" "*/*.cpp" "*.h" "*.cpp")
list(FILTER Othello_src EXCLUDE REGEX "Othello_submit.cpp")

add_executable(Othello ${Othello_src} core/ab-branch.h)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_DEBUG_POSTFIX _d)

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# OpenGL ES
find_path(GLES3_INCLUDE_DIR GLES3/gl3.h)
find_library(GLES3_LIBRARY libGLESv2.so HINTS ${GLES3_INCLUDE_DIR}/../lib)

# GLM
find_package(glm REQUIRED)

if (EMSCRIPTEN)
    message(STATUS "Built in Emscripten to js.")
    target_link_libraries(${PROJECT_NAME} Threads::Threads glm)
    if (BUILD_HTML)
        set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    endif()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DEMSCRIPTEN=1)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
else()
    message(STATUS "Built in Clion to native.")

    # GLFW
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})

    target_link_libraries(${PROJECT_NAME} glfw Threads::Threads ${GLES3_LIBRARY} glm)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLES3_INCLUDE_DIR})
endif()