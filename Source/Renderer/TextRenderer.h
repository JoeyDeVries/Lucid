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
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <memory>
#include <map>

class Texture2D;
class Shader;
class Font;

/*
    The render data required for rendering a single character vertex
*/
struct CharVertex {
    glm::vec4 Position;
    glm::vec4 Color;
};

/*
    The vertex info required to render a single character
*/
struct CharRenderInfo {
    CharVertex Vertices[6];
};

/*
    Renders text using the signed distance fields algorithm as published by Valve. 
    Text render comments don't instantly render text, but stores the text commands
    in a queue after which at one point during a frame all queued text is rendered
    within a single large buffer. 
    Rendered text is seperated as in-place text that's rendered in the game-world 
    or 'normal' text that's displayed in clip-space device coordinates.
*/
class TextRenderer
{
private:
    std::vector<CharRenderInfo> m_RenderDataInPlace; // text data required for rendering text in world-space coordinates
    std::vector<CharRenderInfo> m_RenderData;        // text data required for rendering text in clip-space coordinates
	std::shared_ptr<Shader>     m_Shader;            // the shader used for rendering signed distance fields
	std::shared_ptr<Font>       m_Font;              // the loaded font to use for rendering text

    unsigned int m_VAO, m_VBO;                             // the OpenGL render state required for rendering a single character
public:
	TextRenderer();
	~TextRenderer();

    // initializes the text renderer and the loaded font
	bool Initialize(std::shared_ptr<Font> font);
    // queues text into the renderData queue for deferred rendering
	void RenderText(std::string text, glm::vec2 position, float userScale = 30.0f, bool inPlace = false, glm::vec4 color = glm::vec4(1.0f), glm::vec2 centerBox = glm::vec2(1.0f));
    // renders all queued text data with a single draw call (one for in-place and one for clip-space)
    void Render(glm::mat4 projection, glm::mat4 view = glm::mat4());
};
#endif