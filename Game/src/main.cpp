#include <stdio.h>

#include <engine.hpp>

int main(int argc, char** argv)
{
    ge::Engine gameEngine({ 1280, 720 }, "This is a game!");

    gameEngine.run();

    return 0;
}
