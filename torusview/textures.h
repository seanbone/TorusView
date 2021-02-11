#ifndef TEXTURES_H
#define TEXTURES_H

#include <string>

/**
 * Import a texture from file and return its ID.
 */
unsigned int makeTexture(std::string file, bool hasAlpha = false);

#endif
