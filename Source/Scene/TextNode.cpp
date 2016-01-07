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
#include "TextNode.h"

#include "../Application/GameApplication.h"
#include "../Renderer/TextRenderer.h"

TextNode::TextNode(unsigned int ActorID, glm::vec2 position, glm::vec2 scale) 
    : SceneNode(ActorID, "text", "TEXT", position, 0, scale, 0.0f), m_Text(""), m_Centered(false)
{

}

std::string TextNode::GetText()
{
    return m_Text;
}
glm::vec4 TextNode::GetColor()
{
    return m_Color;
}
int TextNode::GetFontScale()
{
    return m_FontScale;
}

void TextNode::SetText(std::string text)
{
    m_Text = text;
}
void TextNode::SetColor(glm::vec4 color)
{
    m_Color = color;
}
void TextNode::SetFontScale(int scale)
{
    m_FontScale = scale;
}

void TextNode::Render(Scene *scene, Renderer *renderer)
{
    GameApplication::GetInstance()->GetTextRenderer()->RenderText(m_Text, m_Position, m_FontScale, true, m_Color, m_Scale);
}