#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <limits>
#include <math.h>

#include <Engine.h>
#include "../external/tweeny-3.2.0.h"

#include "Button.h"


using namespace Engine;

namespace MyChallengeGame
{

    enum Direction
    {
        Left = 0, Up, Right, Down
    };

    class ChallengeGame : Game
    {
        private:
        Spritebatch* spritebatch;
        Shader* shader;
        Texture2D* dude;
        Texture2D* spritesheet;
        Camera2D* camera;
        BitmapFont* font;

        Button playButton;
        Button creditsInButton;
        Button creditsOutButton;

        glm::vec2 dude_position = glm::vec2(0, 0);

        glm::vec2 gameBoardCenter = glm::vec2(-32, -340+224);
        glm::vec2 gamePieceSize = glm::vec2(64, 64);

        const int MAX_INDEX = 48;
        std::vector<glm::vec2> positions;

        int tintIndex = 0;

        int updateCurrentIndex = 0;
        bool doUpdate = false;
        int updateDirectionLoopCounter = 1;
        int updateDirectionLoopIndex = 0;
        int updateDirectionChanges = 0;

        float moving_dude_x = -200.0f;
        float moving_dude_y = -200.0f;
        tweeny::tween<float, float> position_tween = tweeny::from(moving_dude_x, moving_dude_y).to(100.0f, 50.0f).during(50000).via(tweeny::easing::circularInOut);
        bool updateTween = false;

        /* Game state */

        int gameplayCounter = 0;
        int creditsCounter = 0;

        std::vector<glm::vec2> piecesPositions;
        std::vector<tweeny::tween<float, float>> piecesPositionsTweens;
        std::vector<bool> pieceUpdateStatus;

        //GAME_PIECE_TWEEN_DURATION).via(TWEEN_PIECE_EASING);
        const int GAME_PIECE_TWEEN_DURATION = 100000;
        const tweeny::easing::cubicOutEasing TWEEN_PIECE_EASING = tweeny::easing::cubicOut;

