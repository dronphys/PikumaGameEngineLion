#include <iostream>
#include "Game/Game.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argv[]) {

    spdlog::info("Welcome to spdlog!");
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}