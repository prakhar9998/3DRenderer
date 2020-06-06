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
    drawLine(Vector2i(125, 125), Vector2i(50, 50), sf::Color::Red);
    drawLine(Vector2i(50, 50), Vector2i(534, 236), sf::Color::White);
    m_Buffer.update(m_ColorBuffer);
}

void DisplayBackend::drawLine(Vector2i p1, Vector2i p2, sf::Color color) {
    bool steep = false;
    if (std::abs(p1.x-p2.x) < std::abs(p1.y-p2.y)) {        // line is steep
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
        steep = true;
    }
    
    if (p1.x > p2.x) {
        std::swap(p1.x, p2.x);
        std::swap(p1.y, p2.y);
    }

    int dx = p2.x-p1.x;
    int dy = p2.y-p1.y;
    
    int derror = std::abs(dy) * 2;
    int slopeError = 0;
    int y = p1.y;

    if (steep) {
        for (int x = p1.x; x <= p2.x; x++) {
            setPixel(y, x, color);
            slopeError += derror;
            if (slopeError > dx) {
                y += (p2.y > p1.y ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    } else {
        for (int x = p1.x; x <= p2.x; x++) {
            setPixel(x, y, color);
            slopeError += derror;
            if (slopeError > dx) {
                y += (p2.y > p1.y ? 1: -1);
                slopeError -= dx * 2;
            }
        }
    }
}