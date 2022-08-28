#include <iostream>

#include "ChallengeGame.h"

int main (int argc, char *argv[])
{

    std::cout << "Hello new project!" << std::endl;

    MyChallengeGame::ChallengeGame game = MyChallengeGame::ChallengeGame(800, 600, "Game Development Challenge");
    game.Run();

    return EXIT_SUCCESS;

}
