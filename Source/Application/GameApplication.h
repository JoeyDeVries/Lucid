#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include "../Scene/Scene.h"

class GameApplication
{
private:
    char m_Keys[1024];
    char m_KeysPressed[1024];

    std::shared_ptr<Scene> m_Scene;

    // TODO
    virtual void checkOtherInstances();
    virtual void checkStorage();
    virtual void checkRAM();
public:
     // TODO
    GameApplication();
    void Initialize();
    void ProcessKeyboardInput();

    std::shared_ptr<Scene> GetScene() { return m_Scene; }
};


#endif