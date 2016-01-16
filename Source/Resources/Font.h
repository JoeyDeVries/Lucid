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
#ifndef FONT_H
#define FONT_H

#include <glm/glm.hpp>

#include <memory>
#include <map>

class Texture2D;

/*
    The font's character info required for positioning and rendering a single character.
*/
struct CharInfo {
    glm::vec2 Position;
    glm::vec2 Scale;
    glm::vec2 Offset;
    float XAdvance;
    glm::vec4 RenderInfo[6]; // <vec2 position, vec2 texCoords>
};

/*
    Holds a list of character info together with the font's character spritesheet.
*/
class Font
{
private:
    std::shared_ptr<Texture2D> m_FontAtlas; // the spritesheet holding the signed distance fields of each character
    std::map<char, CharInfo>   m_CharInfo;  // holds per character its positioning and render data

    int m_Width, m_Height; // the width and height of the character spritesheet
    int m_CharSize;        // the size of a single character
public:
    Font();

    // getters
    const std::map<char, CharInfo>&       GetCharInfo();
    std::shared_ptr<Texture2D>            GetFontAtlas();
    int                                   GetWidth();
    int                                   GetHeight();
    int                                   GetCharSize();

    // loads and configures the font data
    void Load(std::shared_ptr<Texture2D> fontAtlas, std::map<char, CharInfo> charInfo, int charSize, int width, int height);
};
#endif