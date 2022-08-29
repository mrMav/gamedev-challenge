#pragma once

#include <Engine.h>
#include <functional>

namespace MyChallengeGame
{

    using namespace Engine;

    class Button
    {
        public:

        glm::vec2 position;

        Button();
        Button(int32_t x, int32_t y, Texture2D* texture, Rectangle<int> clipRect, std::function<void()> callback = nullptr);
        ~Button();

        void Update(Camera2D* camera, float delta);

        void Draw(Spritebatch* spritebatch, float delta);

        private:

        Texture2D* m_Texture = nullptr;
        Rectangle<int> m_clipRect = {0, 0, 0, 0};

        int m_ButtonWidth = 0;
        int m_ButtonHeight = 0;

        std::function<void()> m_callback;

    };

}
