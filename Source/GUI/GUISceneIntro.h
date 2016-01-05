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
#ifndef GUI_SCENE_INTRO_H
#define GUI_SCENE_INTRO_H

#include "GUIContainer.h"

#include <string>

class TextRenderer;
class Texture2D;
class Renderer;

/*
    Displays a brief introduction in text to the user using fade-in and fade-out 
    effects. This allows for briefly presenting the story/atmosphere of the current
    level. After roughly 15 seconds of intro the game starts.
*/
class GUISceneIntro : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture; // background texture of intro scene

    std::string m_IntroText;  // text to display
    float       m_Alpha;      // holds the current alpha value of the container
    float       m_TextAlpha;  // holds the current text alpha value (for separately controlling text fade-out over total control fadeout)
    float       m_TimeActive; // holds the time the scene intro is active
public:
    GUISceneIntro();

    // getters
    std::string GetIntroText();
    // setters
    void SetIntroText(std::string text);

    // initializes the scene intro container and all its relevant controls
    bool Init() override;
    // updates the scene intro container plus all its controls
    void Update(float deltaTime) override;
    // overrides the render background function to render a custom background
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer) override;

    // activates the in-game menu
    void OnActivate() override;
    // de-activates the in-game menu
    void OnDeactivate() override;
};
#endif