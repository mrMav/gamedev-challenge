#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <limits>
#include <math.h>

#include <Engine.h>
#include "tweeny-3.2.0.h"

#include "Button.h"
#include "GamePiece.h"

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
        
        /*
        * Will setup piece at current index based on the direction
        * it should animate
        */
        void CreatePiece(int index, Direction direction);

        /*
        * Init animation state
        */
        void StartPiecesAnimation();

        /*
        * checks if the animation should be over
        */
        void CheckAnimationOver();

        /*
        * Resets the animations to the inital state
        */
        void ResetAnimation();

        private:
        
        // a handy spritebatcher to draw all our elements
        // on the screen. call Begin() before Draw() and End() after
        Spritebatch* m_Spritebatch;
        
        // the shader to be used by our batcher        
        Shader*      m_Shader;

        // the texture that contains all the game graphics
        Texture2D*   m_Spritesheet;

        // the camera that contains the game transform (coord system)
        Camera2D*    m_Camera;

        // a bitmap font to render basic monospaced text
        BitmapFont*  m_Font;

        // UI button elements:

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

        // container for all the game pieces
        std::vector<GamePiece> m_Pieces;

    };

}
