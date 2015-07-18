#include "GameApplication.h"
#include "../Resources/ResourceManager.h"

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

void GameApplication::Initialize()
{
    // Load necessary level/actors/textures/shaders
    //ResourceManager::GetInstance()->LoadLevel(GetScene(), "levels/level1");
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
}

void GameApplication::ProcessKeyboardUp(char key)
{
    m_Keys[key] = false;
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