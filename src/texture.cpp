#include "texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() { image = nullptr; }

Texture::~Texture() { delete [] image; }

Vector3i Texture::getColor(int i, int j) {
    if (!image) return Vector3i(0, 0, 0);
    i = i % width;
    j = j % height;
    unsigned char* pixelOffset = image + (i + j * width) * channels;
    return Vector3i(pixelOffset[0], pixelOffset[1], pixelOffset[2]);
}

bool Texture::load_texture(std::string filename) {
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!image) {
        std::cout << "Error loading texture file " << filename << std::endl;
        return false;
    }
    std::cout << "Successfully loaded texture file " << filename << std::endl;
    return true;
}
