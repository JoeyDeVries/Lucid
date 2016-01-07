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
#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H

#include <glm/glm.hpp>

#include "SpriteNode.h"

/*
    A scene node that represents a light source and should emit light. A light node is also
    rendered as a torch and thus inherits from SpriteNode for its render parts. When a 
    light node is added to a scene it is both added to the scene graph and to the light 
    manager that manages its the shader's required lighting data.
*/
class LightNode : public SpriteNode
{
    glm::vec3 m_Diffuse;     // the diffuse color of the light source
    glm::vec3 m_Specular;    // the specular color of the light source
    float     m_Attenuation; // the attenuation component of the light source (the maximum number of pixels it's light is lit)
public:
    LightNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, float depth, glm::vec2 scale, glm::vec3 diffuse, glm::vec3 specular, float attenuation);

    // getters
    glm::vec3& const GetDiffuse()     { return m_Diffuse; }
    glm::vec3& const GetSpecular()    { return m_Specular; }
    float&     const GetAttenuation() { return m_Attenuation; }
    // setters
	void SetDiffuse(const glm::vec3& diffuse)     { m_Diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular)   { m_Specular = specular; }
	void SetAttenuation(const float& attenuation) { m_Attenuation = attenuation; }

    // determines whether the light source is visible from the camera's perspective, including its attenuation equation.
	bool IsVisible(Scene *scene) const;
};
#endif