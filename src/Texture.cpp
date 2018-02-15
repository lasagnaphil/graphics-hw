//
// Created by lasagnaphil on 2/6/18.
//

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include "Image.h"

Texture::Texture(unsigned char* data,
                 GLint level,
                 GLint internalFormat,
                 GLsizei width,
                 GLsizei height,
                 GLint border,
                 GLenum format,
                 GLenum datatype) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, datatype, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(Builder& builder) :
        Texture(builder.data, builder.level, builder.internalFormat,
        builder.width, builder.height, builder.border, builder.format, builder.datatype) {}

Texture::Texture() :
        Texture(nullptr, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE) {}

void Texture::setParameterf(GLenum pname, GLfloat param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameteri(GLenum pname, GLint param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameterfv(GLenum pname, const GLfloat* param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterfv(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameteriv(GLenum pname, const GLint* param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteriv(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameterIiv(GLenum pname, const GLint* param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterIiv(GL_TEXTURE_2D, pname, param);
}

void Texture::setParameterIuiv(GLenum pname, const GLuint* param) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterIuiv(GL_TEXTURE_2D, pname, param);
}

#undef SETTER_IMPL


