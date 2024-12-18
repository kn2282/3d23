#pragma once

#include "glad/glad_4_6/glad/include/glad/gl.h"
#include <iostream>

//#include "glm/gtx/string_cast.hpp"

#include "3rdParty/stb/stb_image.h"

using namespace std;

GLuint create_texture(const char *filename) {
    GLint width, height, channels;

    stbi_set_flip_vertically_on_load(true);
    auto img = stbi_load(filename, &width, &height, &channels, 0);
    if (!img) {
        cerr << "Could not read image from file " << filename << endl;
        return 0;
    }
    GLenum format;
    if (channels == 3)
        format = GL_RGB;
    else if (channels == 4) {
        format = GL_RGBA;
    }
    GLuint result_texture;
    glGenTextures(1, &result_texture);
    glBindTexture(GL_TEXTURE_2D, result_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0u);
    return result_texture;
}