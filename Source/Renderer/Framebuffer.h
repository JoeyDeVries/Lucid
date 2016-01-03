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

#define GLEW_STATIC
#include <GL/glew.h>
#include "texture2D.h"
#include <memory>

class Framebuffer
{
    GLuint m_FBO, m_RBO;
    GLuint m_Width, m_Height;
    std::shared_ptr<Texture2D> m_ColorBuffer;
    std::shared_ptr<Texture2D> m_DepthBuffer;
public:
    // Constructor
    Framebuffer(GLuint width, GLuint height);
    // Getters
    std::shared_ptr<Texture2D> const GetColorBuffer();
    std::shared_ptr<Texture2D> const GetDepthBuffer();
    // Update the texture used as the color buffer
    void UpdateColorBufferTexture(std::shared_ptr<Texture2D> texture);
    // Binds the current framebuffer (without clearing)
    void Bind();
    // Bind to framebuffer and clear buffers 
    void BeginRender();
    // Unbind framebuffer to store data in buffers
    void EndRender();
};

#endif