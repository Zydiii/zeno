set(ZENO_WITH_ZPM OFF)
set(ZENO_WITH_SYCL OFF)
set(ZENO_WITH_LEGACY OFF)
set(ZENO_WITH_BACKWARD ON)
set(ZENO_BUILD_EDITOR ON)
set(ZENO_BUILD_DESIGNER ON)
set(ZENO_BUILD_TESTS OFF)
set(ZENO_BUILD_BENCHMARK OFF)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/OpenVDB)

add_custom_target(run EXCLUDE_FROM_ALL COMMAND $<TARGET_FILE:zeno_editor>)
add_dependencies(run zeno_editor)
#add_compile_options(-fdiagnostics-color=always)