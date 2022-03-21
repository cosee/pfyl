cmake_minimum_required(VERSION 3.13)
project(pfyl C)
set(CMAKE_C_STANDARD 90)
add_compile_options("-fpic")


set(PFYL_HOME ${PROJECT_SOURCE_DIR})

include(tests/CMakeLists.txt)
