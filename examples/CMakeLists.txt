find_package(raylib)

if (raylib_FOUND)
    add_executable(codotaku_ds_visualizer visualizer.c)
    target_link_libraries(codotaku_ds_visualizer PRIVATE raylib codotaku_ds)
endif ()