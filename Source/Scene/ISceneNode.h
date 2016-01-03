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
#ifndef ISCENE_NODE_H
#define ISCENE_NODE_H

#include "../Renderer/Material.h"

#include <string>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>


// TODO(Joey): if using more hierarchy systems: cache parent-child world-transform
// of current child node and re-calculate from parent when requested.
// Use dirty bit/flag pattern to only re-calculate this if the local system
// is marked dirty; otherwise use cached version.
class Scene;
class Renderer;

class ISceneNode
{
protected:
    unsigned int m_ActorID;
    std::string  m_Name;
    std::string  m_RenderPass;
    glm::mat4    m_Model;
    glm::vec2    m_Position;
    glm::vec2    m_Scale;
    int          m_Depth;
    float        m_Rotation;
    float        m_radius; // for frustum visibility occlusion 

    std::shared_ptr<Material>  m_Material;
public:
    virtual ~ISceneNode() { };
    const unsigned int &GetActorID()    const { return m_ActorID; }
    glm::vec2 const    &GetPosition()         { return m_Position; }
    glm::vec2 const    &GetScale()            { return m_Scale; }
    int const          &GetDepth()            { return m_Depth; }
    float const        &GetRotation()         { return m_Rotation; }
    glm::mat4 const    &GetModel()            { return m_Model; }
    const std::string  &GetName()       const { return m_Name; }
    const std::string  &GetRenderPass() const { return m_RenderPass; }
    
    glm::vec2 GetCenter() { return m_Position + m_Scale * 0.5f; }

    std::shared_ptr<Material>  GetMaterial()  { return m_Material; }
	
    virtual void CalculateModel() = 0;
    void SetPosition(glm::vec2 position) { m_Position = position; CalculateModel(); }
    void SetScale(glm::vec2 scale)       { m_Scale    = scale;    CalculateModel(); }
    void SetDepth(int depth)             { m_Depth    = depth;    CalculateModel(); }
    void SetRotation(float rotation)     { m_Rotation = rotation; CalculateModel(); }
	void SetName(const std::string& name) { m_Name = name; }
	void SetRenderPass(const std::string& renderPass) { m_RenderPass = renderPass; }
    
    void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    virtual void Initialize(Scene *scene) = 0;
    virtual void Restore(Scene *scene) = 0;
    virtual void Update(Scene *scene, float deltaTime) = 0;
    virtual bool IsVisible(Scene *scene) const = 0;
    virtual void PreRender(Scene *scene) = 0;
    virtual void Render(Scene *scene, Renderer *renderer) = 0;
    virtual void PostRender(Scene *scene) = 0;
    
    virtual bool AddChild(std::shared_ptr<ISceneNode> child) = 0;
    virtual bool RemoveChild(unsigned int actorID) = 0;
    virtual void RenderChildren(Scene *scene, Renderer *renderer) = 0;

	virtual std::vector<std::shared_ptr<ISceneNode>> GetChildren() const = 0;
};

#endif