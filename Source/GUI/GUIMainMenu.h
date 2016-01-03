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

//#include "GUIButton.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/shader.h"
#include "../Renderer/Animation.h"

class GUIMainMenu : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture;
    std::shared_ptr<Animation> m_FireAnimation;
public:
    GUIMainMenu();

    bool Init();
    void Update(float deltaTime);
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer); // override background rendering

    void OnActivate();
    void OnDeactivate();

    void ButtonPressed(std::shared_ptr<GUIButton> pButton);
};

#endif