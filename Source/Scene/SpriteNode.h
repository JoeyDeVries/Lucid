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
#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H
#include "SceneNode.h"
#include "../Renderer/Animation.h"
#include <memory>
#include <map>

class Scene;
class SpriteNode : public SceneNode
{
protected:
	bool m_Animation;
    bool m_Reverse;
	std::string m_ActiveAnimation;
	std::map<std::string, std::shared_ptr<Animation>> m_Animations; // <anim-state, animation>
public:
    SpriteNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);
    ~SpriteNode();

    virtual void Initialize(Scene *scene);
    virtual void Update(Scene *scene, float deltaTime);
    virtual void Render(Scene *scene, Renderer *renderer);

	void					   SetAnimation(bool enable);
	bool					   HasAnimation();
    void                       SetReverse(bool reverse);
    bool                       GetReverse();
	void					   AddAnimation(std::shared_ptr<Animation> animation, std::string state = "idle");
	std::shared_ptr<Animation> GetAnimation(std::string state);
	void					   ActivateAnimation(std::string state);	


    void ActorMoved(std::shared_ptr<IEventData> eventData);
};

#endif