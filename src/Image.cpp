//
// Created by lasagnaphil on 2/6/18.
//

#include <stb_image.h>
#include <iostream>
#include "Image.h"

Image::Image(const char* filename) {
    data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load image " << filename << "!" << std::endl;
        exit(2);
    }
}

Image::~Image() {
    stbi_image_free(data);
}
