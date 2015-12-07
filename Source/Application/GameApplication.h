#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include <vector>
#include <map>

#include "../Scene/Scene.h"
#include "../Components/ActorFactory.h"
#include "../Components/Actor.h"
#include "../Communication/EventManager.h"
#include "../Physics/Box2DPhysics.h"
#include "../Audio/AudioEngine.h"

class GameApplication
{
private:
    float m_ScreenWidth, m_ScreenHeight;

    char m_Keys[1024];
    char m_KeysPressed[1024];

    // Game-specifics
    Scene        *m_Scene;
    EventManager *m_EventManager;
    ActorFactory *m_ActorFactory;
    Box2DPhysics *m_Physics;
	AudioEngine  *m_Audio;
    std::map<ActorID, std::shared_ptr<Actor>> m_Actors;
	std::map<std::string, std::shared_ptr<Actor>> m_ImportantActors;

    // TODO
    virtual void checkOtherInstances();
    virtual void checkStorage();
    virtual void checkRAM();

    // Singleton pattern, constructor private
    static std::shared_ptr<GameApplication> m_Instance;
    GameApplication();
public:
    // Retrieves a single instance of this object
    static std::shared_ptr<GameApplication> GetInstance()
    {
        if(!m_Instance)
            m_Instance = std::shared_ptr<GameApplication>(new GameApplication);
        return m_Instance;
    }
    ~GameApplication();

    void Initialize(float width, float height);
    std::shared_ptr<Actor> CreateActor(DEFAULT_ACTOR_TYPES type);
    std::shared_ptr<Actor> GetActor(ActorID actorID);
	void				   SetImportantActor(std::string, std::shared_ptr<Actor> actor);
	std::shared_ptr<Actor> GetImportantActor(std::string);

    void Update(float deltaTime);
    void Render();
    void ProcessKeyboardDown(char key);
    void ProcessKeyboardUp(char key);
    bool IsKeyPressed(char key, bool check_once = false);

    float& ScreenWidth() { return m_ScreenWidth; }
    float& ScreenHeight() { return m_ScreenHeight; }
    Scene* const GetScene() { return m_Scene; }
    EventManager* const GetEventManager() { return m_EventManager; }
    Box2DPhysics* const GetPhysics() { return m_Physics; }

	float const GetTime() { return glfwGetTime(); }

	// global game event
	void OnLevelComplete(std::shared_ptr<IEventData> eventData);
};


#endif