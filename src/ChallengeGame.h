#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <limits>
#include <math.h>

#include <Engine.h>

using namespace Engine;

namespace MyChallengeGame
{

    class ChallengeGame : Game
    {
        private:
        Spritebatch* spritebatch;
        Shader* shader;
        Texture2D* dude;
        Camera2D* camera;

        glm::vec2 dude_position = glm::vec2(0, 0);

        public:
        
        ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Game(screenWidth, screenHeight, windowTitle)
        {

        }
        ~ChallengeGame()
        {
            delete shader;
            delete dude;
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

            shader = new Shader("Shaders/vertex.vert", "Shaders/fragment.frag");
            shader->use();
            dude = new Texture2D("Shaders/dude1.png", {});

            camera = new Camera2D(GetViewport());
            camera->Position.x = 0;
            camera->Position.y = 0;
            camera->Position.z = 1.0f;
            camera->Zoom = 2.0f;

            spritebatch = new Spritebatch();
            
        }

        /*
        * This function will be called once per frame.
        * Use this to update you game logic.
        * delta is the amount of time since the last frame
        */
        void Update(float delta) override
        {
            camera->Update(delta);

            dude_position.x = GetViewport().HalfWidth();
            dude_position.y = GetViewport().HalfHeight();
        }

        /*
        * Use this function to render your game
        * delta is the amount of time since the last frame
        */
        void Render(float delta) override
        {
            glClear(GL_COLOR_BUFFER_BIT);


            spritebatch->Begin(shader, camera, 0);
            spritebatch->Draw(dude, dude_position.x, dude_position.y, 0);
            spritebatch->End();

        }


    };

}
