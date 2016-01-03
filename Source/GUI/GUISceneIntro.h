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

class GUISceneIntro : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture;
    std::string m_IntroText;
    float m_Alpha;
    float m_TextAlpha;
    float m_TimeActive;
public:
    GUISceneIntro();

    std::string GetIntroText();
    void SetIntroText(std::string text);

    bool Init() override;
    void Update(float deltaTime) override;
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer) override; 

    void OnActivate() override;
    void OnDeactivate() override;
};

#endif