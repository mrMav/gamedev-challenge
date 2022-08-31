#include "ChallengeGame.h"

namespace MyChallengeGame
{

    ChallengeGame::ChallengeGame(uint32_t screenWidth, uint32_t screenHeight, const char* windowTitle)
        : Game(screenWidth, screenHeight, windowTitle), m_Pieces(MAX_INDEX)
    {
        srand((unsigned)time(NULL));
    }

    ChallengeGame::~ChallengeGame()
    {
        delete m_Shader;
        delete m_Spritebatch;
        delete m_Camera;
        delete m_Font;
        delete m_Spritesheet;
    }

    void ChallengeGame::Run()
    {
        Game::Run();
    }

    void ChallengeGame::Load()
    {   
        // sets the clear color for the framebuffer
        glClearColor(0.392, 0.584, 0.929, 1);  // good ol' cornflower blue

        // loading assets:
        m_Font = new BitmapFont("assets/mbf_big_00.png", 10, 12);
        m_Shader = new Shader("assets/vertex.vert", "assets/fragment.frag");
        m_Spritesheet = new Texture2D("assets/chips.png", {});

        m_Camera = new Camera2D(GetViewport());
        m_Camera->Position.x = 0;
        m_Camera->Position.y = 0;
        m_Camera->Position.z = 1;
        m_Camera->Zoom       = 1;

        m_Spritebatch = new Spritebatch();

        // create UI buttons:
        // button textures in the spritesheet have offsets of 64 and 128

        // the margin added to the UI elements around the edge of the window
        const int MARGIN        = 36;        
        const int PADDING       = 24;
        const int BUTTON_WIDTH  = 128;
        const int BUTTON_HEIGHT = 64;

        m_PlayButton = Button(GetViewport().HalfWidth() * -1 + MARGIN, GetViewport().HalfHeight() - MARGIN - BUTTON_HEIGHT, m_Spritesheet,
            { 64, 0, BUTTON_WIDTH, BUTTON_HEIGHT },  
            { 64 + BUTTON_WIDTH, 0, BUTTON_WIDTH, BUTTON_HEIGHT },   
            [=]()-> void
            {   
                if(m_AnimationStarted)
                {
                    m_ShouldPause = !m_ShouldPause;
                } else
                {
                    if(m_CurrentCreditsCounter > 0)
                    {
                        ResetAnimation();
                        StartPiecesAnimation();

                        m_CurrentCreditsCounter--;

                    } else
                    {
                        //std::cout << "No credits available!" << std::endl;
                    }
                }
            }
        );

        m_CreditsOutButton = Button(GetViewport().HalfWidth() - BUTTON_WIDTH - MARGIN, m_PlayButton.Position.y, m_Spritesheet,
            { 64, 128, BUTTON_WIDTH, BUTTON_HEIGHT },
            { 64 + BUTTON_WIDTH, 64 * 2, BUTTON_WIDTH, BUTTON_HEIGHT },
            [=]()-> void
            {
                m_AllCreditsOutCounter += m_CurrentCreditsCounter;
                m_CurrentCreditsCounter = 0;
            }
        );

        m_CreditsInButton = Button(m_CreditsOutButton.Position.x - BUTTON_WIDTH - PADDING, m_PlayButton.Position.y, m_Spritesheet,
            { 64, 64, BUTTON_WIDTH, BUTTON_HEIGHT },
            { 64 + BUTTON_WIDTH, 64, BUTTON_WIDTH, BUTTON_HEIGHT},
            [=]()-> void
            {
                m_AllCreditsInCounter++;
                m_CurrentCreditsCounter++;
            }
        );


        // build animation:
        // we will build a little animation consisting of game pieces falling into place one by one
        // we will create the pieces and tell them in which direction they should "fall"
        // -----
        // -6789
        // -5 1-
        // -432-
        // -----
        // 
        // in the "sketch" above, piece 1 will move to the left, leaving a gap in its original
        // place. piece 2 will then move up, filling the gap. Then piece 3 will move to the right
        // filling the gap left by number 2. 

        int indexNumber          = 0;
        int directionLoopCounter = 1;
        int directionChanges     = 0;

        // this array tells the animation order
        Direction directions[] = {Direction::Right, Direction::Down, Direction::Left, Direction::Up};
        Direction currentDirection = directions[0];

        while(indexNumber < MAX_INDEX)
        {
            currentDirection = directions[(directionChanges % 4)]; // 4 possible directions

            for(int i = 0; i < directionLoopCounter; i++)
            {
                if(indexNumber < MAX_INDEX)
                {
                    CreatePiece(indexNumber, currentDirection);
                    indexNumber++;
                }

            }

            directionChanges++;

            if(directionChanges % 2 == 0)
                directionLoopCounter++;

        }

        // after this loop, the m_Pieces vector will have the pieces ordered like in the "sketch"
    }

    void ChallengeGame::Update(float delta)
    {
        m_Camera->Update(delta);

        m_PlayButton.Update(m_Camera, delta);
        m_CreditsInButton.Update(m_Camera, delta);
        m_CreditsOutButton.Update(m_Camera, delta);
        
        // here we control the animation state
        if(!m_ShouldPause && m_AnimationStarted)
        {
            for(int i = 0; i < MAX_INDEX; i++)
            {
                m_Pieces[i].Update(delta);

                // if the current updating piece animation progress milestone is met, we
                // trigger the next piece to start it's animation
                if(m_Pieces[i].TweenProgress() > GamePiece::ANIM_DELAY)
                {
                    if(i + 1 < MAX_INDEX)
                        m_Pieces[i + 1].StartAnimation();
                }

            }

            CheckAnimationOver();
        }

        // just some more info
        // displays framerate on screen
        if(Input::IsKeyJustDown(Key::F))
        {
            m_ShowFrameRate = !m_ShowFrameRate;
        }
    }

