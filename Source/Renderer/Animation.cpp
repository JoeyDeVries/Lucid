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
#include "Animation.h"

#include "texture2D.h"
#include "shader.h"

Animation::Animation(std::vector<AnimationData> &data) : m_Duration(1.0f)
{
    m_AnimationData = data;
}

std::string Animation::GetName()
{
    return m_Name;
}
std::shared_ptr<Texture2D> Animation::GetDiffuse()
{
    return m_Diffuse;
}
std::shared_ptr<Texture2D> Animation::GetSpecular()
{
    return m_Specular;
}
std::shared_ptr<Texture2D> Animation::GetNormal()
{
    return m_Normal;
}

void Animation::SetName(std::string name)
{
    m_Name = name;
}
void Animation::SetDuration(float duration)
{
    m_Duration = duration;
}
void Animation::SetDiffuse(std::shared_ptr<Texture2D> diffuse)
{
    m_Diffuse = diffuse;
}
void Animation::SetSpecular(std::shared_ptr<Texture2D> specular)
{
    m_Specular = specular;
}
void Animation::SetNormal(std::shared_ptr<Texture2D> normal)
{
    m_Normal = normal;
}

void Animation::Init(std::shared_ptr<Shader> shader)
{
    // set m_CurrentTime to random position s.t. similar animations seem more natural
    float randomOffset = (rand() % 100) / 100.0f;
    m_CurrentTime = randomOffset;
}

void Animation::Normalize(int width, int height)
{
    for (auto it = m_AnimationData.begin(); it != m_AnimationData.end(); ++it)
    {
        it->XOffset = it->XOffset / width;
        it->YOffset = it->YOffset / height;
        it->Width   = it->Width / width;
        it->Height  = it->Height / height;
    }
}

void Animation::Update(float deltaTime)
{
    m_CurrentTime += deltaTime;
    m_CurrentTime = fmod(m_CurrentTime, m_Duration);
}

void Animation::ToShader(std::shared_ptr<Shader> shader)
{
    // calculate current frame
    float unit = m_CurrentTime / m_Duration;
    int nr_frames = m_AnimationData.size();
    int current_frame = static_cast<int>(unit * nr_frames);
    // send relevant animation data to shader
    shader->SetVector4f("animationData", m_AnimationData[current_frame].ToVec4(), true);
}