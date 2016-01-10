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
#ifndef ACTOR_COMPONENT_H
#define ACTOR_COMPONENT_H

#include "ActorFactory.h"
#include "Actor.h"

#include <memory>
#include <string>

class Actor;

/*
    An individual Actor/Entity component that defines one part of an actor's game logic.
    An Actor can have multiple ActorComponents of which the combination forms the total of
    its logic.
*/
class ActorComponent
{
    friend class ActorFactory;
public:
    std::string            m_Type;  // the type of component
protected:
    std::shared_ptr<Actor> m_Owner; // the Actor that owns the component
public:
    virtual ~ActorComponent(void) { }

    // initializes the component; returns false if not successful
    virtual bool VInit() = 0;
    // updates the component
    virtual void VUpdate(float delta) { }
private:
    // sets the owner of the component; only ActorFactory should be able to do this
    void setOwner(std::shared_ptr<Actor> actor) { m_Owner = actor; }
    // sets the type of the component; only ActorFactory should be able to do this
    void setType(std::string type) { m_Type = type; }
};
#endif