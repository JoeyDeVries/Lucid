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
#include "LightNode.h"

#include "../Application/GameApplication.h"
#include "../Resources/ResourceManager.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"


LightNode::LightNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, float depth, glm::vec2 scale, glm::vec3 diffuse, glm::vec3 specular, float attenuation)
    : SpriteNode(ActorID, name, renderPass, position, depth, scale, 0.0f)
{
    m_Diffuse = diffuse;
    m_Specular = specular;
    m_Attenuation = attenuation;
}

bool LightNode::IsVisible(Scene *scene) const
{
    BoundingBox cameraBox = scene->GetCamera()->GetBoundingBox();
    // extend the camera bounding box by the attenuation parameter as its radius extends its visibility
    glm::vec2 halfExtendsOffset(m_Attenuation * 0.6f); 
    cameraBox.HalfExtents += halfExtendsOffset;
    // determine if current node is within the camera's visible frustum
    bool visible = true;
    if (m_Position.x > cameraBox.Center.x + cameraBox.HalfExtents.x)
        visible = false; // light is outside right camera edge
    else if (m_Position.x + m_Scale.x < cameraBox.Center.x - cameraBox.HalfExtents.x)
        visible = false; // light is outside left camera edge
    else if (m_Position.y > cameraBox.Center.y + cameraBox.HalfExtents.y)
        visible = false; // light is outside bottom camera edge
    else if (m_Position.y + m_Scale.y < cameraBox.Center.y - cameraBox.HalfExtents.y)
        visible = false;
    return visible;
}