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
    size_t pixelOffset = ((800 - j) * WINDOW_WIDTH + i) * 4;
    m_ColorBuffer[pixelOffset] = color.r;
    m_ColorBuffer[pixelOffset + 1] = color.g;
    m_ColorBuffer[pixelOffset + 2] = color.b;
    m_ColorBuffer[pixelOffset + 3] = color.a;
}

void DisplayBackend::run() {
    Mesh *mesh = new Mesh();
    if(!mesh->loadFile("teapot.obj")) {
        std::cout << "Error loading mesh file.\n";
    }

    while (m_Window.isOpen()) {
        sf::Event event;
        while (m_Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) m_Window.close();
        }

        drawMesh(mesh);
        // update();
        m_Buffer.update(m_ColorBuffer);

        m_Window.clear();
        m_Window.draw(m_DrawBuffer);
        m_Window.display();
    }
}

void DisplayBackend::update() {
    drawLine(Vector2i(0, 0), Vector2i(800, 800), sf::Color::Red);
    drawLine(Vector2i(0, 800), Vector2i(800, 0), sf::Color::White);
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

void DisplayBackend::drawMesh(Mesh* mesh) {
    std::vector<Vector3i> vertices = mesh->getVertexIndices();

    for (int i = 0; i < mesh->getNumFaces(); i++) {
        Vector3f t1, t2, t3;
        t1 = mesh->getVertex(vertices[i].x);
        t2 = mesh->getVertex(vertices[i].y);
        t3 = mesh->getVertex(vertices[i].z);

        Vector2i p1, p2, p3;
        p1.x = (t1.x + 5)*WINDOW_WIDTH/10;
        p1.y = (t1.y + 5)*WINDOW_HEIGHT/10;
        p2.x = (t2.x + 5)*WINDOW_WIDTH/10;
        p2.y = (t2.y + 5)*WINDOW_HEIGHT/10;
        p3.x = (t3.x + 5)*WINDOW_WIDTH/10;
        p3.y = (t3.y + 5)*WINDOW_HEIGHT/10;

        drawLine(p1, p2, sf::Color::White);
        drawLine(p2, p3, sf::Color::Blue);
        drawLine(p1, p3, sf::Color::Red);
    }
}