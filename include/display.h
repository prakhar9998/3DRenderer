#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>
#include "vector3d.h"
#include "mesh.h"

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

    // TODO: move these methods to renderer class.
    // For now drawing wireframe to test wavefront model parsing.
    void drawLine(Vector2i p1, Vector2i p2, sf::Color color);
    void drawMesh(Mesh* mesh, float time);
};

#endif