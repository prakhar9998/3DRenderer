#include "engine.h"
#include "display.h"
#include "renderer.h"
#include "scene.h"

Engine::Engine() : m_IsWindowClosed(false), m_Rotation(0.f) {}
Engine::~Engine() {}

void Engine::processInput(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        m_IsWindowClosed = true;
        return;
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) m_Rotation -= 0.05f;
        else if (event.key.code == sf::Keyboard::Right) m_Rotation += 0.05f;
    }
}

void Engine::run() {
    DisplayBackend display;
    display.createWindow();
    sf::RenderWindow& window = display.getWindowInstance();

    // TODO: Move this into scene loader, which loads all the models and cams by itself.

    //  the files have to be changed here manually for now, later I will use cmdline arguments for this
    Model* teapot = new Model("./test_models/bs_smile.obj");
    teapot->loadDiffuse("./test_models/bs_smile_diffuse.png");
    teapot->transformModel(
        Vector3f(0., 0., 0.),
        Vector3f(0., 0., 0.),
        Vector3f(2.5, 2.5, 2.5)
    );

    Camera* camera = new Camera(Vector3f(0., 0., -3.f), Vector3f(0., 0., 0.));

    Scene scene;
    scene.loadModelInScene(teapot);
    scene.loadCameraInScene(camera);

    Renderer renderer;
    
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            processInput(event);
            if (m_IsWindowClosed) window.close();
        }
        
        sf::Time time = clock.getElapsedTime();
        std::cout << 1.f / time.asSeconds() << std::endl;
        clock.restart().asSeconds();
        renderer.renderScene(scene, m_Rotation);
        display.swapBuffers(renderer.getPixelBuffer());
        display.update();
    }

    delete camera;
    delete teapot;
}
