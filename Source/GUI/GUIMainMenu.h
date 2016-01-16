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
#ifndef GUI_MAIN_MENU_H
#define GUI_MAIN_MENU_H

#include "GUIContainer.h"

#include <memory>

class TextRenderer;
class Renderer;
class Texture2D;
class Shader;
class Animation;
class GUIButton;

/*
    The GUI container instance of the start main menu.
*/
class GUIMainMenu : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture; // texture used for rendering the background
public:
    GUIMainMenu();

    // initializes the main menu container and all its relevant controls
    bool Init();
    // updates the main menu plus all its controls
    void Update(float deltaTime);
    // overrides the render background function to render a custom background
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer); // override background rendering

    // activates the main menu
    void OnActivate();
    // de-activates the main menu
    void OnDeactivate();

    // occurs when a button is pressed; handle accordingly
    void ButtonPressed(std::shared_ptr<GUIButton> pButton);
};
#endif