#include "GUIMainMenu.h"
#include <iostream>
#include "../Resources/ResourceManager.h"
#include "../Application/GameApplication.h"
#include "../Application/Event_StartLevel.h"
#include "../Application/Event_QuitGame.h"

GUIMainMenu::GUIMainMenu()
{

}

bool GUIMainMenu::Init()
{
    SetScale(glm::vec2(GameApplication::GetInstance()->ScreenWidth(), GameApplication::GetInstance()->ScreenHeight()));

    // start game
    std::shared_ptr<GUIButton> btnStartGame(new GUIButton);
    btnStartGame->SetName("btnStartGame");
    btnStartGame->SetPosition(glm::vec2(250.0f, 200.0f));
    btnStartGame->SetScale(glm::vec2(300.0f, 50.0f));
    btnStartGame->SetForeColor(glm::vec4(glm::vec3(0.6f), 1.0f));
    btnStartGame->SetHoverColor(glm::vec4(1.0f));
    btnStartGame->SetText("Start Game");
    // load game
    std::shared_ptr<GUIButton> btnContinue(new GUIButton);
    btnContinue->SetName("btnContinue");
    btnContinue->SetPosition(glm::vec2(250.0f, 275.0f));
    btnContinue->SetScale(glm::vec2(300.0f, 50.0f));
    btnContinue->SetForeColor(glm::vec4(glm::vec3(0.6f), 1.0f));
    btnContinue->SetHoverColor(glm::vec4(1.0f));
    btnContinue->SetText("Continue");
    // options
    std::shared_ptr<GUIButton> btnOptions(new GUIButton);
    btnOptions->SetName("btnOptions");
    btnOptions->SetPosition(glm::vec2(250.0f, 340.0f));
    btnOptions->SetScale(glm::vec2(300.0f, 50.0f));
    btnOptions->SetForeColor(glm::vec4(glm::vec3(0.6f), 1.0f));
    btnOptions->SetHoverColor(glm::vec4(1.0f));
    btnOptions->SetText("Options");
    // quit
    std::shared_ptr<GUIButton> btnQuit(new GUIButton);
    btnQuit->SetName("btnQuit");
    btnQuit->SetPosition(glm::vec2(250.0f, 415.0f));
    btnQuit->SetScale(glm::vec2(300.0f, 50.0f));
    btnQuit->SetForeColor(glm::vec4(glm::vec3(0.6f), 1.0f));
    btnQuit->SetHoverColor(glm::vec4(1.0f));
    btnQuit->SetText("Quit Game");

    // add elements to container control
    AddElement(btnStartGame);
    AddElement(btnContinue);
    AddElement(btnOptions);
    AddElement(btnQuit);

    // load additional resources
    m_BackgroundTexture = ResourceManager::GetInstance()->LoadTexture("menu_background", "textures/menu_background.png");
    m_FireAnimation = ResourceManager::GetInstance()->LoadAnimation("textures/animations/fire-anim.anim");
    ResourceManager::GetInstance()->LoadTexture("light-anim", "textures/animations/fire-anim.png", true);
    return true;
}

void GUIMainMenu::Update(float deltaTime)
{
    m_FireAnimation->Update(deltaTime);
}

void GUIMainMenu::RenderBackground(Renderer *renderer, TextRenderer *textRenderer)
{
    // render title
    //textRenderer->RenderText("Lantarn", glm::vec2(300.0f, 50.0f), 7.0f, false, glm::vec4(1.0, 1.0, 1.0, 1.0f));
    textRenderer->RenderText("Lantarn", glm::vec2(0.0f, 0.0f), 7.0f, false, glm::vec4(1.0, 1.0, 1.0, 1.0f), glm::vec2(m_Scale.x, 100.0f));

    // render background image + three animated sprites
    std::shared_ptr<Shader> spriteShader = ResourceManager::GetInstance()->GetShader("sprite");
    spriteShader->Use();
    spriteShader->SetMatrix4("projection", GameApplication::GetInstance()->GetScene()->GetCamera()->GetProjection());
    spriteShader->SetMatrix4("view", glm::mat4());
    spriteShader->SetInteger("EnableLighting", 0);
    spriteShader->SetVector4f("ColorOverride", glm::vec4(1.0f));
    // 1. first render background
    glm::mat4 model;
    model *= glm::scale(glm::vec3(800.0f, 600.0f, 1.0f));
    spriteShader->SetMatrix4("model", model);
    m_BackgroundTexture->Bind(0);
    renderer->RenderQuad();

    // 2. then individual fire animations
    spriteShader->SetInteger("animation", 1);
    m_FireAnimation->ToShader(spriteShader);
    ResourceManager::GetInstance()->GetTexture("light-anim")->Bind(0);
    // - light 1
    model = glm::mat4();
    model *= glm::translate(glm::vec3(60.0f, 100.0f, 0.0f));
    model *= glm::scale(glm::vec3(110.0f));
    spriteShader->SetMatrix4("model", model);
    renderer->RenderQuad();
    // - light 2
    model = glm::mat4();
    model *= glm::translate(glm::vec3(340.0f, 305.0f, 0.0f));
    model *= glm::scale(glm::vec3(50.0f));
    spriteShader->SetMatrix4("model", model);
    renderer->RenderQuad();
    // - light 3
    model = glm::mat4();
    model *= glm::translate(glm::vec3(570.0f, 275.0f, 0.0f));
    model *= glm::scale(glm::vec3(50.0f));
    spriteShader->SetMatrix4("model", model);
    renderer->RenderQuad();


    spriteShader->SetInteger("animation", 0);

    // reset back to default
    spriteShader->SetInteger("EnableLighting", 1);
}

void GUIMainMenu::OnActivate()
{
    GameApplication::GetInstance()->GetAudio()->StopAll();
    GameApplication::GetInstance()->GetAudio()->PlaySound("audio/menu.mp3", true);
}

void GUIMainMenu::OnDeactivate()
{
    GameApplication::GetInstance()->GetAudio()->StopSound("audio/menu.mp3");
}

void GUIMainMenu::ButtonPressed(std::shared_ptr<GUIButton> pButton)
{
    std::string name = pButton->GetName();
    if (name == "btnStartGame")
    {   // send out start game event
        std::shared_ptr<IEventData> pEvent(new Event_StartLevel("levels/start.tmx"));
        GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
        std::cout << "Button: StartGame event sent. " << std::endl;
    }
    else if (name == "btnContinue")
    {   // send out load game event
        std::shared_ptr<IEventData> pEvent(new Event_StartLevel("levels/start.tmx"));
        GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
        std::cout << "Button: Continue event sent. " << std::endl;
    }
    else if (name == "btnOptions")
    {   // send out load game event
        std::cout << "Button: Options pressed. " << std::endl;
    }
    else if(name == "btnQuit")
    {   // send out game quit event
        std::shared_ptr<IEventData> pEvent(new Event_QuitGame());
        GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
        std::cout << "Button: Quit event sent. " << std::endl;
    }
}