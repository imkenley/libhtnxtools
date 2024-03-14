cmake_minimum_required(VERSION 3.24)
project(libhtnxtools)

set(CMAKE_CXX_STANDARD 17)


include_directories(third_party/fmt/include)

add_subdirectory(third_party/fmt)

add_library(htnxtools libhtnxtools.cc)
add_executable(tree tree.cc)

target_link_libraries(htnxtools fmt)
target_link_libraries(tree htnxtools)
