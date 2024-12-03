#include <iostream>
#include <SlateEngine/Engine/Game/Game.h>
#include "Logger.h"
#include "Player.h"

int main(int argc, const char* argv[])
{
    Game game(GetModuleHandle(NULL), L"> SlateEngine | Player [DirectX11.1]", 1280, 720);

    new Player();
    new Logger();

    game.SetPlayer(Player::Instance);

    game.SetLogger(Logger::Instance);

    std::string workingDirectory = argc > 1 ? argv[1] : "ERROR";

    if (workingDirectory == "ERROR") {
        MessageBoxA(0, "Error", "Error", NULL);
        assert(0 < 0, "Error");
        return 0;
    }

    game.SetWorkingDirectory(workingDirectory);

    if (!game.OnInit())
        return 0;

    game.SetGameState(GameState::PLAYING);
    return game.OnRun();
}