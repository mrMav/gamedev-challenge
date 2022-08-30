#include "GamePiece.h"

namespace MyChallengeGame
{

        const float GamePiece::ANIM_DELAY = 0.8f;

        GamePiece::GamePiece()
        {

        }

        GamePiece::GamePiece(Texture2D* texture, glm::vec2 position, glm::vec2 destination)
            : m_Texture(texture), m_Position(position)
        {
            // TODO: init clip rect y position to be random

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
            if(m_AnimationStarted)
            {
                std::array<float, 2> result = m_Tween.step(delta * STEP_MULT);
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
            m_Tween = tweeny::from(m_Position.x, m_Position.y)
                .to(m_Destination.x, m_Destination.y)
                .during(DURATION)
                .via(tweeny::easing::bounceOut);
        }

}