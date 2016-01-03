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
#ifndef POST_PROCESSER_H
#define POST_PROCESSER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "shader.h"
#include "Framebuffer.h"
#include <memory>

class Renderer;

enum POST_PROCESS_EFFECT
{
    POST_PROCESS_NONE,
    POST_PROCESS_GRAYSCALE,
    POST_PROCESS_INVERT,
    POST_PROCESS_BLUR,
    POST_PROCESS_SHAKE,
    POST_PROCESS_FLASH,
};

class PostProcessor
{
    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<Framebuffer> m_Framebuffer;
public:
    PostProcessor();

    std::shared_ptr<Shader> const GetShader();
    
    void Initialize();
    void PreRender();
    void PostRender();
    void RenderScreenQuad(Renderer* renderer);

    void EnableEffect(POST_PROCESS_EFFECT effect);
    void DisableEffect(POST_PROCESS_EFFECT effect);
};


#endif