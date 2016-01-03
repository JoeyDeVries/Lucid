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
#include <memory>
#include "ActorFactory.h"
#include "Actor.h"

class Actor;

class ActorComponent
{
    friend class ActorFactory;
// DATA
public:
    std::string m_Type;
protected:
    std::shared_ptr<Actor> m_Owner;
// FUNCTIONS
public:
    virtual ~ActorComponent(void) { }
    virtual bool VInit() = 0;
    virtual void VUpdate(float delta) { }
private:
    void setOwner(std::shared_ptr<Actor> actor) { m_Owner = actor; }
	void setType(std::string type) { m_Type = type; }
};

#endif