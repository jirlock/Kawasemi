
#include "Game.h"

int main()
{
    Game* game = new Game();
    game->Initialize();
    game->RunLoop();
    game->Shutdown();

    return 0;
}
