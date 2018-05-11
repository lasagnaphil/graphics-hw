//
// Created by lasagnaphil on 2/6/18.
//

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include "Texture.h"
#include "Image.h"

Texture::Builder::Builder() : data(nullptr),
                              level(0),
                              internalFormat(GL_RGBA),
                              width(0),
                              height(0),
                              border(0),
                              format(GL_RGBA),
                              datatype(GL_UNSIGNED_BYTE) {}

Texture::Builder& Texture::Builder::setImage(Image& image) {
    data = image.getData();
    width = image.getWidth();
    height = image.getHeight();
    int nrComponents = image.getNrChannels();
    if (nrComponents == 1) {
        format = GL_RED;
    }
    else if (nrComponents == 3) {
        format = GL_RGB;
    }
    else if (nrComponents == 4) {
        format = GL_RGBA;
    }
    return *this;
}

Texture::Texture(unsigned char* data,
                 GLint level,
                 GLint internalFormat,
                 GLsizei width,
                 GLsizei height,
                 GLint border,
                 GLenum format,
                 GLenum datatype,
                 TextureType textureType) : type(textureType) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, datatype, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Texture::Texture(Builder& builder) :
        Texture(builder.data, builder.level, builder.internalFormat,
        builder.width, builder.height, builder.border, builder.format, builder.datatype,
        builder.textureType) {}

Texture::Texture() :
        Texture(nullptr, 0, GL_RGBA, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureType::Diffuse) {}

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

Texture Texture::fromImage(Image& image, TextureType type) {
    return Texture::build().setImage(image).setTextureType(type).create();
}

Texture Texture::fromColor(const glm::vec4& color, TextureType type) {
    unsigned char imageData[2 * 2 * 4];
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            imageData[i*8 + j*4] = static_cast<unsigned char>(std::round(255 * color.r));
            imageData[i*8 + j*4 + 1]= static_cast<unsigned char>(std::round(255 * color.g));
            imageData[i*8 + j*4 + 2]= static_cast<unsigned char>(std::round(255 * color.b));
            imageData[i*8 + j*4 + 3]= static_cast<unsigned char>(std::round(255 * color.a));
        }
    }
    return Texture::build()
            .setInternalFormat(GL_RGBA)
            .setWidth(2)
            .setHeight(2)
            .setFormat(GL_RGBA)
            .setData(imageData)
            .setTextureType(type)
            .create();
}



