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
#include "Scene.h"

#include "../Application/GameApplication.h"
#include "../Resources/ResourceManager.h"
#include "../Scene/LightNode.h"

Scene::Scene()
{
    m_Root.reset(new RootNode);
    m_Camera.reset(new Camera);
    m_LightManager.reset(new LightManager);
    m_ParticleEmitter.reset(new ParticleEmitter);
}

Scene::~Scene()
{

}

void Scene::Initialize()
{
    // initializes each scene node
    m_Root->Initialize(this);    

    m_ParticleEmitter->Init(300, m_Camera->GetProjection());

    if(m_AmbientPath != "")
        GameApplication::GetInstance()->GetAudio()->PlaySound(m_AmbientPath, true, 0.5f);
}

void Scene::Clear()
{
    m_Root.reset(new RootNode);
    m_LightManager.reset(new LightManager);
    m_ActorMap.clear();
    m_MatrixStack.Clear();
    m_ParticleEmitter.reset(new ParticleEmitter);
}

void Scene::Restore()
{
    if(m_Root)
        m_Root->Restore(this);
    if (m_Camera)
    {
        m_Camera->SetProjection(GameApplication::GetInstance()->ScreenWidth(), GameApplication::GetInstance()->ScreenHeight());
    }
}

void Scene::Update(float deltaTime)
{
    if(!m_Root)
        return;

    m_Root->Update(this, deltaTime);

    m_ParticleEmitter->Update(this, deltaTime, 2); // 600 particles are required (nrParticles * newParticles == 600 should be true)
}

void Scene::Render(Renderer *renderer)
{
    if (m_Root && m_Camera)
    {
		m_Camera->CalculateViewMatrix();

        // Update lighting parameters
        GetLightManager()->UpdateShader(this, ResourceManager::GetInstance()->GetShader("sprite"));
        GetLightManager()->UpdateShader(this, m_ParticleEmitter->GetShader());

		m_Root->Render(this, renderer);

        m_ParticleEmitter->Render(renderer, m_Camera->GetView());
    }
}

std::shared_ptr<ISceneNode> Scene::GetSceneNode(unsigned int ActorID)
{
    SceneActorMap::iterator i = m_ActorMap.find(ActorID);
    if (i == m_ActorMap.end())
    {
        return std::shared_ptr<ISceneNode>();
    }
    return i->second;
}

bool Scene::AddChild(unsigned int ActorID, std::shared_ptr<ISceneNode> child)
{
    m_ActorMap[ActorID] = child;
    // cast child into different node types and add to other managers if necessary (like if it is a light node, then add it to light manager)
    std::shared_ptr<LightNode> pLight = std::dynamic_pointer_cast<LightNode>(child);
    if (pLight)
    {   // child is a LightNode; also add a reference to the LightManager
         GetLightManager()->AddLight(pLight);  
    }
    // otherwise add it as child to Root node
    return m_Root->AddChild(child);
}

bool Scene::RemoveChild(unsigned int ActorID)
{
    std::shared_ptr<ISceneNode> kid = GetSceneNode(ActorID);
    // Check if it is any of the different node types (like light) and act accordingly)
    std::shared_ptr<LightNode> pLight = std::dynamic_pointer_cast<LightNode>(kid);
    if (pLight)
    {    // child is a LightNode; also remove reference to the LightManager
         GetLightManager()->RemoveLight(pLight);  
    }
    // Otherwise, erase as normal
    m_ActorMap.erase(ActorID);
    return m_Root->RemoveChild(ActorID);
}

void Scene::PushAndSetMatrix(const glm::mat4 model)
{
    m_MatrixStack.Push(model);
}

void Scene::PopMatrix()
{
    m_MatrixStack.Pop();
}

const glm::mat4& Scene::GetTopMatrix()
{
    return m_MatrixStack.GetTopMatrix();
}
