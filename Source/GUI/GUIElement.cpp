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
#include "GUIElement.h"

#include "../Application/GameApplication.h"

#include <iostream>

GUIElement::GUIElement() : m_MouseEntered(false), m_ForeColor(1.0f), m_BackColor(1.0f)
{

}

std::string GUIElement::GetName()
{
    return m_Name;
}
glm::vec2 GUIElement::GetPosition()
{
    return m_Position;
}
glm::vec2 GUIElement::GetScale()
{
    return m_Scale;
}
glm::vec4 GUIElement::GetForeColor()
{
    return m_ForeColor;
}
glm::vec4 GUIElement::GetBackColor()
{
    return m_BackColor;
}
bool GUIElement::GetMouseEntered()
{
    return m_MouseEntered;
}

void GUIElement::SetName(std::string name)
{
    m_Name = name;
}
void GUIElement::SetPosition(glm::vec2 position)
{
    m_Position = position;
    CalculateModel();
}
void GUIElement::SetScale(glm::vec2 scale)
{
    m_Scale = scale;
    CalculateModel();
}
void GUIElement::SetForeColor(glm::vec4 foreColor)
{
    m_ForeColor = foreColor;
}
void GUIElement::SetBackColor(glm::vec4 backColor)
{
    m_BackColor = backColor;
}
void GUIElement::SetMouseEntered(bool entered)
{
    m_MouseEntered = entered;
}

bool GUIElement::IsMouseInside(float x, float y)
{
    bool inside = true;
    glm::vec2 AABBCenter = m_Position + m_Scale * 0.5f;
    glm::vec2 AABBHalfWidths = m_Scale * 0.3f;
    // - check if target is outside AABB and if so change center accordingly 
    if (x > AABBCenter.x + AABBHalfWidths.x)      // target at right side of box
        inside = false;
    else if (x < AABBCenter.x - AABBHalfWidths.x) // target at left side of box
        inside = false;
    if (y > AABBCenter.y + AABBHalfWidths.y)      // target at bottom side of box
        inside = false;
    else if (y < AABBCenter.y - AABBHalfWidths.y) // target at top side of box
        inside = false;
    return inside;
}

// empty, free to override in subclass
void GUIElement::OnKeyPressed(char key) { }
void GUIElement::OnKeyReleased(char key) { }
void GUIElement::OnMouseEnter() { }
void GUIElement::OnMouseLeave() { }
void GUIElement::OnMouseClick() { }