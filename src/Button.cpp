#include "Button.h"
#include <algorithm>


namespace MyChallengeGame
{

    Button::Button()
    {
        
    }

    Button::Button(int32_t x, int32_t y, Texture2D* texture, Rectangle<int> clipRect, Rectangle<int> clipRectHover, std::function<void()> callback)
        : m_Texture(texture), m_clipRect(clipRect), m_clipRectHover(clipRectHover)
    {
        Position.x = x;
        Position.y = y;

        m_ButtonWidth  = clipRect.Width();
        m_ButtonHeight = clipRect.Height();

        m_callback = std::bind(callback);

    }
    Button::~Button()
    {
        
    }

    void Button::Update(Camera2D* camera, float delta)
    {
        // check mouse hit-test

        Cursor cursor = Input::GetCursorPosition();
        glm::vec2 mouse = camera->ScreenToWorld(cursor.x, cursor.y);

        if(mouse.x > Position.x &&
           mouse.x < Position.x + m_ButtonWidth &&
           mouse.y > Position.y &&
           mouse.y < Position.y + m_ButtonHeight)
        {
            
            m_Hover = true;

            if(Input::IsMouseButtonJustDown(Mouse::MouseButtonLeft))
            {
                m_callback();
            }

        }
        else
        {
            m_Hover = false;
        }
    }

    void Button::Draw(Spritebatch* spritebatch, float delta)
    {
        if(!m_Hover)
            spritebatch->Draw(m_Texture, Position.x, Position.y, m_clipRect);
        else
            spritebatch->Draw(m_Texture, Position.x, Position.y, m_clipRectHover);
    }
}