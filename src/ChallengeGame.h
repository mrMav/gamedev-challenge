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
        Button* button;

        glm::vec2 dude_position = glm::vec2(0, 0);

        glm::vec2 centerCoord = glm::vec2(0, 0);

        const int MAX_INDEX = 48;
        std::vector<glm::vec2> positions;

        int tintIndex = 0;

        int updateCurrentIndex = 0;
        bool doUpdate = false;
        int updateDirectionLoopCounter = 1;
        int updateDirectionLoopIndex = 0;
        int updateDirectionChanges = 0;

        float moving_dude_x = -200.0f;
        tweeny::tween<float> position_tween = tweeny::from(moving_dude_x).to(100.0f).during(50000).via(tweeny::easing::circularInOut);
        bool updateTween = false;

        public:
        
        ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Game(screenWidth, screenHeight, windowTitle), positions(MAX_INDEX + 1)
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
            delete button;
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
            //spritesheet = new Texture2D("assets/chips.png", {GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true});
            spritesheet = new Texture2D("assets/chips.png", {});

            camera = new Camera2D(GetViewport());
            camera->Position.x = 0;
            camera->Position.y = 0;
            camera->Position.z = 1.0f;
            camera->Zoom = 2.0f;

            spritebatch = new Spritebatch();
            button = new Button(0, 0, spritesheet, {64, 0, 108, 48},
                [=]()-> void
                {
                    std::cout << "clicked on button" << std::endl;
                    tintIndex++;
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
                    PlaceSquare(indexNumber,currentDirection);
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
            button->Update(camera, delta);
            
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

            if(updateTween)
            {
                int dt = delta * 10000;
                moving_dude_x = position_tween.step(dt);

            }

            if(Input::IsKeyJustDown(Key::M))
            {
                moving_dude_x = -200.0f;
                position_tween = tweeny::from(moving_dude_x).to(200.0f - 24.0f).during(50000).via(tweeny::easing::backOut);
                updateTween = true;
                tintIndex++;
            }

            if(doUpdate)
            {

                Direction directions[] = {Direction::Left, Direction::Up, Direction::Right, Direction::Down};
                Direction currentDirection = directions[0];

                currentDirection = directions[(updateDirectionChanges % 4)]; // 4 possible directions

                if(updateDirectionLoopIndex < updateDirectionLoopCounter)
                {
                    MoveSquare(updateCurrentIndex, currentDirection);
                    
                    updateCurrentIndex++;
                    updateDirectionLoopIndex++;

                }
                
                if(updateDirectionLoopIndex >= updateDirectionLoopCounter)
                {
                    updateDirectionChanges++;
                    updateDirectionLoopIndex = 0;

                    if(updateDirectionChanges % 2 == 0)
                        updateDirectionLoopCounter++;
                }

                doUpdate = false;
            }

        }

        /*
        * Use this function to render your game
        * delta is the amount of time since the last frame
        */
        void Render(float delta) override
        {
            glClear(GL_COLOR_BUFFER_BIT);
            
            for(int i = 0; i < MAX_INDEX; i++)
            {
                glm::vec4 tint = i == (tintIndex % MAX_INDEX) ? glm::vec4(1, 0, 0, 1) : glm::vec4(1);

                spritebatch->Begin(shader, camera, tint);
                spritebatch->Draw(dude, positions[i].x, positions[i].y);
                spritebatch->End();
            }

            spritebatch->Begin(shader, camera, glm::vec4(1));
            spritebatch->Draw(spritesheet, 64, 64, {0, 0, 64, 64});
            button->Draw(spritebatch, delta);
            spritebatch->End();

            spritebatch->Begin(shader, camera, glm::vec4(0, 1, 0, 1), 0, true);
            spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(4)));
            spritebatch->DrawString(font, 12, 12, std::to_string(60.0f / (delta*1000.0f)).c_str());
            spritebatch->End();

        }

        void Move(int index, int direction)
        {
            printf("Index: %d moves in direction %d\n", index, direction);
        }

        void PlaceSquare(int index, Direction direction)
        {
            if(index > 0)
                positions[index] = positions[index - 1];

            if(direction == Direction::Left)
            {
                positions[index].x -= dude->GetWidth();     
            }
            if(direction == Direction::Right)
            {
                positions[index].x += dude->GetWidth();     
            }
            if(direction == Direction::Up)
            {
                positions[index].y -= dude->GetHeight();     
            }
            if(direction == Direction::Down)
            {
                positions[index].y += dude->GetHeight();     
            }
        }

        void MoveSquare(int index, Direction direction)
        {

            //printf("Moving index %d in direction %d\n", index, direction);

            if(direction == Direction::Left)
            {
                positions[index].x -= dude->GetWidth();     
            }
            if(direction == Direction::Right)
            {
                positions[index].x += dude->GetWidth();     
            }
            if(direction == Direction::Up)
            {
                positions[index].y -= dude->GetHeight();     
            }
            if(direction == Direction::Down)
            {
                positions[index].y += dude->GetHeight();     
            }
        }

    };

}
