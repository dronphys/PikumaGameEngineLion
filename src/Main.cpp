#include <iostream>
#include "Game/Game.h"
#include "spdlog/spdlog.h"
#include "Logger/Logger.h"

int main(int argc, char* argv[]) {


    spdlog::info("Welcome to spdlog!");
    Logger::EnableANSIColors();
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}