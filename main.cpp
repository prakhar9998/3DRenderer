#include <SFML/Graphics.hpp>
#include <math.h>

void setPixel(int i, int j, sf::Texture& texture, sf::Uint8* pixels, sf::Color color) {
    size_t pixelOffset = (j * 800 + i) * 4;
    pixels[pixelOffset] = color.r;
    pixels[pixelOffset + 1] = color.g;
    pixels[pixelOffset + 2] = color.b;
    pixels[pixelOffset + 3] = color.a;
}

void line(int x1, int y1, int x2, int y2, sf::Texture& texture, sf::Uint8* pixels, sf::Color color) {
    bool steep = false;
    if (std::abs(x1-x2) < std::abs(y1-y2)) {        // line is steep
        std::swap(x1, y1);
        std::swap(x2, y2);
        steep = true;
    }
    
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2-x1;
    int dy = y2-y1;
    
    int derror = std::abs(dy) * 2;
    int slopeError = 0;
    int y = y1;

    if (steep) {
        for (int x = x1; x <= x2; x++) {
            setPixel(y, x, texture, pixels, color);
            slopeError += derror;
            if (slopeError > dx) {
                y += (y2 > y1 ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    } else {
        for (int x = x1; x <= x2; x++) {
            setPixel(x, y, texture, pixels, color);
            slopeError += derror;
            if (slopeError > dx) {
                y += (y2 > y1 ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    }
    
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "3D Software Renderer");

    sf::Texture texture;
    sf::Uint8 *pixels = new sf::Uint8[4 * 800 * 800];
    texture.create(800, 800);
    sf::Sprite sprite(texture);

    int i = 45;

    while (window.isOpen()) {
        std::fill(pixels, pixels + 4*800*800, 0);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        if (i > 800) i = 0;
        i++;
        line(125, 125, 50, 50, texture, pixels, sf::Color::Red);
        line(130, 130, 756, 234, texture, pixels, sf::Color::White);
        line(i % 800, i % 800, 400, 700, texture, pixels, sf::Color::Blue);


        texture.update(pixels);

        window.clear();
        window.draw(sprite);
        window.display();
    }


    delete [] pixels;

    return 0;
}