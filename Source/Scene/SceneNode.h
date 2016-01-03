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
#include "../Communication/IEventData.h"

class SceneNode : public ISceneNode
{
    friend class Scene;
protected:
    typedef std::vector<std::shared_ptr<ISceneNode>> SceneNodeList;
    SceneNodeList m_Children;
    SceneNode *m_Parent;
public:
    SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);
    virtual ~SceneNode() { };

    virtual void CalculateModel();

    virtual void Initialize(Scene *scene);
    virtual void Restore(Scene *scene);
    virtual void Update(Scene *scene, float deltaTime);
    virtual bool IsVisible(Scene *scene) const;
    virtual void PreRender(Scene *scene);
    virtual void Render(Scene *scene, Renderer *renderer);
    virtual void PostRender(Scene *scene);

    virtual bool AddChild(std::shared_ptr<ISceneNode> child);
    virtual bool RemoveChild(unsigned int actorID);
    virtual void RenderChildren(Scene *scene, Renderer *renderer);

	virtual SceneNodeList GetChildren() const { return m_Children; }
};

#endif