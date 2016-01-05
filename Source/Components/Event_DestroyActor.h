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
#ifndef DESTROY_ACTOR_H
#define DESTROY_ACTOR_H

#include "../Communication/IEventData.h"

/*
    Fires whenever an actor is destroyed and should be cleaned accordingly 
    in each syb-system.
*/
class Event_DestroyActor : public IEventData
{
private:
    ActorID m_ActorID; // the ID of the actor to remove
public:
    static const EventType s_EventType;
    Event_DestroyActor(ActorID actorID) : m_ActorID(actorID) { }

    // returns the type of event
    virtual const EventType& GetEventType() { return s_EventType; } 
    // returns the event name
    virtual const std::string GetName() const { return "Event_DestroyActor"; } 
    // copies the event 
    virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_DestroyActor(m_ActorID)); }
    // returns the ID of the actor to destroy/remove
    const ActorID GetActorID() const { return m_ActorID; }
};
#endif