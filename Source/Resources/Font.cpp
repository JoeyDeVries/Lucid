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
#include "Font.h"

#include "../Renderer/texture2D.h"

Font::Font() : m_Width(0), m_Height(0)
{

}

void Font::Load(std::shared_ptr<Texture2D> fontAtlas, std::map<char, CharInfo> charInfo, int charSize, int width, int height)
{
	m_FontAtlas = fontAtlas;
	m_CharInfo = charInfo;
	m_Width = width;
	m_Height = height;
    m_CharSize = charSize;

	// use char info to generate (normalized) quad render data per char
	for(auto it = m_CharInfo.begin(); it != m_CharInfo.end(); ++it)
	{
		// position data
		glm::vec2 pos = glm::vec2(it->second.Offset.x / m_Width, it->second.Offset.y / m_Height); // origin: top-left
		glm::vec2 width = glm::vec2(it->second.Scale.x / m_Width, it->second.Scale.y / m_Height);
		glm::vec2 texCoords = glm::vec2(it->second.Position.x / m_Width, it->second.Position.y / m_Height);
		// define char render vertices
		it->second.RenderInfo[0] = glm::vec4(pos + glm::vec2(0.0, width.y), texCoords + glm::vec2(0.0, width.y)); // Bottom left
        it->second.RenderInfo[1] = glm::vec4(pos + glm::vec2(width.x, 0.0), texCoords + glm::vec2(width.x, 0.0)); // Top right
        it->second.RenderInfo[2] = glm::vec4(pos,                           texCoords);                           // Top left

        it->second.RenderInfo[3] = glm::vec4(pos + glm::vec2(0.0, width.y), texCoords + glm::vec2(0.0, width.y)); // Bottom left
        it->second.RenderInfo[4] = glm::vec4(pos + width,                   texCoords + width);                   // Bottom right
        it->second.RenderInfo[5] = glm::vec4(pos + glm::vec2(width.x, 0.0), texCoords + glm::vec2(width.x, 0.0)); // Top right
	}
}

const std::map<char, CharInfo>& Font::GetCharInfo()
{
	return m_CharInfo;
}

std::shared_ptr<Texture2D> Font::GetFontAtlas()
{
    return m_FontAtlas;
}

int Font::GetWidth()
{
    return m_Width;
}

int Font::GetHeight()
{
    return m_Height;
}

int Font::GetCharSize()
{
    return m_CharSize;
}