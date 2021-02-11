
cmake_minimum_required(VERSION 3.7)
project(imgui)

file(GLOB SRCFILES 
        *.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
)

file(GLOB HFILES
        *.h
        backends/imgui_impl_glfw.h
        backends/imgui_impl_opengl3.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GLAD")

add_library(${PROJECT_NAME} ${HFILES} ${SRCFILES})
target_include_directories(${PROJECT_NAME} PUBLIC . backends)
target_link_libraries(${PROJECT_NAME} glad glfw)