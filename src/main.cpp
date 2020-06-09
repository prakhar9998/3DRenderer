#include "display.h"

int main() {
    DisplayBackend display;
    display.createWindow();
    display.run();

    return 0;
}