cmake_minimum_required(VERSION 3.12.0)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(main WIN32 ${SOURCES})

target_compile_options(main PRIVATE /W3 /WX)
target_compile_definitions(main PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_features(main PRIVATE cxx_std_17)