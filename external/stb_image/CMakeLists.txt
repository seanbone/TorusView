
cmake_minimum_required(VERSION 3.7)
project(stb_image)

add_library(${PROJECT_NAME} stb_image stb_image_write)
target_include_directories(${PROJECT_NAME} PUBLIC .)
