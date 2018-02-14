//
// Created by lasagnaphil on 2/6/18.
//

#ifndef GENGINE_TEXTURE_H
#define GENGINE_TEXTURE_H

#include "Image.h"

#include <glad/glad.h>

#define VAR_WITH_SETTER(__Type, __memberName, __FaceName) \
    private: __Type __memberName; \
    public: Builder& set##__FaceName(__Type value) { \
        (__memberName) = value; \
        return *this; \
    }

enum class TextureType {
    Default, Diffuse, Specular
};

class Texture {
    class Builder {
        friend class Texture;
    public:
        Builder() : data(nullptr),
                    level(0),
                    internalFormat(GL_RGB),
                    width(0),
                    height(0),
                    border(0),
                    format(GL_RGB),
                    datatype(GL_UNSIGNED_BYTE) {}

        Builder& setImage(Image& image) {
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

        VAR_WITH_SETTER(unsigned char*, data, Data)
        VAR_WITH_SETTER(GLint, level, Level)
        VAR_WITH_SETTER(GLint, internalFormat, InternalFormat)
        VAR_WITH_SETTER(GLsizei, width, Width)
        VAR_WITH_SETTER(GLsizei, height, Height)
        VAR_WITH_SETTER(GLint, border, Border)
        VAR_WITH_SETTER(GLenum, format, Format)
        VAR_WITH_SETTER(GLenum, datatype, Datatype)

        Texture create() {
            return Texture(data, level, internalFormat,
                           width, height, border,
                           format, datatype);
        }

    };

public:
    Texture(unsigned char* data,
            GLint level,
            GLint internalFormat,
            GLsizei width,
            GLsizei height,
            GLint border,
            GLenum format,
            GLenum datatype);

    Texture(Builder& builder);

    Texture();

    void setParameterf(GLenum pname, GLfloat param);
    void setParameteri(GLenum pname, GLint param);
    void setParameterfv(GLenum pname, const GLfloat* param);
    void setParameteriv(GLenum pname, const GLint* param);
    void setParameterIiv(GLenum pname, const GLint* param);
    void setParameterIuiv(GLenum pname, const GLuint* param);

    unsigned int getID() const { return textureID; }

    static Builder build() { return Builder(); }

    static Texture fromImage(Image& image) { return Texture::build().setImage(image).create(); }

    TextureType type = TextureType::Default;
    std::string path;

private:
    unsigned int textureID;
};

#undef VAR_WITH_SETTER

#endif //GENGINE_TEXTURE_H
