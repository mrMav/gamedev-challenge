#include "GamePiece.h"

namespace MyChallengeGame
{
        int RandInt(int min, int max)
        {
            // not the ideal random number generator, but it will work in our simple example
            return (rand() % max) + min; 
        }

        const float GamePiece::ANIM_DELAY = 0.005f;
        const float GamePiece::ANIM_OVER_THRESHOLD = 0.999998f;

        GamePiece::GamePiece()
        {

        }

        GamePiece::GamePiece(Texture2D* texture, glm::vec2 position, glm::vec2 destination)
            : m_Texture(texture), m_InitialPosition(position), m_Position(position), m_Destination(destination)
        {
            m_ClipRect.Y = RandInt(0, 7) * 64;   // there are 7 sprites for pieces, each with 64 width

            m_Tween = tweeny::from(position.x, position.y)
                .to(destination.x, destination.y)
                .during(DURATION)
                .via(tweeny::easing::bounceOut);
        };

        GamePiece::~GamePiece()
        {

        };

        void GamePiece::Update(float delta)
        {

            // updating the animation consists in checking if we
            // must step the tween. if so, advance in relative percentage

            if(m_AnimationStarted)
            {
                m_DeltaAcumulation += delta;
                std::array<float, 2> result = m_Tween.step(m_DeltaAcumulation / static_cast<float>(DURATION));
                
                m_Position.x = result[0];
                m_Position.y = result[1];
            }
        }

        void GamePiece::Draw(Spritebatch* spritebatch, float delta)
        {
            spritebatch->Draw(m_Texture, m_Position.x, m_Position.y, m_ClipRect);
        }

        void GamePiece::StartAnimation()
        {
            m_AnimationStarted = true;
        }

        void GamePiece::SetPosition(glm::vec2 pos)
        {
            m_Position = pos;
        }
        
        void GamePiece::SetPosition(float x, float y)
        {
            m_Position.x = x;
            m_Position.y = y;
        }

        void GamePiece::SetTween()
        {
            m_Tween = tweeny::from(m_InitialPosition.x, m_InitialPosition.y)
                .to(m_Destination.x, m_Destination.y)
                .during(DURATION)
                .via(tweeny::easing::bounceOut);
        }

        void GamePiece::Reset()
        {
            m_Tween.seek(0); // resets the tween back
            m_AnimationStarted = false;            
            SetPosition(m_InitialPosition);
            m_DeltaAcumulation = 0;
        }

}