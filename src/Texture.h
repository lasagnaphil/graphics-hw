//
// Created by lasagnaphil on 2/6/18.
//

#ifndef GENGINE_TEXTURE_H
#define GENGINE_TEXTURE_H

#include "Image.h"

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <cmath>

#define VAR_WITH_SETTER(__Type, __memberName, __FaceName) \
    private: __Type __memberName; \
    public: Builder& set##__FaceName(__Type value) { \
        (__memberName) = value; \
        return *this; \
    }

enum class TextureType {
    Diffuse, Specular
};

class Texture {
    class Builder {
        friend class Texture;
    public:
        Builder();

        Builder& setImage(Image& image);

        VAR_WITH_SETTER(unsigned char*, data, Data)
        VAR_WITH_SETTER(GLint, level, Level)
        VAR_WITH_SETTER(GLint, internalFormat, InternalFormat)
        VAR_WITH_SETTER(GLsizei, width, Width)
        VAR_WITH_SETTER(GLsizei, height, Height)
        VAR_WITH_SETTER(GLint, border, Border)
        VAR_WITH_SETTER(GLenum, format, Format)
        VAR_WITH_SETTER(GLenum, datatype, Datatype)
        VAR_WITH_SETTER(TextureType, textureType, TextureType)

        Texture create() {
            return Texture(data, level, internalFormat,
                           width, height, border,
                           format, datatype, textureType);
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
            GLenum datatype,
            TextureType textureType);

    Texture(Builder& builder);

    Texture();

    ~Texture() = default;
    Texture(const Texture& other) = default;
    Texture& operator=(const Texture& other) = default;
    Texture(Texture&& other) = default;
    Texture& operator=(Texture&& other) = default;

    void setParameterf(GLenum pname, GLfloat param);
    void setParameteri(GLenum pname, GLint param);
    void setParameterfv(GLenum pname, const GLfloat* param);
    void setParameteriv(GLenum pname, const GLint* param);
    void setParameterIiv(GLenum pname, const GLint* param);
    void setParameterIuiv(GLenum pname, const GLuint* param);

    unsigned int getID() const { return textureID; }

    static Builder build() { return Builder(); }

    static Texture fromImage(Image& image, TextureType type = TextureType::Diffuse);

    static Texture fromColor(const glm::vec4& color, TextureType type = TextureType::Diffuse);

    TextureType type = TextureType::Diffuse;
    std::string path;

private:
    unsigned int textureID;
};


#undef VAR_WITH_SETTER

#endif //GENGINE_TEXTURE_H
