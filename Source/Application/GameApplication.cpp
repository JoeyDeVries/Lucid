#include "GameApplication.h"
#include "../Resources/ResourceManager.h"
#include "../Scene/BackgroundNode.h"
#include "../Scene/SpriteNode.h"
#include "../Components/Event_DestroyActor.h"
#include "../Components/Event_LevelComplete.h"

#include <iostream>

std::shared_ptr<GameApplication> GameApplication::m_Instance = std::shared_ptr<GameApplication>();

GameApplication::GameApplication()
{
    m_Scene = new Scene;
    m_EventManager = new EventManager;
    m_ActorFactory = new ActorFactory;
    m_Physics      = new Box2DPhysics;
	m_Audio		   = new AudioEngine;
}

GameApplication::~GameApplication()
{
	// de-register global game events
	EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnLevelComplete);
	GetEventManager()->RemoveListener(eventListener, Event_LevelComplete::s_EventType);

	// clean-up
    delete m_Scene;
    delete m_EventManager;
    delete m_ActorFactory;
    delete m_Physics;
	delete m_Audio;
}

void GameApplication::Initialize(float width, float height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    // Initialize physics
    m_Physics->Initialize();
    // Initialize renderer
    m_Scene->GetRenderer()->Initialize();
    m_Scene->GetCamera()->SetProjection(width, height, 0.0, 50.0f);
    // Load default placeholder textures/shaders before loading level
	ResourceManager::GetInstance()->LoadShader("sprite", "shaders/sprite.vs", "shaders/sprite.frag");
    ResourceManager::GetInstance()->LoadTexture("specular", "textures/specular.png");
    ResourceManager::GetInstance()->LoadTexture("normal", "textures/normal.png");
    ResourceManager::GetInstance()->LoadLevel(m_Scene, "levels/begin.lvl");
    // Create level-independant actors
    // - background (no need for it to be related to an actor, only for rendering)
    std::shared_ptr<Actor> backgroundActor = CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_EMPTY);
    std::shared_ptr<BackgroundNode> backgroundNode(new BackgroundNode(backgroundActor->GetID())); // 0 = no_actor id
	std::shared_ptr<Material> backgroundMaterial = std::shared_ptr<Material>(new Material());
	backgroundMaterial->SetShader(ResourceManager::GetInstance()->GetShader("sprite"));
    backgroundMaterial->SetDiffuse(ResourceManager::GetInstance()->LoadTexture("background", "textures/background2.png"));
    backgroundMaterial->SetSpecular(ResourceManager::GetInstance()->GetTexture("specular"));
    backgroundMaterial->SetNormal(ResourceManager::GetInstance()->LoadTexture("background_normal", "textures/background2_normal.png"));
    backgroundNode->SetMaterial(backgroundMaterial);
    m_Scene->AddChild(backgroundActor->GetID(), backgroundNode);  

	// register for global game event
	EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnLevelComplete);
	GetEventManager()->AddListener(eventListener, Event_LevelComplete::s_EventType);

    // initialize scene (e.g. set default projection matrices for each shader)
    m_Scene->Initialize();

	// set background music TODO(Joey): let it belong to the level/scene
	m_Audio->PlaySound("audio/ambient2.wav", true, 0.2f);

}

std::shared_ptr<Actor> GameApplication::CreateActor(DEFAULT_ACTOR_TYPES type)
{
    std::shared_ptr<Actor> actor = m_ActorFactory->CreateActor(type);
	m_Actors[actor->GetID()] = actor;
    //m_Actors.push_back(actor);
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


void GameApplication::Update(float deltaTime)
{
    // Update all actors
    auto it = m_Actors.begin();
    for(auto it = m_Actors.begin(); it != m_Actors.end(); it++)
        it->second->Update(deltaTime);

    if (m_Keys[GLFW_KEY_5])
    {
        std::shared_ptr<Event_DestroyActor> pEvent(new Event_DestroyActor(1337));
        m_EventManager->QueueEvent(pEvent);
    }

    // Process all queued events
    m_EventManager->Update();

    // Process physics
    if (m_Physics)
    {
        m_Physics->Update();
        m_Physics->SyncVisibleScene();
    }

    // Update all scene components
    m_Scene->Update(deltaTime);
}

void GameApplication::Render()
{
    m_Scene->Render();
    if (m_Physics)
    {
        // prepare stuff for debug drawing (note this is not nice MVC stuff, but it's only for debugging)
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf((const GLfloat*)&m_Scene->GetCamera()->GetProjection()[0]);
        glUseProgram(0);
        //m_Physics->RenderDiagnostics();
    }
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