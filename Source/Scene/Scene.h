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
#ifndef SCENE_H
#define SCENE_H

#include "ISceneNode.h"
#include "RootNode.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "LightManager.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/TextRenderer.h"
#include "../Renderer/ParticleEmitter.h"
#include <map>
#include <string>
#include <vector>
#include <memory>


typedef std::map<unsigned int, std::shared_ptr<ISceneNode>> SceneActorMap; // relates actor's to SceneNodes

class Scene
{
protected:
    std::shared_ptr<RootNode>        m_Root;
    std::shared_ptr<Camera>          m_Camera;
	std::shared_ptr<LightManager>    m_LightManager;
    std::shared_ptr<ParticleEmitter> m_ParticleEmitter; 

    MatrixStack   m_MatrixStack;
    SceneActorMap m_ActorMap;

	int m_SceneWidth, m_SceneHeight;
    std::string m_SceneIntro;
    std::string m_ScenePath;
    std::string m_AmbientPath;
public:
    Scene();
    virtual ~Scene();

    void Initialize();
    void Clear();
    void Restore();
    void Update(float deltaTime);
    void Render(Renderer *renderer);

    std::shared_ptr<ISceneNode> GetSceneNode(unsigned int ActorID);
    bool AddChild(unsigned int ActorID, std::shared_ptr<ISceneNode> child);
    bool RemoveChild(unsigned int ActorID);

    std::shared_ptr<Camera>       const GetCamera()         { return m_Camera; }
    std::shared_ptr<LightManager> const GetLightManager()   { return m_LightManager; }
	std::shared_ptr<RootNode>	  const GetRootNode()		{ return m_Root; }

	int         GetSceneWidth()  const { return m_SceneWidth; }
	int         GetSceneHeight() const { return m_SceneHeight; }
    std::string GetSceneIntro()        { return m_SceneIntro; }
    std::string GetScenePath()         { return m_ScenePath; }
    std::string GetAmbientPath()       { return m_AmbientPath; }
	void SetSceneWidth(int width)         { m_SceneWidth = width; }
	void SetSceneHeight(int height)       { m_SceneHeight = height; }
    void SetSceneIntro(std::string intro) { m_SceneIntro = intro; }
    void SetScenePath(std::string path)   { m_ScenePath = path; }
    void SetAmbientPath(std::string path) { m_AmbientPath = path; }

    void PushAndSetMatrix(const glm::mat4 model);
    void PopMatrix();
    const glm::mat4& GetTopMatrix();
};

#endif