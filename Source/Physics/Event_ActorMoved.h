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
#ifndef EVENT_ACTOR_MOVED_H
#define EVENT_ACTOR_MOVED_H
#include "../Communication/IEventData.h"
#include "../Components/Actor.h"
#include <glm/glm.hpp>

//typedef unsigned int ActorID;

class Event_ActorMoved : public IEventData
{
private:
	ActorID m_ActorID;
	glm::vec2 m_NewPosition;
    float m_Rotation;
public:
	static const EventType s_EventType;
	Event_ActorMoved(ActorID actorID, glm::vec2 NewPos, float Rotation) : m_ActorID(actorID), m_NewPosition(NewPos), m_Rotation(Rotation) { }

	virtual const EventType& GetEventType() { return s_EventType; }
	virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_ActorMoved(m_ActorID, m_NewPosition, m_Rotation)); }
	virtual const std::string GetName() const { return "Event_ActorMoved"; }
	const ActorID GetActorID() const { return m_ActorID; }
	const glm::vec2 GetNewPosition() const { return m_NewPosition; }
    const float     GetNewRotation() const { return m_Rotation; }
};

#endif