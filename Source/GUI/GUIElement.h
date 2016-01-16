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
#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <glm/glm.hpp>

#include <memory>
#include <string>

class Renderer;
class TextRenderer;
class Shader;

/*
    Provides the base of any GUI element. Defines the relevant state shared
    by each GUI element and defines the template functions. GUI elements 
    should be contained within a GUI container.
*/
class GUIElement
{
protected:
    std::string m_Name;         // the name of the GUI element to identify it from other controls
    glm::vec2   m_Position;     // the top-left position of the GUI element
    glm::vec2   m_Scale;        // the scale of the element
    glm::vec4   m_ForeColor;    // the element's foreground/text color
    glm::vec4   m_BackColor;    // the element's background color
    glm::mat4   m_Model;        // the world-space model matrix for rendering the GUI element
    bool        m_MouseEntered; // is set to true if the mouse position is currently inside the GUI element
public:
    GUIElement();

    // getters
    std::string GetName();
    glm::vec2   GetPosition();
    glm::vec2   GetScale();
    glm::vec4   GetForeColor();
    glm::vec4   GetBackColor();
    bool        GetMouseEntered();
    // setters
    void SetName(std::string name);
    void SetPosition(glm::vec2 position);
    void SetScale(glm::vec2 scale);
    void SetForeColor(glm::vec4 foreColor);
    void SetBackColor(glm::vec4 backColor);
    void SetMouseEntered(bool entered);

    // re-calculates model matrix
    virtual void CalculateModel() = 0;

    // renders the GUIElement, should be overriden in subclasses
    virtual void Render(Renderer *renderer, TextRenderer *textRenderer, std::shared_ptr<Shader> shader) = 0;

    bool IsMouseInside(float x, float y);

    // NOTE(Joey): for GUI elements we don't use the default EventQueue system
    //             as this will incur too much overhead of adding/removing of
    //             events while most GUI elements are inactive most of the time
    // fires when a (relevant) key is pressed
    virtual void OnKeyPressed(char key);
    // fires when a (relevant) key is released
    virtual void OnKeyReleased(char key);
    // fires when the mouse enters this control's dimensions
    virtual void OnMouseEnter();
    // fires when the mouse leaves this control's dimensions
    virtual void OnMouseLeave();
    // fires when the (left) mouse button is clcked
    virtual void OnMouseClick();
};
#endif