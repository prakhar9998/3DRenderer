#include "display.h"

DisplayBackend::DisplayBackend()
: m_ColorBuffer(new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * 4]) {
    m_Buffer.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    m_Buffer.setRepeated(false);
    m_DrawBuffer.setTexture(m_Buffer);
}

DisplayBackend::~DisplayBackend() {
    delete [] m_ColorBuffer;
}

void DisplayBackend::createWindow() {
    m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "3D Software Renderer");
}

void DisplayBackend::setPixel(int i, int j, sf::Color color) {
    size_t pixelOffset = (j * WINDOW_WIDTH + i) * 4;
    m_ColorBuffer[pixelOffset] = color.r;
    m_ColorBuffer[pixelOffset + 1] = color.g;
    m_ColorBuffer[pixelOffset + 2] = color.b;
    m_ColorBuffer[pixelOffset + 3] = color.a;
}

void DisplayBackend::run() {
    while (m_Window.isOpen()) {
        sf::Event event;
        while (m_Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) m_Window.close();
        }

        update();

        m_Window.clear();
        m_Window.draw(m_DrawBuffer);
        m_Window.display();
    }
}

void DisplayBackend::update() {
    drawLine(125, 125, 50, 50, sf::Color::Red);
    drawLine(50, 50, 534, 236, sf::Color::White);
    m_Buffer.update(m_ColorBuffer);
}

void DisplayBackend::drawLine(int x1, int y1, int x2, int y2, sf::Color color) {
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
            setPixel(y, x, color);
            slopeError += derror;
            if (slopeError > dx) {
                y += (y2 > y1 ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    } else {
        for (int x = x1; x <= x2; x++) {
            setPixel(x, y, color);
            slopeError += derror;
            if (slopeError > dx) {
                y += (y2 > y1 ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    }
}