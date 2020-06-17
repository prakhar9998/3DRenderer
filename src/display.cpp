#include "display.h"
#include <math.h>
#include <algorithm>

DisplayBackend::DisplayBackend() {
    m_PixelBuffer = new sf::Uint8[WINDOW_WIDTH*WINDOW_HEIGHT*4];
    m_Buffer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_Buffer.setRepeated(false);
    m_DrawBuffer.setTexture(m_Buffer);
}

DisplayBackend::~DisplayBackend() {
    delete [] m_PixelBuffer;
}

void DisplayBackend::createWindow() {
    m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3D Software Renderer");
}

void DisplayBackend::update() {
    m_Window.clear();
    m_Window.draw(m_DrawBuffer);
    m_Window.display();
}

void DisplayBackend::swapBuffers(sf::Uint8* pixelBuffer) {
    std::copy_n(pixelBuffer, 4*WINDOW_HEIGHT*WINDOW_WIDTH, m_PixelBuffer);
    m_Buffer.update(m_PixelBuffer);
    std::fill(pixelBuffer, pixelBuffer + 4*WINDOW_WIDTH*WINDOW_HEIGHT, 0);
}

sf::RenderWindow& DisplayBackend::getWindowInstance() {
    return m_Window;
}