#include <SFML/Graphics.hpp>
#include "engine.h"
#include "display.h"
#include "rasterizer.h"

// TODO: the mesh will go into Scene class which will take care of all meshes being rendered.
#include "mesh.h"
Engine::Engine() : isWindowClosed(false) {}
Engine::~Engine() {}

void Engine::processInput() {

}

void Engine::render() {

}

void Engine::run() {
    DisplayBackend display;
    display.createWindow();
    Rasterizer rasterizer;
    sf::RenderWindow& window = display.getWindowInstance();

    // move this mesh stuff to scene manager class
    Mesh* mesh = new Mesh;
    if (!mesh->loadFile("cow.obj")) {
        std::cout << "Error loading mesh file.\n";
    }

    mesh->normalizeMesh();
    float rot = 0;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    rot -= 0.05f;
                } if (event.key.code == sf::Keyboard::Right) {
                    rot += 0.05f;
                }
            }
        }
        
        sf::Time time = clock.getElapsedTime();
        std::cout << 1.f / time.asSeconds() << std::endl;
        clock.restart().asSeconds();
        rasterizer.drawMesh(mesh, rot);
        display.swapBuffers(rasterizer.getBuffer());
        display.update();
    }
}