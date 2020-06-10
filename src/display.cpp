#include "display.h"
#include <math.h>
#include <algorithm>

DisplayBackend::DisplayBackend()
: m_PixelBuffer(new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * 4]) {
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

void DisplayBackend::run() {
    Mesh *mesh = new Mesh();
    if(!mesh->loadFile("teapot.obj")) {
        std::cout << "Error loading mesh file.\n";
    }   
    mesh->normalizeMesh();

    sf::Clock clock;
    sf::Clock rot;
    float lastTime = 0;

    while (m_Window.isOpen()) {
        sf::Event event;
        while (m_Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) m_Window.close();
        }

        sf::Time time = clock.getElapsedTime();
        std::cout << 1.f / time.asSeconds() << std::endl;

        clock.restart().asSeconds();

        update();
        m_Buffer.update(m_PixelBuffer);
    }
}

void DisplayBackend::update() {
    m_Window.clear();
    m_Window.draw(m_DrawBuffer);
    m_Window.display();
}

void DisplayBackend::swapBuffers(sf::Uint8* pixelBuffer) {
    m_PixelBuffer = pixelBuffer;
    m_Buffer.update(m_PixelBuffer);
    std::fill(pixelBuffer, pixelBuffer + 4*WINDOW_WIDTH*WINDOW_HEIGHT, 0);
}

sf::RenderWindow& DisplayBackend::getWindowInstance() {
    return m_Window;
}