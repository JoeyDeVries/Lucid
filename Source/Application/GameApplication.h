#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include <list>

#include "../Scene/Scene.h"
#include "../Components/ActorFactory.h"
#include "../Components/Actor.h"
#include "../Communication/EventManager.h"

class GameApplication
{
private:
    float m_ScreenWidth, m_ScreenHeight;

    char m_Keys[1024];
    char m_KeysPressed[1024];

    // Game-specifics
    Scene *m_Scene;
    EventManager *m_EventManager;
    ActorFactory *m_ActorFactory;
    std::list<std::shared_ptr<Actor>> m_Actors;

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
    void Update(float deltaTime);
    void Render();
    void ProcessKeyboardDown(char key);
    void ProcessKeyboardUp(char key);
    bool IsKeyPressed(char key, bool check_once = false);

    float& ScreenWidth() { return m_ScreenWidth; }
    float& ScreenHeight() { return m_ScreenHeight; }
    Scene* const GetScene() { return m_Scene; }
    EventManager* const GetEventManager() { return m_EventManager; }
};


#endif