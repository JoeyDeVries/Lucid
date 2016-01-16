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

#include <glm/glm.hpp>

#include "SceneNode.h"

class Scene;
class Renderer;

/*
    A scene node dedicated to display text. Can be attached to the scene graph as to display
    text in the world or attached to other scene nodes (for textual display at top of monsters
    for instance; displaying their name).
*/
class TextNode : public SceneNode
{
private:
    std::string m_Text;      // the text to render
    glm::vec4   m_Color;     // at what color to render the text
    int         m_FontScale; // the scale of the font
    bool        m_Centered;  // whether the text is centered within the node's dimensions
public:
    TextNode(unsigned int ActorID, glm::vec2 position, glm::vec2 scale = glm::vec2(1.0f));

    // getters
    std::string GetText();
    glm::vec4   GetColor();
    int         GetFontScale();
    // setters
    void SetText(std::string text);
    void SetColor(glm::vec4 color);
    void SetFontScale(int scale);

    // renders the text node, delegating the render commands to the text renderer
    void Render(Scene *scene, Renderer *renderer) override;
};
#endif