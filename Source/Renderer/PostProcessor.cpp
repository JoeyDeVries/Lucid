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
#include "PostProcesser.h"

#include "Renderer.h"

#include "../Resources/ResourceManager.h"
#include "../Application/GameApplication.h"

PostProcessor::PostProcessor()
{
  
}

void PostProcessor::Initialize()
{
    m_Shader = ResourceManager::GetInstance()->LoadShader("postprocessor", "shaders/post_processor.vs", "shaders/post_processor.frag");
    m_Framebuffer.reset(new Framebuffer(GameApplication::GetInstance()->ScreenWidth(), GameApplication::GetInstance()->ScreenHeight()));
}

std::shared_ptr<Shader> const PostProcessor::GetShader()
{
    return m_Shader;
}

void PostProcessor::PreRender()
{
    m_Framebuffer->BeginRender();
}

void PostProcessor::PostRender()
{
    m_Framebuffer->EndRender();
}

void PostProcessor::RenderScreenQuad(Renderer* renderer)
{
    m_Shader->Use();
    m_Framebuffer->GetColorBuffer()->Bind(0);
    renderer->RenderQuad();
}

void PostProcessor::EnableEffect(POST_PROCESS_EFFECT effect)
{
    switch (effect)
    {
    case POST_PROCESS_EFFECT::POST_PROCESS_BLUR:
        m_Shader->SetInteger("blur", true, true);
        break;
    case POST_PROCESS_EFFECT::POST_PROCESS_GRAYSCALE:
        m_Shader->SetInteger("grayscale", true, true);
        break;
    case POST_PROCESS_EFFECT::POST_PROCESS_INVERT:
        m_Shader->SetInteger("invert", true, true);
        break;
    }
}

void PostProcessor::DisableEffect(POST_PROCESS_EFFECT effect)
{
    switch (effect)
    {
    case POST_PROCESS_EFFECT::POST_PROCESS_BLUR:
        m_Shader->SetInteger("blur", false, true);
        break;
    case POST_PROCESS_EFFECT::POST_PROCESS_GRAYSCALE:
        m_Shader->SetInteger("grayscale", false, true);
        break;
    case POST_PROCESS_EFFECT::POST_PROCESS_INVERT:
        m_Shader->SetInteger("invert", false, true);
        break;
    }
}