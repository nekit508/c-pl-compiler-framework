cmake_minimum_required(VERSION 3.28)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE core_src "src/**.*")
file(GLOB_RECURSE core_include "include/**.*")

message("src: ${core_src}")

add_executable(compiler)
target_sources(compiler PUBLIC ${core_src} ${core_include})
target_include_directories(compiler PUBLIC "include")
