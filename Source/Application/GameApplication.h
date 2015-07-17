#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include <list>

#include "../Scene/Scene.h"
#include "../Components/ActorFactory.h"
#include "../Components/Actor.h"

class GameApplication
{
private:
    char m_Keys[1024];
    char m_KeysPressed[1024];

    Scene *m_Scene;
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
    std::shared_ptr<GameApplication> GetInstance()
    {
        if(!m_Instance)
            m_Instance = std::shared_ptr<GameApplication>(new GameApplication);
        return m_Instance;
    }

    void Initialize();
    void Update(float deltaTime);
    void Render();
    void ProcessKeyboardDown(char key);
    void ProcessKeyboardUp(char key);

    Scene* const GetScene() { return m_Scene; }
};


#endif