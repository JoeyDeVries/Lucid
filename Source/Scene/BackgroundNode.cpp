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
#include "BackgroundNode.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/Material.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/shader.h"
#include "../Scene/Camera.h"

BackgroundNode::BackgroundNode(unsigned int ActorID) : SceneNode(ActorID, "Background", "BACKGROUND", glm::vec2(0.0), 0)
{

}

void BackgroundNode::CalculateModel()
{
    // Only do scaling operations, no need to translate/position for background (except depth)
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0, 0.0, -m_Depth));
    model = glm::scale(model, glm::vec3(m_Scale, 1.0));
    m_Model = model;
}

void BackgroundNode::Initialize(Scene *scene)
{
    SetScale(glm::vec2(scene->GetSceneWidth(), scene->GetSceneHeight()));
}

void BackgroundNode::Render(Scene *scene, Renderer *renderer)
{
    // Render background
    m_Material->GetShader()->Use();
    m_Material->GetShader()->SetMatrix4("model", m_Model);
    m_Material->GetShader()->SetMatrix4("view", scene->GetCamera()->GetView());
    m_Material->PreRender();
    renderer->RenderQuad();
}