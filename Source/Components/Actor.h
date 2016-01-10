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
#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>

#include "ActorComponentDefinitions.h"
#include "ActorFactory.h"
#include "ActorComponent.h"

#include <memory>
#include <map>

// ActorID is simply an unsigned int
typedef unsigned int ActorID;

class ActorComponent;

/*
    The default game object/entity in the game world. Each object that has renderable state and/or
    game logic is represented by an Actor. Each Actor is uniquely identified throughout the game.
    The game logic of Actors are built up around a semi-derivative entity-component pattern where
    its logic is derived from its component (aggregation over inheritance).
    An Actor should NOT be subclassed: all variaty comes from its components. Its only purpose is
    to manage its components.
*/
class Actor
{
    friend class ActorFactory;
    typedef std::map<std::string, std::shared_ptr<ActorComponent>> ActorComponents;
private:
    // actor state
    ActorID		        m_id;         // unique identifier of the actor
    ActorComponents     m_components; // the 'entity' components of an actor that defines its logic
    DEFAULT_ACTOR_TYPES m_Type;       // the type of the actor

    glm::vec2 m_Position; // the world-space position of the actor
    glm::vec2 m_Scale;    // the world-space scale dimensions of the actor
    int       m_Depth;    // the world-space depth of the actor
    float     m_Rotation; // the world-space rotation angle of the actor

    // adds a component to the actor which should be called from ActorFactory only
    void addComponent(std::shared_ptr<ActorComponent> component);
    // sets the ID of the current actor
    void setID(ActorID id);
    // sets the type of the actor 
    void setType(DEFAULT_ACTOR_TYPES type) { m_Type = type; }
public:
    Actor(void);
    ~Actor(void);

    // destroys the actor and its components
    void Destroy(void);
    // updates the actor's components
    void Update(float deltaTime);

    // getters
    DEFAULT_ACTOR_TYPES GetType() { return m_Type; }
    const glm::vec2&    GetPosition() { return m_Position; }
    const glm::vec2&    GetScale() { return m_Scale; }
    const int&          GetDepth() { return m_Depth; }
    const float&        GetRotation() { return m_Rotation; }
    const unsigned int  GetID() { return m_id; }
    glm::vec2           GetCenter() { return m_Position + m_Scale * 0.5f; }
    // setters
    void SetPosition(glm::vec2 pos) { m_Position = pos; }
    void SetScale(glm::vec2 scale) { m_Scale = scale; }
    void SetDepth(int depth) { m_Depth = depth; }
    void SetRotation(float rotation) { m_Rotation = rotation; }

    // NOTE(Joey): unfortunately in C++ template functions need to be declared and defined in the same file
    // retrieves a weak pointer to the component of an actor if it exists
    template<class ComponentType> std::weak_ptr<ComponentType> GetComponent(std::string type)
    {
        ActorComponents::iterator findIt = m_components.find(type);
        if (findIt != m_components.end())
        {
            std::shared_ptr<ActorComponent> pBase(findIt->second);
            // cast to subclass version of the pointer
            std::shared_ptr<ComponentType> pSub(std::tr1::static_pointer_cast<ComponentType>(pBase));
            std::weak_ptr<ComponentType> pWeakSub(pSub); // convert strong pointer to weak pointer
            return pWeakSub;
        }
        else
            return std::weak_ptr<ComponentType>();
    }
};
#endif