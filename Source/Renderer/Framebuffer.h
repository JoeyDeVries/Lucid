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
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

//#define GLEW_STATIC
//#include <GL/glew.h>
//#include "texture2D.h"

#include <memory>

class Texture2D;

/*
    A conventient class for managing framebuffer objects.
*/
class Framebuffer
{
    std::shared_ptr<Texture2D> m_ColorBuffer; // holds the color data of the framebuffer
    std::shared_ptr<Texture2D> m_DepthBuffer; // holds the depth information of the framebuffer

    unsigned int m_Width, m_Height; // the width and height of the framebuffer
    unsigned int m_FBO, m_RBO;      // the underlying OpenGL object IDs of the framebuffer and underlying render buffer object
public:
    Framebuffer(unsigned int width, unsigned int height);

    // getters
    std::shared_ptr<Texture2D> const GetColorBuffer();
    std::shared_ptr<Texture2D> const GetDepthBuffer();

    // update the texture used as the color buffer
    void UpdateColorBufferTexture(std::shared_ptr<Texture2D> texture);

    // binds the current framebuffer (without clearing)
    void Bind();
    // bind to framebuffer and clear buffers 
    void BeginRender();
    // unbind framebuffer to store data in buffers
    void EndRender();
};
#endif