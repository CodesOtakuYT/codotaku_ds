cmake_minimum_required(VERSION 3.10)
project(codotaku_ds C)

add_library(codotaku_ds INTERFACE)
target_include_directories(codotaku_ds INTERFACE include)
target_compile_features(codotaku_ds INTERFACE c_std_99)

add_library(codotaku_ds_library src/ds_impl.c)
target_link_libraries(codotaku_ds_library PUBLIC codotaku_ds)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_C_STANDARD 99)
    add_subdirectory(examples)
endif ()
