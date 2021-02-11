
cmake_minimum_required(VERSION 3.12)
project(torusview)

file(GLOB SRCFILES *.cpp)
file(GLOB HFILES *.h)

# Tell the compiled binary where its absolute path is
add_compile_definitions(SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SRCFILES} ${HFILES})
target_link_libraries(${PROJECT_NAME} glfw glad stb_image glm)

# Copy shader files to build directory
# See https://stackoverflow.com/questions/13429656/how-to-copy-contents-of-a-directory-into-build-directory-after-make-with-cmake
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/
#    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/)