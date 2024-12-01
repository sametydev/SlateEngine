#include <iostream>
#include <SlateEngine/Engine/Game/Game.h>
#include "Logger.h"
#include "Player.h"
int main()
{
    Game game(GetModuleHandle(NULL), L"> SlateEngine | Player [DirectX11.1]", 1280, 720);

    new Player();
    new Logger();

    game.SetPlayer(Player::Instance);
    game.SetWorkingDirectory("Projects\\TestProject\\");
    game.SetLogger(Logger::Instance);

    if (!game.OnInit())
        return 0;
    game.SetGameState(GameState::PLAYING);
    return game.OnRun();
}