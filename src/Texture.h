//
// Created by lasagnaphil on 2/6/18.
//

#ifndef GENGINE_TEXTURE_H
#define GENGINE_TEXTURE_H


#include "Image.h"

class Texture {
public:
    Texture(Image& image,
            GLint level,
            GLint internalFormat,
            GLsizei width,
            GLsizei height,
            GLint border,
            GLenum format,
            GLenum type);

    void setParameterf(GLenum pname, GLfloat param);
    void setParameteri(GLenum pname, GLint param);

    unsigned int getID() const { return textureID; }

private:
    unsigned int textureID;
};



#endif //GENGINE_TEXTURE_H
