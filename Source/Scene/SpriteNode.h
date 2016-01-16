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

#include <memory>
#include <map>

class IEventData;
class Animation;
class Scene;

/*
    SpriteNode is a derivation of scene node specifically for rendering 2D sprites with
    or without animations. If an animation has been given, sprite node uses the 
    animation's corresponding material instead of its default material for rendering.
*/
class SpriteNode : public SceneNode
{
protected:
    std::string m_ActiveAnimation;  // states which animation (if any) is currently active/rendered
    bool        m_Animation;        // states whether this sprite node uses animations
    bool        m_Reverse;          // states whether the graphics should be reversed on x-axis (sprite moving opposite direction)

    std::map<std::string, std::shared_ptr<Animation>> m_Animations; // holds list of animations: <anim-state, animation>
public:
    SpriteNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);
    ~SpriteNode();

    // getters
    bool HasAnimation();
    bool GetReverse();
    // setters
    void SetAnimation(bool enable);
    void SetReverse(bool reverse);

    // initializes the sprite node and its children
    virtual void Initialize(Scene *scene);
    // updates the sprite node and all its children
    virtual void Update(Scene *scene, float deltaTime);
    // renders the sprite node and its children
    virtual void Render(Scene *scene, Renderer *renderer);
  
    // returns an animation part of a specific state of the sprite node
    std::shared_ptr<Animation> GetAnimation(std::string state);
    // adds an animation as a given state of the sprite node
    void                       AddAnimation(std::shared_ptr<Animation> animation, std::string state = "idle");
    // activates a given animation state of the spritenode; the animation that corresponds to this state is now active
    void                       ActivateAnimation(std::string state);	

    // fires whenever an actor was moved from the physics system
    void ActorMoved(std::shared_ptr<IEventData> eventData);
};
#endif