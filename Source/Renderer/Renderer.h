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
#ifndef RENDERER_H
#define RENDERER_H
#include "../Scene/SceneNode.h"
#include "PostProcesser.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer
{
private:
    PostProcessor* m_PostProcessor;

    GLuint m_VAO, m_VBO;
    void configureQuad();
public:
    Renderer();
    ~Renderer();
    virtual void Initialize();
    virtual void PreRender();
    virtual void RenderQuad();

    virtual void PrePostProcessRender();
    virtual void PostPostProcessRender();
    virtual void PostProcessQuadRender();

    virtual void EnableEffect(POST_PROCESS_EFFECT effect);
    virtual void DisableEffect(POST_PROCESS_EFFECT effect);
};

#endif