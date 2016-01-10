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
#ifndef GUI_CONTAINER_H
#define GUI_CONTAINER_H

#include "GUIElement.h"
#include "GUIButton.h"

#include <vector>

class TextRenderer;
class Renderer;
class Shader;
class Camera;

/*
    A container used for rendering and containing GUI elements. A GUI container is
    required to render GUI components; each GUI element requires a parent GUI 
    container. The GUI container can store any number of GUI elements and process
    their actions.
*/
class GUIContainer
{
private:
    std::vector<std::shared_ptr<GUIElement>> m_Elements; // a list of GUI elements the container contains
protected:
    // add a GUI element to the container
    void AddElement(std::shared_ptr<GUIElement> element);  
    // remove a GUI element from the container
    void RemoveElement(std::shared_ptr<GUIElement> element);

    bool      m_IsActive; // is the container currently active (being rendered, responding to user input)
    glm::vec2 m_Position; // sets the top-left position of the container 
    glm::vec2 m_Scale;    // defines the scale of the container
public:
    GUIContainer();

    // getters
    bool IsActive();
    glm::vec2 GetPosition();
    glm::vec2 GetScale();
    // setters
    void SetActive(bool active);
    void SetPosition(glm::vec2 position);
    void SetScale(glm::vec2 scale);

    // initializes the GUI container; this should generate all GUI elements corresponding to specific GUI
    // overlays; this can either be hard-coded or generated from file.
    virtual bool Init() = 0;
    // updates the container and all GUI elements
    virtual void Update(float deltaTime) = 0;
    // renders the container and all GUI elements
    virtual void Render(Renderer *renderer, TextRenderer *textRenderer, std::shared_ptr<Camera> camera);
    // template method pattern: allow GUI systems to override RenderBackground
    virtual void RenderBackground(Renderer *renderer, TextRenderer *textRenderer);

    // sets the current GUI container as the active GUI container
    virtual void OnActivate();
    // deactivates the GUI container 
    virtual void OnDeactivate();

    // occurs when a keyboard key is pressed
    void OnKeyPressed(char key);
    // occurs when a keyboard key is released
    void OnKeyReleased(char key);
    // occurs when the mouse has changed position
    void OnMouseMove(float x, float y);
    // occurs when the mouse button is clicked
    void OnMouseClick(bool leftButton);

    // is called whenever any of its GUIButton elements are pressed.
    virtual void ButtonPressed(std::shared_ptr<GUIButton> pButton);
};
#endif