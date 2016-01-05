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
#include "GUIMainMenu.h"

#include "GUIButton.h"

#include "../Resources/ResourceManager.h"
#include "../Application/GameApplication.h"
#include "../Application/Event_StartLevel.h"
#include "../Application/Event_QuitGame.h"
#include "../Renderer/TextRenderer.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/shader.h"
#include "../Renderer/Animation.h"
#include "../Audio/AudioEngine.h"
#include "../Communication/EventManager.h"

#include <fstream>

GUIMainMenu::GUIMainMenu()
{

}

bool GUIMainMenu::Init()
{
    // full screen
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
    // quit
    std::shared_ptr<GUIButton> btnQuit(new GUIButton);
    btnQuit->SetName("btnQuit");
    btnQuit->SetPosition(glm::vec2(250.0f, 350.0f));
    btnQuit->SetScale(glm::vec2(300.0f, 50.0f));
    btnQuit->SetForeColor(glm::vec4(glm::vec3(0.6f), 1.0f));
    btnQuit->SetHoverColor(glm::vec4(1.0f));
    btnQuit->SetText("Quit Game");

    // add elements to container control
    AddElement(btnStartGame);
    AddElement(btnContinue);
    AddElement(btnQuit);

    // load additional resources
    m_BackgroundTexture = ResourceManager::GetInstance()->LoadTexture("menu_background", "textures/menu_background.png");
    return true;
}

void GUIMainMenu::Update(float deltaTime)
{

}

void GUIMainMenu::RenderBackground(Renderer *renderer, TextRenderer *textRenderer)
{
    // render title
    textRenderer->RenderText("Lucid", glm::vec2(0.0f, 0.0f), 7.0f, false, glm::vec4(1.0, 1.0, 1.0, 1.0f), glm::vec2(m_Scale.x, 100.0f));

    // render background image + three animated sprites
    std::shared_ptr<Shader> spriteShader = ResourceManager::GetInstance()->GetShader("sprite");
    spriteShader->Use();
    spriteShader->SetMatrix4("projection", GameApplication::GetInstance()->GetScene()->GetCamera()->GetProjection());
    spriteShader->SetMatrix4("view", glm::mat4());
    spriteShader->SetInteger("EnableLighting", 0);
    spriteShader->SetVector4f("ColorOverride", glm::vec4(1.0f));
    // render background
    glm::mat4 model;
    model *= glm::scale(glm::vec3(800.0f, 600.0f, 1.0f));
    spriteShader->SetMatrix4("model", model);
    m_BackgroundTexture->Bind(0);
    renderer->RenderQuad();

    spriteShader->SetInteger("animation", 0);

    // reset back to default
    spriteShader->SetInteger("EnableLighting", 1);
}

void GUIMainMenu::OnActivate()
{
    GameApplication::GetInstance()->GetAudio()->StopAll();
    GameApplication::GetInstance()->GetAudio()->PlaySound("audio/ambient.wav", true);
}

void GUIMainMenu::OnDeactivate()
{
    GameApplication::GetInstance()->GetAudio()->StopSound("audio/ambient.wav");
}

void GUIMainMenu::ButtonPressed(std::shared_ptr<GUIButton> pButton)
{
    std::string name = pButton->GetName();
    if (name == "btnStartGame")
    {   // send out start game event
        std::shared_ptr<IEventData> pEvent(new Event_StartLevel("levels/tutorial.tmx"));
        GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
    }
    else if (name == "btnContinue")
    {   // send out load game event (with level loaded from savefile)
        std::ifstream file("save.dat");
        std::string level;
        std::getline(file, level);
        if (level != "")
        {
            std::shared_ptr<IEventData> pEvent(new Event_StartLevel(level));
            GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
        }
        else
        {
            std::shared_ptr<IEventData> pEvent(new Event_StartLevel("levels/tutorial.tmx"));
            GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
        }
        file.close();
        SetActive(false);
    }
    else if(name == "btnQuit")
    {   // send out game quit event
        std::shared_ptr<IEventData> pEvent(new Event_QuitGame());
        GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
    }
}