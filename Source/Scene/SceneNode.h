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
#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "ISceneNode.h"

/*
    Intstantieted version of the abstract base class ISceneNode. SceneNode represents a renderable
    node for use in the scene graph. Each scene node holds a list of other node children in which
    each child's transform is built up on top of its parent's transform.
*/
class SceneNode : public ISceneNode
{
    friend class Scene;
protected:
    typedef std::vector<std::shared_ptr<ISceneNode>> SceneNodeList;
    SceneNodeList m_Children; // the list of scene node children this node list has
    SceneNode     *m_Parent;  // a link back to the parent scene node of this scene node
public:
    SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);
    virtual ~SceneNode() { };

    // (re-)calculates the model matrix based on the scene node positional properties
    virtual void CalculateModel();

    // initializes the scene node and its children
    virtual void Initialize(Scene *scene);
    // restores the scene node and its children after switching scenes/game-state or window losing focus
    virtual void Restore(Scene *scene);
    // updates the scene node and all its children
    virtual void Update(Scene *scene, float deltaTime);

    // determines if the scene node is visible to the scene's camera based on its dimensions
    virtual bool IsVisible(Scene *scene) const;
    // does pre-render configuration before rendering (and its children)
    virtual void PreRender(Scene *scene);
    // renders the scene node and its children
    virtual void Render(Scene *scene, Renderer *renderer);
    // does post-render configuration after rendering (and its children)
    virtual void PostRender(Scene *scene);

    // adds a child scene node to the scene node
    virtual bool AddChild(std::shared_ptr<ISceneNode> child);
    // removes a child scene node from the scene node
    virtual bool RemoveChild(unsigned int actorID);
    // renders only the scene node's children
    virtual void RenderChildren(Scene *scene, Renderer *renderer);

    // returns a list of the scene node's children
    virtual SceneNodeList GetChildren() const { return m_Children; }
};
#endif