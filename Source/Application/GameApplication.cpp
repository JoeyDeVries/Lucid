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
	// TODO(Joey): Not very important, but data-driven development would be a welcome abstraction here while loading levels/entities
    // Load necessary actors/textures/shaders before loading level
	ResourceManager::GetInstance()->LoadShader("sprite", "shaders/sprite.vs", "shaders/sprite.frag");
	ResourceManager::GetInstance()->LoadShader("sprite-anim", "shaders/sprite_animation.vs", "shaders/sprite_animation.frag");
	ResourceManager::GetInstance()->LoadTexture("block", "textures/stone.png");
	ResourceManager::GetInstance()->LoadTexture("block_color", "textures/stone_color.png");
    ResourceManager::GetInstance()->LoadTexture("specular", "textures/specular.png");
    ResourceManager::GetInstance()->LoadTexture("normal", "textures/normal.png");
    ResourceManager::GetInstance()->LoadTexture("block_specular", "textures/stone_specular.png");
    ResourceManager::GetInstance()->LoadTexture("block_normal", "textures/stone_normal.png");
	ResourceManager::GetInstance()->LoadTexture("flag", "textures/flag.png", true);
	ResourceManager::GetInstance()->LoadTexture("flag_normal", "textures/flag_normal.png");
	ResourceManager::GetInstance()->LoadTexture("light", "textures/light.png", true);
	ResourceManager::GetInstance()->LoadTexture("light-anim", "textures/animations/fire-anim2.png", true);
    ResourceManager::GetInstance()->LoadLevel(m_Scene, "levels/begin.lvl");
    // Create level-independant actors
    // - background (no need for it to be related to an actor, only for rendering)
    std::shared_ptr<Actor> backgroundActor = CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_EMPTY);
    std::shared_ptr<BackgroundNode> backgroundNode(new BackgroundNode(backgroundActor->GetID())); // 0 = no_actor id
	std::shared_ptr<Material> backgroundMaterial = std::shared_ptr<Material>(new Material());
	backgroundMaterial->SetShader(ResourceManager::GetInstance()->GetShader("sprite"));
    backgroundMaterial->SetDiffuse(ResourceManager::GetInstance()->LoadTexture("background", "textures/background.png"));
    backgroundMaterial->SetSpecular(ResourceManager::GetInstance()->GetTexture("specular"));
    backgroundMaterial->SetNormal(ResourceManager::GetInstance()->GetTexture("normal"));
    backgroundNode->SetMaterial(backgroundMaterial);
    m_Scene->AddChild(backgroundActor->GetID(), backgroundNode);
 //   // - player
    std::shared_ptr<Actor> actor = CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_PLAYER);
    actor->SetPosition(glm::vec2(150.0, 313.0));
    actor->SetDepth(48);
    actor->SetScale(glm::vec2(45.0));
    std::shared_ptr<SpriteNode> node(new SpriteNode(actor->GetID(), "player", "MAIN", actor->GetPosition(), actor->GetDepth(), actor->GetScale()));
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material());
    material->SetShader(ResourceManager::GetInstance()->GetShader("sprite"));
    material->SetDiffuse(ResourceManager::GetInstance()->LoadTexture("player", "textures/player.png", true));
    material->SetSpecular(ResourceManager::GetInstance()->LoadTexture("player_specular", "textures/player_specular.png"));
    material->SetNormal(ResourceManager::GetInstance()->LoadTexture("player_normal", "textures/player_normal.png"));
    node->SetMaterial(material);
	//// - lantern attached to player
	std::shared_ptr<Actor> lantern = CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_LANTERN);
	lantern->SetPosition(glm::vec2(180.0f, 340.0f));
	lantern->SetDepth(10);
	std::shared_ptr<LightNode> lanternNode(
		new LightNode(lantern->GetID(), "lantern", "LIGHT", lantern->GetPosition(), lantern->GetDepth(), lantern->GetScale(),
			glm::vec3(2.0, 2.0, 2.0), glm::vec3(1.0, 1.0, 1.0), 250.0f)
		);
	// ... we don't need a material/shader as light won't be renderable
	m_Scene->GetLightManager()->AddLight(lanternNode);
	node->AddChild(lanternNode);
	m_Scene->AddChild(lantern->GetID(), lanternNode);
	SetImportantActor("lantern", lantern);

	m_Scene->AddChild(actor->GetID(), node);
	m_Physics->AddCharacter(actor, 1.0f);
	SetImportantActor("player", actor);

	// register for global game event
	EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &GameApplication::OnLevelComplete);
	GetEventManager()->AddListener(eventListener, Event_LevelComplete::s_EventType);

    // initialize scene (e.g. set default projection matrices for each shader)
    m_Scene->Initialize();
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