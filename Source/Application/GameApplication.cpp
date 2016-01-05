/*******************************************************************
** Copyright (C) 2014-2015 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Lucid.
** https://github.com/JoeyDeVries/Lucid
**
** Lucid is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#include "GameApplication.h"

#include "Event_StartLevel.h"
#include "Event_QuitGame.h"

#include "../Resources/ResourceManager.h"
#include "../Scene/BackgroundNode.h"
#include "../Scene/SpriteNode.h"
#include "../Components/Event_DestroyActor.h"
//#include "../Components/Event_LevelComplete.h"
#include "../Components/Event_DestroyActor.h"
#include "../Communication/EventManager.h"
#include "../Audio/AudioEngine.h"
#include "../GUI/GUIContainer.h"
#include "../GUI/GUIMainMenu.h"
#include "../GUI/GUISceneIntro.h"
#include "../GUI/GUIGameMenu.h"

#include <iostream>

std::shared_ptr<GameApplication> GameApplication::m_Instance = std::shared_ptr<GameApplication>();

GameApplication::GameApplication() : m_Active(true)
{
    m_Scene        = new Scene;
    m_Renderer     = new Renderer;
    m_EventManager = new EventManager;
    m_ActorFactory = new ActorFactory;
    m_Physics      = new Box2DPhysics;
	m_Audio		   = new AudioEngine;
	m_TextRenderer = new TextRenderer;
}

GameApplication::~GameApplication()
{
    // can't delete pointers here as other destructors still need GameApplication (for de-registering events for example)
}

void GameApplication::Initialize(float width, float height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    // Initialize physics
    m_Physics->Initialize();

    // Initialize renderer
    m_Renderer->Initialize();
    m_Scene->GetCamera()->SetProjection(width, height, 0.0, 50.0f); // has to be initialized first as its projection is also used by GUI menus

    // Load default placeholder textures/shaders before loading level
	ResourceManager::GetInstance()->LoadShader("sprite", "shaders/sprite.vs", "shaders/sprite.frag")->SetInteger("EnableLighting", 1, true);
    ResourceManager::GetInstance()->LoadTexture("specular", "textures/specular.png");
    ResourceManager::GetInstance()->LoadTexture("normal", "textures/normal.png");

    // register for global game event
    EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnStartLevel);
    GetEventManager()->AddListener(eventListener, Event_StartLevel::s_EventType);
    eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnQuitGame);
    GetEventManager()->AddListener(eventListener, Event_QuitGame::s_EventType);
    eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnDestroyActor);
    GetEventManager()->AddListener(eventListener, Event_DestroyActor::s_EventType);

	// load font(s) and initialize text-renderer
	std::shared_ptr<Font> font = ResourceManager::GetInstance()->LoadFont("gui/font.fnt");
	m_TextRenderer->Initialize(font);

    // initialize GUI
    m_GUIContainers["main_menu"] = std::shared_ptr<GUIContainer>(new GUIMainMenu);
	m_GUIContainers["scene_intro"] = std::shared_ptr<GUISceneIntro>(new GUISceneIntro);
    m_GUIContainers["game_menu"] = std::shared_ptr<GUIGameMenu>(new GUIGameMenu);
    for(auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->Init();

    // Start game in main menu
	SwitchState(GameState::GAME_MAIN_MENU);
}

void GameApplication::CleanUp()
{
    // clean-up
    delete m_Scene;
    delete m_Renderer;
    delete m_EventManager;
    delete m_ActorFactory;
    delete m_Physics;
    delete m_Audio;
    delete m_TextRenderer;
}

std::shared_ptr<Actor> GameApplication::CreateActor(DEFAULT_ACTOR_TYPES type)
{
    std::shared_ptr<Actor> actor = m_ActorFactory->CreateActor(type);
    // if we have an emtpy actor type, no need to add it to list of actors.
    // empty actors act merely as decoration and don't need processing
    if(type != DEFAULT_ACTOR_TYPES::ACTOR_EMPTY) 
	    m_Actors[actor->GetID()] = actor;
    return actor;
}

std::shared_ptr<Actor> GameApplication::GetActor(ActorID actorID)
{
	return m_Actors[actorID];
	// iterating through a list is too slow for its real-time purpose and query frequency
	// make a performance-memory tradeoff here by storing actors in a hashmap.
    /*for (auto it = m_Actors.begin(); it != m_Actors.end(); ++it)
        if((*it)->GetID() == actorID)
            return (*it);*/
    return std::shared_ptr<Actor>();
}

void GameApplication::SetImportantActor(std::string name, std::shared_ptr<Actor> actor)
{
	m_ImportantActors[name] = actor;
}

std::shared_ptr<Actor> GameApplication::GetImportantActor(std::string name)
{
	if(m_ImportantActors.find(name) != m_ImportantActors.end())
		return m_ImportantActors[name];
	else
		return std::shared_ptr<Actor>();
}

