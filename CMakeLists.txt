
cmake_minimum_required(VERSION 3.7)
project(ogl-tuts)

set(EXTERNAL_PACKAGES ${CMAKE_CURRENT_SOURCE_DIR}/external)
add_subdirectory(${EXTERNAL_PACKAGES} external EXCLUDE_FROM_ALL)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(torusview)
