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
#ifndef TEXT_NODE_H
#define TEXT_NODE_H
#include "SceneNode.h"
#include <glm/glm.hpp>

class Scene;
class Renderer;

class TextNode : public SceneNode
{
private:
    std::string m_Text;
    glm::vec4   m_Color;
    int         m_FontScale;
    bool        m_Centered;
public:
    TextNode(unsigned int ActorID, glm::vec2 position, glm::vec2 scale = glm::vec2(1.0f));

    std::string GetText();
    glm::vec4   GetColor();
    int         GetFontScale();

    void SetText(std::string text);
    void SetColor(glm::vec4 color);
    void SetFontScale(int scale);


    void Render(Scene *scene, Renderer *renderer) override;
};



#endif