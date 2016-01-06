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
#include "LightManager.h"

#include "../Application/GameApplication.h"

#include <iostream>
#include <algorithm>

LightManager::LightManager() : m_FocusType("")
{

}

void LightManager::AddLight(std::shared_ptr<LightNode> lightNode)
{
	// make sure it wasn't already added
	for(auto it = m_Lights.begin(); it != m_Lights.end(); it++)
		if((*it) == lightNode)
			return;
	m_Lights.push_back(lightNode);
}

void LightManager::RemoveLight(std::shared_ptr<LightNode> lightNode)
{
	m_Lights.erase(std::remove(m_Lights.begin(), m_Lights.end(), lightNode));
}

void LightManager::FocusOnLightType(std::string name)
{
    m_FocusType = name;
}

void LightManager::RemoveFocus()
{
    m_FocusType = "";
}

void LightManager::UpdateShader(Scene *scene, std::shared_ptr<Shader> shader)
{
	float time = GameApplication::GetInstance()->GetTime();
    // Update all lights that are currently visible to the shader (up to a maximum)
	shader->Use();
	int nr_active = 0;
	for(int i = 0; i < m_Lights.size(); ++i)
	{
        if(m_FocusType != "" && m_Lights[i]->GetName() != m_FocusType) // if focus, only render focussed lights
            continue;
		if (m_Lights[i]->IsVisible(scene))
		{
			float value = std::sin(GameApplication::GetInstance()->GetTime() * 4.0f + (i * m_Lights.size()) + (rand() % 100) / 200.0f) * (0.03 + (rand() % 100) / 1500.0f) + 1.0;
			std::shared_ptr<LightNode> light = m_Lights[i];
			shader->SetVector3f("Lights[" + std::to_string(nr_active) + "].Position", glm::vec3(light->GetPosition() + light->GetScale() * 0.5f, light->GetDepth())); // no need to inverse depth due to LHC system
			shader->SetVector3f("Lights[" + std::to_string(nr_active) + "].Diffuse", light->GetDiffuse() * glm::vec3(value));
			shader->SetVector3f("Lights[" + std::to_string(nr_active) + "].Specular", light->GetSpecular() * glm::vec3(value));
			shader->SetFloat("Lights[" + std::to_string(nr_active) + "].Linear", light->GetAttenuation() * value);
			++nr_active;
		}
	}
	shader->SetInteger("NrActiveLights", nr_active);
}