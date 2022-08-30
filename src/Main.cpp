#include <iostream>

#include "ChallengeGame.h"

int main (int argc, char *argv[])
{

    MyChallengeGame::ChallengeGame game = MyChallengeGame::ChallengeGame(648, 808, "GameDevelopmentChallenge_JorgeNoro");
    game.Run();

    return EXIT_SUCCESS;

}
