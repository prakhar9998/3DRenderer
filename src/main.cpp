#include <SFML/Graphics.hpp>
#include <math.h>
#include "display.h"

int main() {
    DisplayBackend display;
    display.createWindow();
    display.run();

    // display.drawLine(125, 125, 50, 50, sf::Color::Red);
    // line(130, 130, 756, 234, texture, pixels, sf::Color::White);
    // line(i % 800, i % 800, 400, 700, texture, pixels, sf::Color::Blue);

    return 0;
}