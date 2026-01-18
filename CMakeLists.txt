cmake_minimum_required(VERSION 4.1)
project(codotaku_ds C)

set(CMAKE_C_STANDARD 11)

find_package(raylib REQUIRED)

add_executable(codotaku_ds main.c)
target_link_libraries(codotaku_ds PRIVATE raylib)