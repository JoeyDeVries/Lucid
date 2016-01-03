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
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "shader.h"
#include "texture2D.h"
#include <glm/glm.hpp>
#include "../Resources/Font.h"

struct CharVertex {
    glm::vec4 Position;
    glm::vec4 Color;
};

struct CharRenderInfo {
    CharVertex Vertices[6];
};


class TextRenderer
{
private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Font> m_Font;
    std::vector<CharRenderInfo> m_RenderData;
    std::vector<CharRenderInfo> m_RenderDataInPlace;

    GLuint m_VAO, m_VBO;
public:
	TextRenderer();
	~TextRenderer();

	bool Initialize(std::shared_ptr<Font> font);
    // Queues text into the renderData queue for later rendering
	void RenderText(std::string text, glm::vec2 position, float userScale = 30.0f, bool inPlace = false, glm::vec4 color = glm::vec4(1.0f), glm::vec2 centerBox = glm::vec2(1.0f));
    // Renders all queued text data with a single draw call
    void Render(glm::mat4 projection, glm::mat4 view = glm::mat4());
};

#endif