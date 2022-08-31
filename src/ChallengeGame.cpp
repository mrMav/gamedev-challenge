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

        m_Font = new BitmapFont("assets/mbf_big_00.png", 10, 12);

        m_Shader = new Shader("assets/vertex.vert", "assets/fragment.frag");
        m_Shader->use();
        m_Spritesheet = new Texture2D("assets/chips.png", {});

        m_Camera = new Camera2D(GetViewport());
        m_Camera->Position.x = 0;
        m_Camera->Position.y = 0;
        m_Camera->Position.z = 1.0f;
        m_Camera->Zoom = 1.0f;

        m_Spritebatch = new Spritebatch();

        /* create some buttons */

        m_PlayButton = Button(GetViewport().HalfWidth() * -1 + 36, GetViewport().HalfHeight() - 36 - 64, m_Spritesheet,
            { 64, 0, 128, 64 },
            { 64 + 128, 0, 128, 64 },
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
                        std::cout << "No credits available!" << std::endl;
                    }
                }
            }
        );

        m_CreditsInButton = Button(GetViewport().HalfWidth() - 128 * 2 - 36 - 36, GetViewport().HalfHeight() - 36 - 64, m_Spritesheet,
            { 64, 64, 128, 64 },
            { 64 + 128, 64, 128, 64},
            [=]()-> void
            {
                std::cout << "clicked on credits in" << std::endl;
                m_AllCreditsInCounter++;
                m_CurrentCreditsCounter++;
            }
        );

        m_CreditsOutButton = Button(GetViewport().HalfWidth() - 128 - 36, GetViewport().HalfHeight() - 36 - 64, m_Spritesheet,
            { 64, 128, 128, 64 },
            { 64 + 128, 128, 128, 64 },
            [=]()-> void
            {
                std::cout << "clicked on credits out" << std::endl;
                m_AllCreditsOutCounter += m_CurrentCreditsCounter;
                m_CurrentCreditsCounter = 0;
            }
        );

        // let's try to make our little animation

        int indexNumber = 0;
        
        int directionLoopCounter = 1;
        int directionChanges = 0;

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
    }

    void ChallengeGame::Update(float delta)
    {
        m_Camera->Update(delta);

        m_PlayButton.Update(m_Camera, delta);
        m_CreditsInButton.Update(m_Camera, delta);
        m_CreditsOutButton.Update(m_Camera, delta);
        
        if(!m_ShouldPause && m_AnimationStarted)
        {
            for(int i = 0; i < MAX_INDEX; i++)
            {
                m_Pieces[i].Update(delta);

                if(m_Pieces[i].TweenProgress() > GamePiece::ANIM_DELAY)
                {
                    if(i + 1 < MAX_INDEX)
                        m_Pieces[i + 1].StartAnimation();
                }

            }

            CheckAnimationOver();
        }

        if(Input::IsKeyJustDown(Key::F))
        {
            m_ShowFrameRate = !m_ShowFrameRate;
        }
    }

    void ChallengeGame::Render(float delta)
    {
        glClear(GL_COLOR_BUFFER_BIT);
            
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
            m_Spritebatch->Begin(m_Shader, m_Camera, glm::vec4(0.1, 1, 0.1, 1), 0, true);
            m_Spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(4)));
            m_Spritebatch->DrawString(m_Font, 1, 1, std::to_string(60.0f / (delta*1000.0f)).c_str());
            m_Spritebatch->End();
        }

        // game state render
        float scale = 2.0f;
        m_Spritebatch->Begin(m_Shader, m_Camera, glm::vec4(1), 0, true);
        m_Spritebatch->SetCustomView(glm::scale(glm::mat4(1), glm::vec3(scale)));            
        glm::vec2 pos1 = m_Camera->WorldToScreen(m_PlayButton.position.x, m_PlayButton.position.y - 64);
        glm::vec2 pos2 = m_Camera->WorldToScreen(m_CreditsInButton.position.x, m_CreditsInButton.position.y - ((12 + 2) * 2 * 3));
        m_Spritebatch->DrawString(m_Font, pos1.x / scale, pos1.y / scale, (std::string("Number of\nplays: ") + std::to_string(m_GameplayCounter)).c_str());
        m_Spritebatch->DrawString(m_Font, pos2.x / scale, pos2.y / scale, (std::string("Credits: ") + std::to_string(m_CurrentCreditsCounter)).c_str());
        m_Spritebatch->DrawString(m_Font, pos2.x / scale, pos2.y / scale + 12, (std::string("Credits In: ") + std::to_string(m_AllCreditsInCounter)).c_str());
        m_Spritebatch->DrawString(m_Font, pos2.x / scale, pos2.y / scale + 24, (std::string("Credits Out: ") + std::to_string(m_AllCreditsOutCounter)).c_str());
        
        

        if(m_ShouldPause)
            m_Spritebatch->DrawString(m_Font, 64, GetViewport().HalfHeight() - 14, (std::string("(Simulation paused)")).c_str());

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
        if(m_Pieces[MAX_INDEX - 1].TweenProgress() >= 0.999998f)
        {
            m_AnimationStarted = false;
            m_GameplayCounter++;
        }
    }

    void ChallengeGame::ResetAnimation()
    {
        for(int i = MAX_INDEX - 1; i >= 0; i--)
        {
            m_Pieces[i].Reset();
        }
    }

}
