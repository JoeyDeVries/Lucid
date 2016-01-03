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
#include "texture2D.h"
#include "shader.h"
#include <vector>
#include <memory>

struct AnimationData
{
	float XOffset, YOffset;
	float Width, Height;

	glm::vec4 ToVec4() 
	{
		return glm::vec4(XOffset, YOffset, Width, Height);
	}
};

class Animation
{
private:
    std::string m_Name;
    std::vector<AnimationData> m_AnimationData;
	float m_Duration;
	float m_CurrentTime; // transform to unit interval in calculations or irrelevant? -> (scaled to m_Duration for now)
    // animation sheets
    std::shared_ptr<Texture2D> m_Diffuse;
    std::shared_ptr<Texture2D> m_Specular;
    std::shared_ptr<Texture2D> m_Normal;
public:
	Animation(std::vector<AnimationData> &data);

    std::string GetName();
    std::shared_ptr<Texture2D> GetDiffuse();
    std::shared_ptr<Texture2D> GetSpecular();
    std::shared_ptr<Texture2D> GetNormal();

	void SetDuration(float duration);
    void SetName(std::string name);
    void SetDiffuse(std::shared_ptr<Texture2D> diffuse);
    void SetSpecular(std::shared_ptr<Texture2D> specular);
    void SetNormal(std::shared_ptr<Texture2D> normal);

    

	void Init(std::shared_ptr<Shader> shader);
	void Normalize(int width, int height); // normalizes animation value from pixels to render-texture coordinates
	void Update(float deltaTime);
	void ToShader(std::shared_ptr<Shader> shader);

};

#endif