void GameApplication::SwitchState(GameState state)
{
    m_GameState = (state == GameState::GAME_NONE ? m_GameState : state);

    // by default de-activate all GUI containers and only activate what is relevant
    for (auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->SetActive(false);
	switch (m_GameState)
	{
	case GameState::GAME_MAIN_MENU:
		m_GUIContainers["main_menu"]->SetActive(true);
		break;
	case GameState::GAME_INTRO:
	{
		std::shared_ptr<GUISceneIntro> pGUI = std::dynamic_pointer_cast<GUISceneIntro>(m_GUIContainers["scene_intro"]);
		if (pGUI)
		{
			pGUI->SetIntroText(m_Scene->GetSceneIntro());
			pGUI->SetActive(true);
		}
		break;
	}
    case GameState::GAME_MAIN:
        break;
    case GameState::GAME_GAME_MENU:
        m_GUIContainers["game_menu"]->SetActive(true);
        break;
    default:
        break;
    }
}


void GameApplication::Update(float deltaTime)
{
    // Process all queued events
    m_EventManager->Update();

    if (m_GameState == GameState::GAME_MAIN || m_GameState == GameState::GAME_GAME_MENU || m_GameState == GameState::GAME_INTRO)
    {
        if (m_GameState != GameState::GAME_INTRO && m_GameState != GameState::GAME_GAME_MENU)
        {
            // Update all actors
            auto it = m_Actors.begin();
            for (auto it = m_Actors.begin(); it != m_Actors.end(); it++)
                it->second->Update(deltaTime);

            // Process physics
            if (m_Physics)
            {
                m_Physics->Update();
                m_Physics->SyncVisibleScene();
                // process any deletes if occured
                m_Physics->RemoveQueuedItems();
            }
        }

        // Update all scene components
        m_Scene->Update(deltaTime);
    } 

    // Update GUI components
    for (auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->Update(deltaTime);
}

void GameApplication::Render()
{
    m_Renderer->PreRender();

    if (m_GameState == GameState::GAME_MAIN || m_GameState == GameState::GAME_INTRO || m_GameState == GameState::GAME_GAME_MENU)
    {
        m_Scene->Render(m_Renderer);

        if (m_Physics)
        {
            // prepare stuff for debug drawing (note this is not nice MVC stuff, but it's only for debugging)
            glMatrixMode(GL_PROJECTION);
            glm::mat4 viewProjection = m_Scene->GetCamera()->GetProjection() * m_Scene->GetCamera()->GetView();
            glLoadMatrixf((const GLfloat*)&viewProjection[0]);
            glUseProgram(0);
            //m_Physics->RenderDiagnostics();
        }

        // render all gameplay-related text (clears queue before GUI text rendering)
        m_TextRenderer->Render(m_Scene->GetCamera()->GetProjection(), m_Scene->GetCamera()->GetView());
    }

    // lastly render GUI elements
    for (auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->Render(m_Renderer, m_TextRenderer, m_Scene->GetCamera());
    // then render all GUI queued text
    m_TextRenderer->Render(m_Scene->GetCamera()->GetProjection());
}

void GameApplication::ProcessKeyboardDown(short key)
{
    if (key == GLFW_KEY_ESCAPE && (m_GameState == GameState::GAME_MAIN || m_GameState == GameState::GAME_GAME_MENU))
        SwitchState(m_GameState == GameState::GAME_MAIN ? GameState::GAME_GAME_MENU : GameState::GAME_MAIN);
    m_Keys[key] = true;
}

void GameApplication::ProcessKeyboardUp(short key)
{
    m_Keys[key] = false;
    m_KeysPressed[key] = false;
}

void GameApplication::ProcessMouseMove(float x, float y)
{
    for(auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->OnMouseMove(x, y);
}

void GameApplication::ProcessMouseClick(bool leftButton)
{
    for (auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->OnMouseClick(leftButton);
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

void GameApplication::OnStartLevel(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_StartLevel> pEvent = std::dynamic_pointer_cast<Event_StartLevel>(eventData);
    if (pEvent)
    {
        // remove current list of actors
        for (auto it = m_Actors.begin(); it != m_Actors.end(); ++it)
        {
            m_Physics->RemoveActor(it->second->GetID());
            m_Scene->RemoveChild(it->second->GetID());
            it->second->Destroy();
        }
        m_Actors.clear();
        m_ImportantActors.clear();
        m_Physics->RemoveQueuedItems();
        m_Physics->Reset();
        m_Audio->StopAll();
        m_Scene->Clear();
        m_EventManager->Clear();
        ResourceManager::GetInstance()->LoadLevel(m_Scene, pEvent->GetLevelPath().c_str());
        m_Scene->Initialize();

        SwitchState(GameState::GAME_INTRO);
    }
}

void GameApplication::OnQuitGame(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_QuitGame> pEvent = std::dynamic_pointer_cast<Event_QuitGame>(eventData);
    if (pEvent)
    {
        m_Active = false;
    }
}

void GameApplication::OnDestroyActor(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_DestroyActor> pEvent = std::dynamic_pointer_cast<Event_DestroyActor>(eventData);
    if (pEvent)
    {
        if (pEvent->GetActorID() == m_ImportantActors["player"]->GetID())
        {
            // remove current list of actors
            for (auto it = m_Actors.begin(); it != m_Actors.end(); ++it)
            {
                m_Physics->RemoveActor(it->second->GetID());
                m_Scene->RemoveChild(it->second->GetID());
                it->second->Destroy();
            }
            m_Actors.clear();
            m_ImportantActors.clear();
            m_Physics->RemoveQueuedItems();
            m_Physics->Reset();
            m_Audio->StopAll();
            m_Scene->Clear();
            m_EventManager->Clear();
            GetAudio()->PlaySound("audio/death.mp3");

            ResourceManager::GetInstance()->LoadLevel(m_Scene, m_Scene->GetScenePath().c_str());
            m_Scene->Initialize();
        }
        else
        {   // destroy actor as normal
            m_Physics->RemoveActor(pEvent->GetActorID());
            m_Scene->RemoveChild(pEvent->GetActorID());
            m_Actors[pEvent->GetActorID()]->Destroy();
            m_Actors.erase(pEvent->GetActorID());
        }
    }
}