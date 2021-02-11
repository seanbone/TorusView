#ifndef SAVEIMAGE_H
#define SAVEIMAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>

// https://stackoverflow.com/a/26221725/817296
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

// https://stackoverflow.com/a/12774387/817296
inline bool file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

void save_image(const char* filepath, GLFWwindow* w) {
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}

/**
 * Automatically find an available file name and save a screenshot there.
 * file_pattern is used in std::format with a single integer argument.
 */
void save_screenshot(std::string file_pattern, GLFWwindow* w) {
    unsigned int i = 0;
    std::string fname = "";
    
    do {
        fname = string_format(file_pattern, ++i);
    } while (file_exists(fname));

    std::cout << fname << std::endl;
    save_image(fname.c_str(), w);
}

#endif
