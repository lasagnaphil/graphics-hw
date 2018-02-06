//
// Created by lasagnaphil on 2/6/18.
//

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include "Image.h"

Texture::Texture(Image& image,
                 GLint level,
                 GLint internalFormat,
                 GLsizei width,
                 GLsizei height,
                 GLint border,
                 GLenum format,
                 GLenum type) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, type, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setParameterf(GLenum pname, GLfloat param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameteri(GLenum pname, GLint param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}
