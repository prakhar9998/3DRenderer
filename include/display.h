#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>

class DisplayBackend {
private:
    const static int WINDOW_WIDTH = 800;
    const static int WINDOW_HEIGHT = 800;

    sf::RenderWindow m_Window;
    sf::Texture m_Buffer;
    sf::Sprite m_DrawBuffer;
    sf::Uint8* m_ColorBuffer;

private:
    void update();

public:
    DisplayBackend();
    ~DisplayBackend();

    void createWindow();
    void setPixel(int x, int y, sf::Color color);

    // TODO: refactor out part of this method to the engine class later
    void run();

    // TODO: move this method into rasterizer class
    void drawLine(int x1, int y1, int x2, int y2, sf::Color color);
};

#endif