#pragma once

#include <Engine.h>
#include <functional>

namespace MyChallengeGame
{

    using namespace Engine;

    class Button
    {
        public:

        // the button position for bounds checking and rendering
        glm::vec2 Position;

        Button();
        Button(int32_t x, int32_t y, Texture2D* texture, Rectangle<int> clipRect, Rectangle<int> clipRectHover, std::function<void()> callback = nullptr);
        ~Button();

        void Update(Camera2D* camera, float delta);

        void Draw(Spritebatch* spritebatch, float delta);

        private:
        
        // clip rects used for rendering a portion of the texture
        Rectangle<int> m_clipRect      = {0, 0, 0, 0};
        Rectangle<int> m_clipRectHover = {0, 0, 0, 0};

        int m_ButtonWidth  = 0;
        int m_ButtonHeight = 0;

        Texture2D* m_Texture = nullptr;

        bool m_Hover = false;

        std::function<void()> m_callback;
        
    };

}
