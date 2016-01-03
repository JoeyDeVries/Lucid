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
#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H
#include <map>
#include <string>

#include "ActorComponentDefinitions.h"
#include "Actor.h"
#include "ActorComponent.h"


// definitions
typedef ActorComponent *(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> actorComponentCreatorMap;
 
   
 
class ActorFactory
{
// DATA
private:
    unsigned int m_lastActorID = 1; // 0 = no_actor
    actorComponentCreatorMap m_actorComponentCreators;
// FUNCTIONS
public:
    ActorFactory(void);
    ~ActorFactory(void);
    std::shared_ptr<Actor> CreateActor(DEFAULT_ACTOR_TYPES actorType);
private:
    std::shared_ptr<ActorComponent> createComponent(std::string component);
    unsigned int getNextActorID(void);
};


#endif