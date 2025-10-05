cmake_minimum_required(VERSION 3.28)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

set(lib_include_dir "include")
file(GLOB_RECURSE lib_src "src/**.*")
file(GLOB_RECURSE lib_include "${lib_include_dir}/**.*")

file(GLOB_RECURSE example_src "example/src/**.*")
file(GLOB_RECURSE example_include "example/include/**.*")

add_library(compiler_lib)
target_sources(compiler_lib PUBLIC ${lib_src} ${lib_include})
target_include_directories(compiler_lib PUBLIC ${lib_include_dir})

message("src: ${example_src}")

add_executable(compiler_example)
target_sources(compiler_example PUBLIC ${example_src} ${example_include})
target_link_libraries(compiler_example PUBLIC compiler_lib)
target_include_directories(compiler_example PUBLIC ${lib_include_dir} "example/include")

set_target_properties(compiler_lib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(compiler_example PROPERTIES LINKER_LANGUAGE CXX)
