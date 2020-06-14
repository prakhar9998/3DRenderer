#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>
#include "vector3d.h"
#include "mesh.h"

class DisplayBackend {
private:
    sf::RenderWindow m_Window;
    sf::Texture m_Buffer;
    sf::Sprite m_DrawBuffer;
    sf::Uint8* m_PixelBuffer;

public:
    const static int WINDOW_WIDTH = 800;
    const static int WINDOW_HEIGHT = 800;

public:
    DisplayBackend();
    ~DisplayBackend();

    void update();
    void createWindow();
    void swapBuffers(sf::Uint8* pixelBuffer);
    
    sf::RenderWindow& getWindowInstance();
};

#endif      // DISPLAY_H
