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
    std::string workingDirectory = argc > 1 ? argv[1] : "ERROR";
    std::cout << argv[1];

    game.SetWorkingDirectory(workingDirectory);
    game.SetLogger(Logger::Instance);

    if (!game.OnInit())
        return 0;
    game.SetGameState(GameState::PLAYING);
    return game.OnRun();
}