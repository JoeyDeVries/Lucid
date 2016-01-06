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

//#define GLEW_STATIC
//#include <GL/glew.h>
//#include "shader.h"
//#include "Framebuffer.h"
#include <memory>


class Framebuffer;
class Renderer;
class Shader;

/*
    The different types of post processing effects.
*/
enum POST_PROCESS_EFFECT
{
    POST_PROCESS_NONE,
    POST_PROCESS_GRAYSCALE,
    POST_PROCESS_INVERT,
    POST_PROCESS_BLUR,
    POST_PROCESS_SHAKE,
    POST_PROCESS_FLASH,
};

/*
    Allows several post-processing effects to be applied over the original rendered scene.
*/
class PostProcessor
{
    std::shared_ptr<Framebuffer> m_Framebuffer; // the framebuffer storing the rendered scene 
    std::shared_ptr<Shader>      m_Shader;      // the shader that runs over the rendered scene applying zero or more post-processing effects
public:
    PostProcessor();

    // getters
    std::shared_ptr<Shader> const GetShader();
    
    // initializes the post processor and its required render state
    void Initialize();
    // binds the relevant textures and framebuffer(s)
    void PreRender();
    // stores the rendered data into the framebuffer(s)
    void PostRender();
    // renders a full screen-space quad to render the scene with post-processing effects applied
    void RenderScreenQuad(Renderer* renderer);

    // enables a specific post-processing effect
    void EnableEffect(POST_PROCESS_EFFECT effect);
    // disables a specific post-processing effect
    void DisableEffect(POST_PROCESS_EFFECT effect);
};
#endif