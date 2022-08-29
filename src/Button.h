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

        Button(int32_t x, int32_t y, Texture2D* texture, Rectangle<int> clipRect, std::function<void()> callback = nullptr);
        ~Button();

        void Update(Camera2D* camera, float delta);

        void Draw(Spritebatch* spritebatch, float delta);

        private:

        Texture2D* m_Texture;
        Rectangle<int> m_clipRect;

        int m_ButtonWidth;
        int m_ButtonHeight;

        std::function<void()> m_callback;

    };

}
