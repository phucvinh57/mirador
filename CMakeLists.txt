cmake_minimum_required(VERSION 3.16)
project(demo-mir-compositor)
set(CMAKE_CXX_STANDARD 23)

include(FindPkgConfig)
pkg_check_modules(MIRAL miral REQUIRED IMPORTED_TARGET)
add_executable(miracle miracle.cpp)
target_compile_definitions(miracle PRIVATE MIR_LOG_COMPONENT="miracle")
add_custom_command(
    TARGET miracle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/miracle.config"
            "$<TARGET_FILE_DIR:miracle>/miracle.config")
target_link_libraries(miracle PkgConfig::MIRAL)
