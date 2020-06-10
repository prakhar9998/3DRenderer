#ifndef ENGINE_H
#define ENGINE_H

class Engine {
private:
    bool isWindowClosed;

private:
    void processInput();
    void render();

public:
    Engine();
    ~Engine();

    void run();
};

#endif      // ENGINE_H