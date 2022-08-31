
#include <Engine.h>
#include "tweeny-3.2.0.h"

namespace MyChallengeGame
{
    using namespace Engine;

    // representation of a game piece
    class GamePiece
    {

        public:

        // each individual piece animation duration
        static const int DURATION = 1000;

        // the amount of delay before the other piece starts the animation
        static const float ANIM_DELAY;

        // threshold for anim over check (in percent)
        static const float ANIM_OVER_THRESHOLD;
        
        GamePiece();
        GamePiece(Texture2D* texture, glm::vec2 position, glm::vec2 destination);
        ~GamePiece();

        void Update(float delta);

        void Draw(Spritebatch* spritebatch, float delta);

        void StartAnimation();

        void SetPosition(glm::vec2 pos);

        void SetPosition(float x, float y);

        glm::vec2 GetPosition() const { return m_Position; };

        void SetTween();

        float TweenProgress() const { return m_Tween.progress(); };

        void Reset();

        private:
        
        glm::vec2 m_InitialPosition;
        glm::vec2 m_Position;
        glm::vec2 m_Destination;

        tweeny::tween<float, float> m_Tween;
        Rectangle<int> m_ClipRect = {0, 0, 64, 64};
        Texture2D* m_Texture;

        float m_AnimationStarted = false;

        // used to accumulate time for animation progress
        float m_DeltaAcumulation = 0;

    };


}