    void ChallengeGame::Render(float delta)
    {
        // our engine is not mature enough to have it's own render device
        // so we must call the clear function directly to opengl...
        glClear(GL_COLOR_BUFFER_BIT);
        
        // this spritebatch step will render everything that is in "world" coordinates
        m_Spritebatch->Begin(m_Shader, m_Camera, glm::vec4(1));

        for(auto p : m_Pieces)
        {
            p.Draw(m_Spritebatch, delta);
        }

        m_PlayButton.Draw(m_Spritebatch, delta);
        m_CreditsInButton.Draw(m_Spritebatch, delta);
        m_CreditsOutButton.Draw(m_Spritebatch, delta);
        m_Spritebatch->End();

        // framerate counter
        if(m_ShowFrameRate)
        {
            // because we tint the framerate counter, we need a new draw call
            m_Spritebatch->Begin(m_Shader, m_Camera, glm::vec4(0.1, 1, 0.1, 1), 0, true);
            m_Spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(4)));
            m_Spritebatch->DrawString(m_Font, 1, 1, std::to_string(60.0f / (delta*1000.0f)).c_str());  // TODO: this should be an averaged value
            m_Spritebatch->End();
        }

        // game state render
        // our current font is too small, so we will scale this pat of the UI
        // by using a custom transform
        // some of the positioning values are just numbers that fitted this current application
        // our engine doesn't have flex containers for UI or anything like that yet
        const float SCALE = 2.0f;

        glm::vec2 playTextPosition    = m_Camera->WorldToScreen(m_PlayButton.Position.x, m_PlayButton.Position.y - 64);  
        glm::vec2 creditsTextPosition = m_Camera->WorldToScreen(m_CreditsInButton.Position.x, m_CreditsInButton.Position.y - ((m_Font->CharHeight() + 2) * 2 * 3));

        m_Spritebatch->Begin(m_Shader, m_Camera, glm::vec4(1), 0, true);
        m_Spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(SCALE)));     

        m_Spritebatch->DrawString(m_Font,
            playTextPosition.x / SCALE, playTextPosition.y / SCALE,
            (std::string("Number of\nplays: ") + std::to_string(m_GameplayCounter)).c_str()
        );
        m_Spritebatch->DrawString(m_Font,
            creditsTextPosition.x / SCALE, creditsTextPosition.y / SCALE,
            (std::string("Credits: ") + std::to_string(m_CurrentCreditsCounter)).c_str()
        );
        m_Spritebatch->DrawString(m_Font,
            creditsTextPosition.x / SCALE, creditsTextPosition.y / SCALE + m_Font->CharHeight(),
            (std::string("Credits In: ") + std::to_string(m_AllCreditsInCounter)).c_str()
        );
        m_Spritebatch->DrawString(m_Font,
            creditsTextPosition.x / SCALE, creditsTextPosition.y / SCALE + m_Font->CharHeight() * 2,
            (std::string("Credits Out: ") + std::to_string(m_AllCreditsOutCounter)).c_str()
        );
        
        // render pause state text
        // again, positioning values are just numbers that fitted this current application
        if(m_ShouldPause)
            m_Spritebatch->DrawString(m_Font,
            64, GetViewport().HalfHeight() - m_Font->CharHeight() - 2,
            (std::string("(Simulation paused)")).c_str()
        );

        m_Spritebatch->End();
    }

    void ChallengeGame::CreatePiece(int index, Direction direction)
    {
        glm::vec2 position;
        glm::vec2 destination;

        if(index > 0)
        {
            position = m_Pieces[index - 1].GetPosition();
        }
        else
        {
            position = m_GameBoardCenter;
        }

        destination = position;

        if(direction == Direction::Left)
        {
            position.x += m_GamePieceSize.x;
        }
        if(direction == Direction::Right)
        {
            position.x -= m_GamePieceSize.x;
        }
        if(direction == Direction::Up)
        {
            position.y += m_GamePieceSize.y;
        }
        if(direction == Direction::Down)
        {
            position.y -= m_GamePieceSize.y;
        }

        m_Pieces[index] = GamePiece(m_Spritesheet, position, destination);
    }

    void ChallengeGame::StartPiecesAnimation()
    {
        // we start the animation on the first piece, then they will chain start
        m_Pieces[0].StartAnimation();
        m_AnimationStarted = true;
    }

    void ChallengeGame::CheckAnimationOver()
    {
        if(m_Pieces[MAX_INDEX - 1].TweenProgress() >= GamePiece::ANIM_OVER_THRESHOLD)
        {
            m_AnimationStarted = false;
            m_GameplayCounter++;
        }
    }

    void ChallengeGame::ResetAnimation()
    {
        // we must cycle backwards in order to
        // effectively change the pieces state
        for(int i = MAX_INDEX - 1; i >= 0; i--)
        {
            m_Pieces[i].Reset();
        }
    }

}
