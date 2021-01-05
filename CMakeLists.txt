cmake_minimum_required(VERSION 3.19)
project(Othello)

set(CMAKE_CXX_STANDARD 14)

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# SDL2
find_package(SDL2 REQUIRED)

# OpenGL ES
find_path(GLES3_INCLUDE_DIR GLES3/gl3.h)
find_library(GLES3_LIBRARY libGLESv2.so HINTS ${GLES3_INCLUDE_DIR}/../lib)

# GLM
find_package(glm REQUIRED)

file(GLOB Othello_src "*.h" "*.cpp")
list(FILTER Othello_src EXCLUDE REGEX "Othello_submit.cpp")

add_executable(Othello ${Othello_src} shader.h shader.cpp)
target_link_libraries(Othello SDL2::SDL2 Threads::Threads ${GLES3_LIBRARY} glm)
target_include_directories(Othello PUBLIC ${GLES3_INCLUDE_DIR})