        public:
        
        ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Game(screenWidth, screenHeight, windowTitle), piecesPositions(MAX_INDEX + 1), piecesPositionsTweens(MAX_INDEX + 1), pieceUpdateStatus(MAX_INDEX + 1)
        {
            
        }
        ~ChallengeGame()
        {
            delete shader;
            delete dude;
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
            dude = new Texture2D("Shaders/dude1.png", {});
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
                    std::cout << "clicked on button play" << std::endl;
                    tintIndex++;
    
                    doUpdate = !doUpdate; // TODO: fix this

                    if(creditsCounter > 0)
                    {
                        pieceUpdateStatus[0] = true;
                        creditsCounter = std::max(0, creditsCounter - 1);
                        gameplayCounter++;
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
                    //Move(indexNumber, currentDirection);
                    CreatePiece(indexNumber, currentDirection);
                    indexNumber++;
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
            
            if(Input::IsKeyJustDown(Key::Enter))
            {
                doUpdate = true;

                // reset 
                if(updateCurrentIndex >= MAX_INDEX)
                {
                    doUpdate = false;
                    // no more animation
                }

            }

            if(Input::IsKeyJustDown(Key::S))
            {
                doUpdate = true;

            }

            if(updateTween)
            {
                int dt = delta * 10000;
                std::array<float, 2> r = position_tween.step(dt);
                moving_dude_x = r[0];
                moving_dude_y = r[1];

            }

            if(Input::IsKeyJustDown(Key::M))
            {
                moving_dude_x = -200.0f;
                moving_dude_x = -200.0f;
                position_tween = tweeny::from(moving_dude_x, moving_dude_y).to(100.0f, 50.0f).during(50000).via(tweeny::easing::circularInOut);
                updateTween = true;
                tintIndex++;
            }

            // if(doUpdate)
            // {

            //     Direction directions[] = {Direction::Left, Direction::Up, Direction::Right, Direction::Down};
            //     Direction currentDirection = directions[0];

            //     currentDirection = directions[(updateDirectionChanges % 4)]; // 4 possible directions

            //     if(updateDirectionLoopIndex < updateDirectionLoopCounter)
            //     {
            //         MoveSquare(updateCurrentIndex, currentDirection);
                    
            //         updateCurrentIndex++;
            //         updateDirectionLoopIndex++;

            //     }
                
            //     if(updateDirectionLoopIndex >= updateDirectionLoopCounter)
            //     {
            //         updateDirectionChanges++;
            //         updateDirectionLoopIndex = 0;

            //         if(updateDirectionChanges % 2 == 0)
            //             updateDirectionLoopCounter++;
            //     }

            //     doUpdate = false;
            // }

            if(doUpdate)
            {

                for(int i = 0; i < MAX_INDEX; i++)
                {
                    if(pieceUpdateStatus[i])
                    {
                        std::array<float, 2> result = piecesPositionsTweens[i].step(delta);
                        piecesPositions[i].x = result[0];
                        piecesPositions[i].y = result[1];

                        // if the current tween progress is above a certain percentage, trigger next one
                        if(piecesPositionsTweens[i].progress() > 0.25f)
                        {
                            pieceUpdateStatus[i + 1] = true;
                        }

                    }

                }

                //doUpdate = false;
            }

        }

        /*
        * Use this function to render your game
        * delta is the amount of time since the last frame
        */
        void Render(float delta) override
        {
            glClear(GL_COLOR_BUFFER_BIT);
            
            // for(int i = 0; i < MAX_INDEX; i++)
            // {
            //     glm::vec4 tint = i == (tintIndex % MAX_INDEX) ? glm::vec4(1, 0, 0, 1) : glm::vec4(1);

            //     spritebatch->Begin(shader, camera, tint);
            //     spritebatch->Draw(spritesheet, positions[i].x, positions[i].y, {0, 0, 64, 64});
            //     spritebatch->End();
            // }

            spritebatch->Begin(shader, camera, glm::vec4(1));

            spritebatch->Draw(spritesheet, moving_dude_x, moving_dude_y, {0, 64, 64, 64});


            for(int i = 0; i < MAX_INDEX; i++)
            {
                spritebatch->Draw(spritesheet, piecesPositions[i].x, piecesPositions[i].y, {0, 0, 64, 64});
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
            if(index > 0)
            {
                piecesPositions[index] = piecesPositions[index -1];
            }
            else
            {
                piecesPositions[index] = gameBoardCenter;
            }

            if(direction == Direction::Left)
            {
                piecesPositions[index].x += gamePieceSize.x;

                piecesPositionsTweens[index] =
                    tweeny::from(piecesPositions[index].x, piecesPositions[index].y)
                    .to(piecesPositions[index].x - gamePieceSize.x, piecesPositions[index].y)
                    .during(GAME_PIECE_TWEEN_DURATION)
                    .via(TWEEN_PIECE_EASING);
            }
            if(direction == Direction::Right)
            {
                piecesPositions[index].x -= gamePieceSize.x;

                piecesPositionsTweens[index] =
                    tweeny::from(piecesPositions[index].x, piecesPositions[index].y)
                    .to(piecesPositions[index].x + gamePieceSize.x, piecesPositions[index].y)
                    .during(GAME_PIECE_TWEEN_DURATION)
                    .via(TWEEN_PIECE_EASING);
            }
            if(direction == Direction::Up)
            {
                piecesPositions[index].y += gamePieceSize.y;

                piecesPositionsTweens[index] =
                    tweeny::from(piecesPositions[index].x, piecesPositions[index].y)
                    .to(piecesPositions[index].x, piecesPositions[index].y - gamePieceSize.y)
                    .during(GAME_PIECE_TWEEN_DURATION)
                    .via(TWEEN_PIECE_EASING);
            }
            if(direction == Direction::Down)
            {
                piecesPositions[index].y -= gamePieceSize.y;

                piecesPositionsTweens[index] =
                    tweeny::from(piecesPositions[index].x, piecesPositions[index].y)
                    .to(piecesPositions[index].x, piecesPositions[index].y + gamePieceSize.y)
                    .during(GAME_PIECE_TWEEN_DURATION)
                    .via(TWEEN_PIECE_EASING);
            }
        }

        void MoveSquare(int index, Direction direction)
        {

            //printf("Moving index %d in direction %d\n", index, direction);

            if(direction == Direction::Left)
            {
                positions[index].x -= gamePieceSize.x;
            }
            if(direction == Direction::Right)
            {
                positions[index].x += gamePieceSize.x;  
            }
            if(direction == Direction::Up)
            {
                positions[index].y -= gamePieceSize.y;
            }
            if(direction == Direction::Down)
            {
                positions[index].y += gamePieceSize.y;
            }
        }

    };

}
