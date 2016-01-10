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

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>


class Renderer;
class Material;
class Scene;

/*
    The interface for a game render object stored in a scene graph. Holds graphics and graph-
    related functionality. Holds a list of children scene nodes that inherit the transformations
    of their parent node. Note that the abstract base class does not contain a list of scene 
    node children; this is reserved for the class definition of SceneNode.h.

    IDEA(Joey): if using more hierarchy systems: cache parent-child world-transform
    of current child node and re-calculate from parent when requested.
    Use dirty bit/flag pattern to only re-calculate this if the local system
    is marked dirty; otherwise use cached version.
*/
class ISceneNode
{
protected:
    unsigned int m_ActorID;    // unique actor ID this scene node is related to
    std::string  m_Name;       // name of the scene node
    std::string  m_RenderPass; // type of render pass this node should render in
    glm::mat4    m_Model;      // the local to world space model matrix
    glm::vec2    m_Position;   // the world-space position of the scene node
    glm::vec2    m_Scale;      // the world-space scale of the scene node
    int          m_Depth;      // the depth of the scene node in the overal scene
    float        m_Rotation;   // the rotation in angle of the scene node
    float        m_radius;     // the radius of the scene node for frustum visibility occlusion 

    std::shared_ptr<Material>  m_Material; // the material the scene node uses for describing its render appearance
public:
    virtual ~ISceneNode() { };

    // getters
    const unsigned int &GetActorID()    const { return m_ActorID; }
    glm::vec2 const    &GetPosition()         { return m_Position; }
    glm::vec2 const    &GetScale()            { return m_Scale; }
    int const          &GetDepth()            { return m_Depth; }
    float const        &GetRotation()         { return m_Rotation; }
    glm::mat4 const    &GetModel()            { return m_Model; }
    const std::string  &GetName()       const { return m_Name; }
    const std::string  &GetRenderPass() const { return m_RenderPass; }    
    glm::vec2          GetCenter()            { return m_Position + m_Scale * 0.5f; }
    std::shared_ptr<Material>  GetMaterial()  { return m_Material; }	
    // setters
    void SetPosition(glm::vec2 position)  { m_Position = position; CalculateModel(); }
    void SetScale(glm::vec2 scale)        { m_Scale    = scale;    CalculateModel(); }
    void SetDepth(int depth)              { m_Depth    = depth;    CalculateModel(); }
    void SetRotation(float rotation)      { m_Rotation = rotation; CalculateModel(); }
	void SetName(const std::string& name) { m_Name = name; }
	void SetRenderPass(const std::string& renderPass)    { m_RenderPass = renderPass; }    
    void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    // (re-)calculates the model matrix based on the scene node positional properties
    virtual void CalculateModel() = 0;

    // initializes the scene node and its children
    virtual void Initialize(Scene *scene) = 0;
    // restores the scene node and its children after switching scenes/game-state or window losing focus
    virtual void Restore(Scene *scene) = 0;
    // updates the scene node and all its children
    virtual void Update(Scene *scene, float deltaTime) = 0;

    // determines if the scene node is visible based on its dimensions
    virtual bool IsVisible(Scene *scene) const = 0;
    // does pre-render configuration before rendering (and its children)
    virtual void PreRender(Scene *scene) = 0;
    // renders the scene node and its children
    virtual void Render(Scene *scene, Renderer *renderer) = 0;
    // does post-render configuration after rendering (and its children)
    virtual void PostRender(Scene *scene) = 0;
    
    // adds a child scene node to the scene node
    virtual bool AddChild(std::shared_ptr<ISceneNode> child) = 0;
    // removes a child scene node from the scene node
    virtual bool RemoveChild(unsigned int actorID) = 0;
    // renders only the scene node's children
    virtual void RenderChildren(Scene *scene, Renderer *renderer) = 0;

    // returns a list of the scene node's children
	virtual std::vector<std::shared_ptr<ISceneNode>> GetChildren() const = 0;
};
#endif