//
// Created by lasagnaphil on 2/6/18.
//

#ifndef GENGINE_IMAGE_H
#define GENGINE_IMAGE_H


class Image {
public:
    Image(const char* filename);
    ~Image();

    unsigned char* getData() { return data; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getNrChannels() const { return nrChannels; }

private:
    unsigned char* data;
    int width, height, nrChannels;
};


#endif //GENGINE_IMAGE_H
