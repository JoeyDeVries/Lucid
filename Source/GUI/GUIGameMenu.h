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
#ifndef GUI_GAME_MENU_H
#define GUI_GAME_MENU_H

#include "GUIContainer.h"

#include <memory>

class TextRenderer;
class Renderer;
class GUIButton;
class Texture2D;

/*
    The GUI container instance of the in-game menu.
*/
class GUIGameMenu : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture; // background texture of the menu
public:
    GUIGameMenu();

    // initializes the in-game menu container and all its relevant controls
    bool Init() override;
    // updates the in-game menu plus all its controls
    void Update(float deltaTime) override;
    // overrides the render background function to render a custom background
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer) override; // override background rendering

    // activates the in-game menu
    void OnActivate() override;
    // de-activates the in-game menu
    void OnDeactivate() override;

    // occurs when a button is pressed; handle accordingly
    void ButtonPressed(std::shared_ptr<GUIButton> pButton) override;
};
#endif