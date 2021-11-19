set(ZENO_TARGET Editor)
set(ZENO_USE_SYSTEM_TBB ON)
set(ZENO_WITH_SYCL OFF)
set(HIPSYCL_TARGETS "omp")
set(ZENO_WITH_LEGACY OFF)
set(ZENO_WITH_BACKWARD ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/hipSYCL)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-fdiagnostics-color=always)

add_custom_target(run
    COMMAND $<TARGET_FILE:zeno>
    )
add_dependencies(run zeno)
