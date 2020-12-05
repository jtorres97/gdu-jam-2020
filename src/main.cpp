#include "Game.h"

int main(int argv, char **args)
{
    Game game;
    game.Initialize();
    game.Run();
    game.Close();

    return 0;
}
