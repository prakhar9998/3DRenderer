#include <SFML/Graphics.hpp>

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
        
        for (int i = 0; i < 800; i++) {
            for (int j = 0; j < 800; j++) {
                if (i % 10 == 0 || j % 10 == 0) render.setPixel(i, j, sf::Color::Red);
            }
        }

        texture.loadFromImage(render);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}