#include <SFML/Graphics.hpp>
#include <math.h>

void line(int x1, int y1, int x2, int y2, sf::Image& image, sf::Color color) {
    // trying to implement naive first
    bool steep = false;
    if (std::abs(x1-x2) < std::abs(y1-y2)) {        // line is steep
        steep = true;
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    for (int x = x1; x <= x2; x++) {
        float t = (x - x1)/(float)(x2 - x1);
        int y = y1 * (1.-t) + y2*t;
        if (steep) image.setPixel(y, x, color);
        else image.setPixel(x, y, color);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "3D Software Renderer");

    sf::Image render;
    render.create(800, 800);

    sf::Texture texture;
    texture.loadFromImage(render);
    sf::Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        line(50, 50, 125, 125, render, sf::Color::Red);
        line(130, 130, 756, 234, render, sf::Color::White);

        texture.loadFromImage(render);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    render.saveToFile("render.tga");

    return 0;
}