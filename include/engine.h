#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>

class Engine {
private:
    bool m_IsWindowClosed;
    float m_Rotation;
private:
    void processInput(const sf::Event& event);

public:
    Engine();
    ~Engine();

    void run();
};

#endif      // ENGINE_H