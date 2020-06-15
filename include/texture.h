#include <SFML/Graphics/Color.hpp>
#include <string>

struct Texture {
    unsigned char* image;
    int width;
    int height;
    int channels;

    Texture();
    ~Texture();

    sf::Color getColor(int i, int j);
    bool load_texture(std::string filename);
};
