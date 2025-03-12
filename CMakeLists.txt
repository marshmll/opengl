cmake_minimum_required(VERSION 3.21)
project(opengl CXX C)

add_executable(opengl src/main.cpp src/glad.c)

add_subdirectory(src)
add_subdirectory(externals/glfw/)

target_link_libraries(opengl PRIVATE glfw)
target_compile_features(opengl PRIVATE c_std_99 cxx_std_17)
target_include_directories(opengl PRIVATE include/ externals/glfw)
