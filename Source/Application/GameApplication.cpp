#include "GameApplication.h"
#include "Event_StartLevel.h"
#include "Event_QuitGame.h"
#include "../Resources/ResourceManager.h"
#include "../Scene/BackgroundNode.h"
#include "../Scene/SpriteNode.h"
#include "../Components/Event_DestroyActor.h"
#include "../Components/Event_LevelComplete.h"
#include "../Components/Event_DestroyActor.h"
#include "../GUI/GUIMainMenu.h"

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
    // Create level-independant actors
    // - background (no need for it to be related to an actor, only for rendering)
 //   std::shared_ptr<Actor> backgroundActor = CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_EMPTY);
 //   std::shared_ptr<BackgroundNode> backgroundNode(new BackgroundNode(backgroundActor->GetID())); // 0 = no_actor id
	//std::shared_ptr<Material> backgroundMaterial = std::shared_ptr<Material>(new Material());
	//backgroundMaterial->SetShader(ResourceManager::GetInstance()->GetShader("sprite"));
 //   backgroundMaterial->SetDiffuse(ResourceManager::GetInstance()->LoadTexture("background", "textures/background2.png"));
 //   backgroundMaterial->SetSpecular(ResourceManager::GetInstance()->GetTexture("specular"));
 //   backgroundMaterial->SetNormal(ResourceManager::GetInstance()->LoadTexture("background_normal", "textures/background2_normal.png"));
 //   backgroundNode->SetMaterial(backgroundMaterial);
    //m_Scene->AddChild(backgroundActor->GetID(), backgroundNode);  

	// register for global game event
	EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnLevelComplete);
	GetEventManager()->AddListener(eventListener, Event_LevelComplete::s_EventType);
    eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnStartLevel);
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
    for(auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->Init();

    // Start game in main menu
	switchState(GameState::GAME_MAIN_MENU);
}

void GameApplication::CleanUp()
{
    // de-register global game events
    EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnLevelComplete);
    GetEventManager()->RemoveListener(eventListener, Event_LevelComplete::s_EventType);

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
void GameApplication::ClearActors()
{
   /* for (auto it = m_Actors.begin(); it != m_Actors.end(); ++it)
    {
        m_Physics->RemoveActor(it->second->GetID());
        m_Scene->RemoveChild(it->second->GetID());
    }
    m_Physics->RemoveQueuedItems();
    m_Actors.clear();
    m_ImportantActors.clear();*/
}

void GameApplication::switchState(GameState state)
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
    case GameState::GAME_MAIN:
        // JOEY(TODO): m_GUIContainers["game_overlay"]->SetActive(true);  // listens to all game-relevant events and acts accordingly
        m_Audio->PlaySound("audio/ambient2.wav", true, 0.3f);
        break;
    case GameState::GAME_GAME_MENU:
        //m_GUIContainers["game_menu"]->SetActive(true);
        break;
    default:
        break;
    }
}


void GameApplication::Update(float deltaTime)
{
    // Process all queued events
    m_EventManager->Update();

    if (m_GameState == GameState::GAME_MAIN)
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

        // Update all scene components
        m_Scene->Update(deltaTime);
    } 

    // Update GUI components
    for (auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->Update(deltaTime);
}

void GameApplication::Render()
{
    m_Renderer->PreRender(); // TODO(Joey): decouple renderer from scene and link to GameApplication

    if (m_GameState == GameState::GAME_MAIN || m_GameState == GameState::GAME_GAME_MENU)
    {
        m_Scene->Render(m_Renderer);

        if (m_Physics)
        {
            // prepare stuff for debug drawing (note this is not nice MVC stuff, but it's only for debugging)
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf((const GLfloat*)&m_Scene->GetCamera()->GetProjection()[0]);
            glUseProgram(0);
            //m_Physics->RenderDiagnostics();
        }

        //m_TextRenderer->RenderText("Joey de Vries presents...", glm::vec2(300, 660), 2.5f, true, glm::vec4(0.35, 0.35, 0.35, 1.0));
        //m_TextRenderer->RenderText("Lantarn", glm::vec2(570, 340), 6.0f, true, glm::vec4(0.24, 0.05, 0.05, 0.25));

        // render all gameplay-related text (clears queue before GUI text rendering)
        m_TextRenderer->Render(m_Scene->GetCamera()->GetProjection(), m_Scene->GetCamera()->GetView());
    }

    // lastly render GUI elements
    for (auto it = m_GUIContainers.begin(); it != m_GUIContainers.end(); ++it)
        it->second->Render(m_Renderer, m_TextRenderer, m_Scene->GetCamera());
    // then render all GUI queued text
    m_TextRenderer->Render(m_Scene->GetCamera()->GetProjection());
}

void GameApplication::ProcessKeyboardDown(char key)
{
    m_Keys[key] = true;
}

void GameApplication::ProcessKeyboardUp(char key)
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

// global game events
void GameApplication::OnLevelComplete(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_LevelComplete> pEvent = std::dynamic_pointer_cast<Event_LevelComplete>(eventData);
	if (pEvent)
	{
		std::cout << "LEVEL COMPLETE!" << std::endl;
		std::cout << "INITIATE CLEAN-UP..." << std::endl;
		m_Audio->PlaySound("audio/end_hit.wav");
	}
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
        //ClearActors();
        m_Scene->Clear();
        m_EventManager->Clear();
        ResourceManager::GetInstance()->LoadLevel(m_Scene, pEvent->GetLevelPath().c_str());
        m_Scene->Initialize();

        switchState(GameState::GAME_MAIN);
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
            std::cout << "Player destroyed... restarting game" << std::endl;
        }
        else
        {   // destroy actor as normal

        }
    }
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