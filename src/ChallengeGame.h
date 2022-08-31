#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <limits>
#include <math.h>

#include <Engine.h>
#include "tweeny-3.2.0.h"

#include "Button.h"
#include "GamePiece.h"

/*

TODO:
done - make chips random
     - change text button from "play" to "start";
     - change text color to white and tint lettering
done - add stats:
         - # of all credits inserted;
         - # of all credits removed;
         - # of current credits;
         - # of plays
done - make play counter only add up at the end of the play
done - make it so only when button start is pressed again, the board resets
done - rearrange visuals
     - test, test, test (linux and windows)
     - make cmake installs

*/

namespace MyChallengeGame
{
    using namespace Engine;

    enum Direction
    {
        Left = 0, Up, Right, Down
    };

    class ChallengeGame : Game
    {
        public:
        
        ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle);
        ~ChallengeGame();

        /*
        * Entry point for the game.
        * called manually after game initialization
        */
        void Run() override;

        /*
        * Use this function to load and create the game elements
        */
        void Load() override;

        /*
        * This function will be called once per frame.
        * Use this to update you game logic.
        * delta is the amount of time since the last frame
        */
        void Update(float delta) override;

        /*
        * Use this function to render your game
        * delta is the amount of time since the last frame
        */
        void Render(float delta) override;

        void CreatePiece(int index, Direction direction);

        void StartPiecesAnimation();

        void CheckAnimationOver();

        void ResetAnimation();

        private:
        
        Spritebatch* m_Spritebatch;
        Shader*      m_Shader;
        Texture2D*   m_Spritesheet;
        Camera2D*    m_Camera;
        BitmapFont*  m_Font;

        Button m_PlayButton;
        Button m_CreditsInButton;
        Button m_CreditsOutButton;

        // center of the constructed animation
        // (values are offsets from the screen center (camera 0, 0))
        const glm::vec2 m_GameBoardCenter = glm::vec2(-32, -116);

        // the size of each game animation piece
        const glm::vec2 m_GamePieceSize   = glm::vec2(64, 64);

        // the number of created pieces
        const int MAX_INDEX = 80;

        // Game state:

        bool m_ShowFrameRate    = false;
        bool m_ShouldPause      = false;
        bool m_AnimationStarted = false;

        int m_GameplayCounter       = 0;
        int m_AllCreditsInCounter   = 0;
        int m_AllCreditsOutCounter  = 0;
        int m_CurrentCreditsCounter = 0;

        std::vector<GamePiece> m_Pieces;

    };

}
