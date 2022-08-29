#include "Button.h"
#include <algorithm>


namespace MyChallengeGame
{

    Button::Button()
    {
        
    }

    Button::Button(int32_t x, int32_t y, Texture2D* texture, Rectangle<int> clipRect, std::function<void()> callback)
        : m_Texture(texture), m_clipRect(clipRect)
    {
        position.x = x;
        position.y = y;

        m_ButtonWidth  = clipRect.Width();
        m_ButtonHeight = clipRect.Height();

        m_callback = std::bind(callback);

    }
    Button::~Button()
    {
        
    }

    void Button::Update(Camera2D* camera, float delta)
    {
        // check mouse hittest

        Cursor cursor = Input::GetCursorPosition();
        glm::vec2 mouse = camera->ScreenToWorld(cursor.x, cursor.y);

        if(mouse.x > position.x &&
           mouse.x < position.x + m_ButtonWidth &&
           mouse.y > position.y &&
           mouse.y < position.y + m_ButtonHeight)
        {
            //std::cout << "Button hover!" << std::endl;

            if(Input::IsMouseButtonJustDown(Mouse::MouseButtonLeft))
            {
                m_callback();
            }

        }
    }

    void Button::Draw(Spritebatch* spritebatch, float delta)
    {
        spritebatch->Draw(m_Texture, position.x, position.y, m_clipRect);

    }
}