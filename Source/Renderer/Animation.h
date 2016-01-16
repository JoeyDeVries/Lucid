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
#ifndef ANIMATION_H
#define ANIMATION_H

#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Texture2D;
class Shader;

/*
    Contains all the animation data relevant of rendering a single animation frame.
*/
struct AnimationData
{
    float XOffset, YOffset; // the texture coordinate x and y offsets
    float Width, Height;    // the width and height of each frame in the spritesheet

    // returns the animation data in a convenient 4-element vector
    glm::vec4 ToVec4()
    {
        return glm::vec4(XOffset, YOffset, Width, Height);
    }
};

/*
    Holds all relevant spritesheet animation data. This includes the actual spritesheets
    and the per-frame animation data required to render the animation. 
    Animation also calculates on a per-frame basis the animation position/time-frame.
*/
class Animation
{
private:
    std::vector<AnimationData> m_AnimationData; // animation data for each animation frame
    std::shared_ptr<Texture2D> m_Diffuse;       // the diffuse animation spritesheet
    std::shared_ptr<Texture2D> m_Specular;      // the specular animation spritesheet
    std::shared_ptr<Texture2D> m_Normal;        // the normal animation spritesheet

    std::string m_Name;        // the name of the animation
    float       m_Duration;    // the total duration of the animation in seconds 
    float       m_CurrentTime; // the normalized time the animation is currently rendering with in the unit range [0,1]
public:
    Animation(std::vector<AnimationData> &data);

    // getters
    std::shared_ptr<Texture2D> GetDiffuse();
    std::shared_ptr<Texture2D> GetSpecular();
    std::shared_ptr<Texture2D> GetNormal();
    std::string GetName();
    // setters
    void SetDiffuse(std::shared_ptr<Texture2D> diffuse);
    void SetSpecular(std::shared_ptr<Texture2D> specular);
    void SetNormal(std::shared_ptr<Texture2D> normal);
    void SetDuration(float duration);
    void SetName(std::string name);

    // initializes the animation shader uniforms
    void Init(std::shared_ptr<Shader> shader);
    // normalizes all animation data to ranges within the unit interval [0,1]; regardless of original pixel dimensions
    void Normalize(int width, int height);
    // updates the animation frame to render 
    void Update(float deltaTime);
    // sends all relevant animation data of the current timestep to the shader
    void ToShader(std::shared_ptr<Shader> shader);
};
#endif