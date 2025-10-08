#include <iostream>
#include "Game/Game.h"
#include "spdlog/spdlog.h"
#include "Logger/Logger.h"
#include <sol/sol.hpp>


void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file("../assets/scripts/myscript.lua");
    int someVariableCpp = lua["some_variable"];
    std::cout << "someVariableCpp: " << someVariableCpp << std::endl;
}
int main(int argc, char* argv[]) {

    TestLua();

    // spdlog::info("Welcome to spdlog!");
    // Logger::EnableANSIColors();
    // Game game;
    // game.Initialize();
    // game.Run();
    // game.Destroy();

    return 0;
}