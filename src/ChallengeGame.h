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

        const int MAX_INDEX = 48;

        /* Game state */
        bool shouldPause = false;
        bool animationStarted = false;
        int gameplayCounter = 0;
        int creditsCounter = 0;

        std::vector<GamePiece> pieces;

        public:
        
        ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Game(screenWidth, screenHeight, windowTitle), pieces(MAX_INDEX)
        {
            
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

            font = new BitmapFont("Shaders/mbf_big_00.png", 10, 12);

            shader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");
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
                        shouldPause = !shouldPause; // TODO: fix this
                    } else
                    {
                        if(creditsCounter > 0)
                        {
                            StartPiecesAnimation();

                            creditsCounter--;
                            gameplayCounter++;

                        } else
                        {
                            std::cout << "No credits available!" << std::endl;
                        }
                    }
                }
            );

            creditsInButton = Button(GetViewport().HalfWidth() - 128 * 2 - 36, GetViewport().HalfHeight() - 36 - 64, spritesheet, {64 + 128, 0, 128, 64},
                [=]()-> void
                {
                    std::cout << "clicked on credits in" << std::endl;
                    creditsCounter++;
                }
            );

            creditsOutButton = Button(GetViewport().HalfWidth() - 128 - 36, GetViewport().HalfHeight() - 36 - 64, spritesheet, {64 + 128, 64, 128, 64},
                [=]()-> void
                {
                    std::cout << "clicked on credits out" << std::endl;
                    creditsCounter = 0;
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
            }

            CheckAnimationOver();

            std::cout << glm::to_string(pieces[0].GetPosition()) << std::endl;

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
            spritebatch->Begin(shader, camera, glm::vec4(0, 1, 0, 1), 0, true);
            spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(4)));
            spritebatch->DrawString(font, 1, 1, std::to_string(60.0f / (delta*1000.0f)).c_str());
            spritebatch->End();

            // game state render
            float scale = 2.0f;
            spritebatch->Begin(shader, camera, glm::vec4(1), 0, true);
            spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(scale)));
            //spritebatch->DrawString(font, playButton.position.x, playButton.position.y - 16, (std::string("Number of plays: ") + std::to_string(gameplayCounter)).c_str());
            glm::vec2 pos1 = camera->WorldToScreen(playButton.position.x, playButton.position.y - 64);
            glm::vec2 pos2 = camera->WorldToScreen(creditsInButton.position.x + 60, creditsInButton.position.y - 44);
            spritebatch->DrawString(font, pos1.x / scale, pos1.y / scale, (std::string("Number of\nplays: ") + std::to_string(gameplayCounter)).c_str());
            spritebatch->DrawString(font, pos2.x / scale, pos2.y / scale, (std::string("Credits: ") + std::to_string(creditsCounter)).c_str());
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
                
                // must reset now
                // for(auto p : pieces)
                // {
                //     p.Reset();
                // }

                for(int i = MAX_INDEX - 1; i >= 0; i--)
                {
                    pieces[i].Reset();
                }

            }
        }

    };

}
