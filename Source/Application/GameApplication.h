#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include <vector>
#include <map>

#include "GameState.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Scene.h"
#include "../Components/ActorFactory.h"
#include "../Components/Actor.h"
#include "../Communication/EventManager.h"
#include "../Physics/Box2DPhysics.h"
#include "../Audio/AudioEngine.h"
#include "../GUI/GUIContainer.h"

class GameApplication
{
private:
    bool m_Active;
    float m_ScreenWidth, m_ScreenHeight;

    char m_Keys[1024];
    char m_KeysPressed[1024];

    GameState m_GameState;

    // Game-specifics
    Scene        *m_Scene;
    Renderer     *m_Renderer;
    EventManager *m_EventManager;
    ActorFactory *m_ActorFactory;
    Box2DPhysics *m_Physics;
    AudioEngine  *m_Audio;
    TextRenderer *m_TextRenderer;

    std::map<ActorID, std::shared_ptr<Actor>>     m_Actors;
    std::map<std::string, std::shared_ptr<Actor>> m_ImportantActors;

    // GUI
    std::map<std::string, std::shared_ptr<GUIContainer>> m_GUIContainers;

    // State
    void switchState(GameState state = GameState::GAME_NONE);

    // TODO(Joey): default OS initialization requirements
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
        if (!m_Instance)
            m_Instance = std::shared_ptr<GameApplication>(new GameApplication);
        return m_Instance;
    }
    ~GameApplication();

    void Initialize(float width, float height);
    void CleanUp();
    std::shared_ptr<Actor> CreateActor(DEFAULT_ACTOR_TYPES type);
    std::shared_ptr<Actor> GetActor(ActorID actorID);
    void				   SetImportantActor(std::string, std::shared_ptr<Actor> actor);
    std::shared_ptr<Actor> GetImportantActor(std::string);
    void                   ClearActors();

    void Update(float deltaTime);
    void Render();
    void ProcessKeyboardDown(char key);
    void ProcessKeyboardUp(char key);
    void ProcessMouseMove(float x, float y);
    void ProcessMouseClick(bool leftButton);
    bool IsKeyPressed(char key, bool check_once = false);


    bool GetActive() { return m_Active; }
    float& ScreenWidth() { return m_ScreenWidth; }
    float& ScreenHeight() { return m_ScreenHeight; }
    Scene* const GetScene() { return m_Scene; }
    Renderer* const GetRenderer() { return m_Renderer; }
    EventManager* const GetEventManager() { return m_EventManager; }
    Box2DPhysics* const GetPhysics() { return m_Physics; }
	TextRenderer* const GetTextRenderer() { return m_TextRenderer; }
    AudioEngine*        const GetAudio() { return m_Audio; }

	float const GetTime() { return glfwGetTime(); }

	// events
	void OnLevelComplete(std::shared_ptr<IEventData> eventData);
    void OnStartLevel(std::shared_ptr<IEventData> eventData);
    void OnQuitGame(std::shared_ptr<IEventData> eventData);
};


#endif