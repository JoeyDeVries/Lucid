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

#include "MatrixStack.h"
#include "ISceneNode.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

class ParticleEmitter;
class LightManager;
class Renderer;
class RootNode;
class Camera;

typedef std::map<unsigned int, std::shared_ptr<ISceneNode>> SceneActorMap; // relates actor's to SceneNodes

/*
    Represents the renderable portion of the game. Holds a scene graph of differently
    typed scene nodes with parent-child relationships. Rendering starts at the root
    node that manages different render passes. The scene also holds a camera object
    that represents the render-region/viewport of the scene. The light manager and
    particle emitter add to the graphic fidelity of the scene.
*/
class Scene
{
protected:
    std::shared_ptr<RootNode>        m_Root;            // manages different render passes and the whole scenegraph render-setup as a whole
    std::shared_ptr<Camera>          m_Camera;          // represents the user render region of the scene
    std::shared_ptr<LightManager>    m_LightManager;    // represents all light sources in a scene and organizes them for efficient rendering
    std::shared_ptr<ParticleEmitter> m_ParticleEmitter; // renders particle effects in an efficient fashion to the scene

    MatrixStack   m_MatrixStack; // holds a stack of matrices to represent parent-child matrix transformation relations
    SceneActorMap m_ActorMap;    // holds a map relating unique actor IDs to their respective scene nodes, seperating game logic from render logic

    int m_SceneWidth, m_SceneHeight; // the width and height of the scene
    std::string m_SceneIntro;        // the text displayed at the introduction of the scene
    std::string m_ScenePath;         // the path the scene scene/level was generated from
    std::string m_AmbientPath;       // the path to the ambient background music file
public:
    Scene();
    virtual ~Scene();

    // getters
    std::shared_ptr<ISceneNode> GetSceneNode(unsigned int ActorID);

    std::shared_ptr<Camera>       const GetCamera()       { return m_Camera; }
    std::shared_ptr<LightManager> const GetLightManager() { return m_LightManager; }
    std::shared_ptr<RootNode>	  const GetRootNode()     { return m_Root; }

    int         GetSceneWidth()  const { return m_SceneWidth; }
    int         GetSceneHeight() const { return m_SceneHeight; }
    std::string GetSceneIntro()        { return m_SceneIntro; }
    std::string GetScenePath()         { return m_ScenePath; }
    std::string GetAmbientPath()       { return m_AmbientPath; }
    // setters
    void SetSceneWidth(int width)         { m_SceneWidth = width; }
    void SetSceneHeight(int height)       { m_SceneHeight = height; }
    void SetSceneIntro(std::string intro) { m_SceneIntro = intro; }
    void SetScenePath(std::string path)   { m_ScenePath = path; }
    void SetAmbientPath(std::string path) { m_AmbientPath = path; }

    // initializes the scene 
    void Initialize();
    // resets the scene to its default initialized state
    void Clear();
    // restores the scene and all nodes after a process/focus switch or change of configuration 
    void Restore();
    // updates the scene and all nodes
    void Update(float deltaTime);
    // renders the scene with all its nodes
    void Render(Renderer *renderer);

    // adds a child node to the scene graph
    bool AddChild(unsigned int ActorID, std::shared_ptr<ISceneNode> child);
    // removes a child node from the scene graph
    bool RemoveChild(unsigned int ActorID);

    // pushes a matrix on top of the transformation stack
    void PushAndSetMatrix(const glm::mat4 model);
    // pops a matrix from the transformation stack
    void PopMatrix();
    // returns the top matrix on the transformation stack
    const glm::mat4& GetTopMatrix();
};
#endif