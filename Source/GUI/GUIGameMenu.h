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

class GUIGameMenu : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture;
public:
    GUIGameMenu();

    bool Init() override;
    void Update(float deltaTime) override;
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer) override; // override background rendering

    void OnActivate() override;
    void OnDeactivate() override;

    void ButtonPressed(std::shared_ptr<GUIButton> pButton) override;
};


#endif