#include <iostream>

#include "ChallengeGame.h"

int main (int argc, char *argv[])
{

    std::cout << "Hello new project!" << std::endl;

    MyChallengeGame::ChallengeGame game = MyChallengeGame::ChallengeGame(520, 680, "GameDevelopmentChallenge_JorgeNoro");
    game.Run();

    return EXIT_SUCCESS;

}
