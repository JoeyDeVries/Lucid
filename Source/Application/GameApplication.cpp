#include "GameApplication.h"
#include "../Resources/ResourceManager.h"

#include <iostream>

std::shared_ptr<GameApplication> GameApplication::m_Instance = std::shared_ptr<GameApplication>();

GameApplication::GameApplication()
{
    m_Scene = new Scene;
    m_ActorFactory = new ActorFactory;
}

GameApplication::~GameApplication()
{
    delete m_Scene;
    delete m_ActorFactory;
}

void GameApplication::Initialize(float width, float height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    // Initialize renderer
    m_Scene->GetRenderer()->Initialize();
    m_Scene->GetCamera()->SetProjection(width, height);
    // Load necessary actors/textures/shaders before loading level
    ResourceManager::GetInstance()->LoadShader("sprite", "shaders/sprite.vs", "shaders/sprite.frag");
    ResourceManager::GetInstance()->LoadTexture("block", "textures/block.png");
    ResourceManager::GetInstance()->LoadTexture("player", "textures/player.png", true);
    ResourceManager::GetInstance()->LoadTexture("background", "textures/background.png");
    ResourceManager::GetInstance()->LoadLevel(m_Scene, "levels/begin.lvl");
    //CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_PLAYER);
}

std::shared_ptr<Actor> GameApplication::CreateActor(DEFAULT_ACTOR_TYPES type)
{
    std::shared_ptr<Actor> actor = m_ActorFactory->CreateActor(type);
    m_Actors.push_back(actor);
    return actor;
}

void GameApplication::Update(float deltaTime)
{
    // Update all actors
    auto it = m_Actors.begin();
    for(auto it = m_Actors.begin(); it != m_Actors.end(); it++)
        (*it)->Update(deltaTime);

    // Update all scene components
    m_Scene->Update(deltaTime);
}

void GameApplication::Render()
{
    m_Scene->Render();
}

void GameApplication::ProcessKeyboardDown(char key)
{
    m_Keys[key] = true;
    std::cout << key << std::endl;
}

void GameApplication::ProcessKeyboardUp(char key)
{
    m_Keys[key] = false;
    m_KeysPressed[key] = false;
    std::cout << "OFF:" << key << std::endl;
}

bool GameApplication::IsKeyPressed(char key, bool check_once)
{
    if (check_once)
    {
        if (m_Keys[key] && !m_KeysPressed[key])
        {
            m_KeysPressed[key] = true;
            return true;
        }
        else
            return false;
    }
    return m_Keys[key];
}


void GameApplication::checkOtherInstances()
{
    // TODO
}

void GameApplication::checkStorage()
{
    // TODO
}

void GameApplication::checkRAM()
{
    // TODO
}