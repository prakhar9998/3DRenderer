#include <string>
#include "vector3d.h"

struct Texture {
    unsigned char* image;
    int width;
    int height;
    int channels;

    Texture();
    ~Texture();

    Vector3i getColor(int i, int j);
    bool load_texture(std::string filename);
};
