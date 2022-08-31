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
        private:
        Spritebatch* spritebatch;
        Shader* shader;
        Texture2D* spritesheet;
        Camera2D* camera;
        BitmapFont* font;

        Button playButton;
        Button creditsInButton;
        Button creditsOutButton;

        glm::vec2 gameBoardCenter = glm::vec2(-32, -340+224);
        glm::vec2 gamePieceSize = glm::vec2(64, 64);

        const int MAX_INDEX = 80;

        /* Game state */
        bool showFrameRate    = false;
        bool shouldPause      = false;
        bool animationStarted = false;
        int gameplayCounter       = 0;
        int allCreditsInCounter   = 0;
        int allCreditsOutCounter  = 0;
        int currentCreditsCounter = 0;

        std::vector<GamePiece> pieces;

        public:
        
        ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Game(screenWidth, screenHeight, windowTitle), pieces(MAX_INDEX)
        {
            srand((unsigned)time(NULL));
        }
        ~ChallengeGame()
        {
            delete shader;
            delete spritebatch;
            delete camera;
            delete font;
            delete spritesheet;
        }

        void Run() override
        {
            Game::Run();
        }

        /*
        * Use this function to load and create the game elements
        */
        void Load() override
        {
            glClearColor(0.392, 0.584, 0.929, 1);  // good ol' cornflower blue

            font = new BitmapFont("assets/mbf_big_00.png", 10, 12);

            shader = new Shader("assets/vertex.vert", "assets/fragment.frag");
            shader->use();
            spritesheet = new Texture2D("assets/chips.png", {});

            camera = new Camera2D(GetViewport());
            camera->Position.x = 0;
            camera->Position.y = 0;
            camera->Position.z = 1.0f;
            camera->Zoom = 1.0f;

            spritebatch = new Spritebatch();

            /* create some buttons */

            playButton = Button(GetViewport().HalfWidth() * -1 + 36, GetViewport().HalfHeight() - 36 - 64, spritesheet, {64, 0, 128, 64},
                [=]()-> void
                {   
                    if(animationStarted)
                    {
                        shouldPause = !shouldPause;
                    } else
                    {
                        if(currentCreditsCounter > 0)
                        {
                            ResetAnimation();
                            StartPiecesAnimation();

                            currentCreditsCounter--;

                        } else
                        {
                            std::cout << "No credits available!" << std::endl;
                        }
                    }
                }
            );

            creditsInButton = Button(GetViewport().HalfWidth() - 128 * 2 - 36 - 36, GetViewport().HalfHeight() - 36 - 64, spritesheet, {64 + 128, 0, 128, 64},
                [=]()-> void
                {
                    std::cout << "clicked on credits in" << std::endl;
                    allCreditsInCounter++;
                    currentCreditsCounter++;
                }
            );

            creditsOutButton = Button(GetViewport().HalfWidth() - 128 - 36, GetViewport().HalfHeight() - 36 - 64, spritesheet, {64 + 128, 64, 128, 64},
                [=]()-> void
                {
                    std::cout << "clicked on credits out" << std::endl;
                    allCreditsOutCounter += currentCreditsCounter;
                    currentCreditsCounter = 0;
                }
            );

            // let's try to make our little animation

            int indexNumber = 0;
            
            int directionLoopCounter = 1;
            int directionChanges = 0;

            Direction directions[] = {Direction::Right, Direction::Down, Direction::Left, Direction::Up};
            Direction currentDirection = directions[0];

            while(indexNumber < MAX_INDEX)
            {
                currentDirection = directions[(directionChanges % 4)]; // 4 possible directions

                for(int i = 0; i < directionLoopCounter; i++)
                {
                    if(indexNumber < MAX_INDEX)
                    {
                        CreatePiece(indexNumber, currentDirection);
                        indexNumber++;
                    }

                }

                directionChanges++;

                if(directionChanges % 2 == 0)
                    directionLoopCounter++;

            }
            
        }

        /*
        * This function will be called once per frame.
        * Use this to update you game logic.
        * delta is the amount of time since the last frame
        */
        void Update(float delta) override
        {
            camera->Update(delta);

            playButton.Update(camera, delta);
            creditsInButton.Update(camera, delta);
            creditsOutButton.Update(camera, delta);
            
            if(!shouldPause && animationStarted)
            {
                for(int i = 0; i < MAX_INDEX; i++)
                {
                    pieces[i].Update(delta);

                    if(pieces[i].TweenProgress() > GamePiece::ANIM_DELAY)
                    {
                        pieces[i + 1].StartAnimation();
                    }

                }

                CheckAnimationOver();
            }

            if(Input::IsKeyJustDown(Key::F))
            {
                showFrameRate = !showFrameRate;
            }


        }

        /*
        * Use this function to render your game
        * delta is the amount of time since the last frame
        */
        void Render(float delta) override
        {
            glClear(GL_COLOR_BUFFER_BIT);
            
            spritebatch->Begin(shader, camera, glm::vec4(1));

            for(auto p : pieces)
            {
                p.Draw(spritebatch, delta);
            }

            playButton.Draw(spritebatch, delta);
            creditsInButton.Draw(spritebatch, delta);
            creditsOutButton.Draw(spritebatch, delta);
            spritebatch->End();

            // framerate counter
            if(showFrameRate)
            {
                spritebatch->Begin(shader, camera, glm::vec4(1, 0, 0, 1), 0, true);
                spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(4)));
                spritebatch->DrawString(font, 1, 1, std::to_string(60.0f / (delta*1000.0f)).c_str());
                spritebatch->End();
            }

            // game state render
            float scale = 2.0f;
            spritebatch->Begin(shader, camera, glm::vec4(1), 0, true);
            spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(scale)));            
            glm::vec2 pos1 = camera->WorldToScreen(playButton.position.x, playButton.position.y - 64);
            glm::vec2 pos2 = camera->WorldToScreen(creditsInButton.position.x, creditsInButton.position.y - ((12 + 2) * 2 * 3));
            spritebatch->DrawString(font, pos1.x / scale, pos1.y / scale, (std::string("Number of\nplays: ") + std::to_string(gameplayCounter)).c_str());
            spritebatch->DrawString(font, pos2.x / scale, pos2.y / scale, (std::string("Credits: ") + std::to_string(currentCreditsCounter)).c_str());
            spritebatch->DrawString(font, pos2.x / scale, pos2.y / scale + 12, (std::string("Credits In: ") + std::to_string(allCreditsInCounter)).c_str());
            spritebatch->DrawString(font, pos2.x / scale, pos2.y / scale + 24, (std::string("Credits Out: ") + std::to_string(allCreditsOutCounter)).c_str());
            
            

            if(shouldPause)
                spritebatch->DrawString(font, 64, GetViewport().HalfHeight() - 14, (std::string("(Simulation paused)")).c_str());

            spritebatch->End();

        }

        void CreatePiece(int index, Direction direction)
        {

            glm::vec2 position;
            glm::vec2 destination;

            if(index > 0)
            {
                position = pieces[index - 1].GetPosition();
            }
            else
            {
                position = gameBoardCenter;
            }

            destination = position;

            if(direction == Direction::Left)
            {
                position.x += gamePieceSize.x;
            }
            if(direction == Direction::Right)
            {
                position.x -= gamePieceSize.x;
            }
            if(direction == Direction::Up)
            {
                position.y += gamePieceSize.y;
            }
            if(direction == Direction::Down)
            {
                position.y -= gamePieceSize.y;
            }

            pieces[index] = GamePiece(spritesheet, position, destination);
            
        }

        void StartPiecesAnimation()
        {
            // we start the animation on the first piece, then they will chain start
            pieces[0].StartAnimation();
            animationStarted = true;
        }

        void CheckAnimationOver()
        {
            if(pieces[MAX_INDEX - 1].TweenProgress() >= 0.999998f)
            {
                animationStarted = false;
                gameplayCounter++;
            }
        }

        void ResetAnimation()
        {
            for(int i = MAX_INDEX - 1; i >= 0; i--)
            {
                pieces[i].Reset();
            }
        }

    };

}
