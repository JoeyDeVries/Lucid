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
#ifndef EVENT_LIGHT_STATE_SWITCHED_H
#define EVENT_LIGHT_STATE_SWITCHED_H
#include "../Communication/IEventData.h"

enum LightState
{
	OFF,
	WHITE,
	RED,
	GREEN,
	BLUE
};

class Event_LightStateSwitched : public IEventData
{
private:
	LightState m_LightState;
public:
	static const EventType s_EventType;
	Event_LightStateSwitched(LightState lightState) : m_LightState(lightState) { }

	const EventType& GetEventType() { return s_EventType; }
	std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_LightStateSwitched(m_LightState)); }
	const std::string GetName() const { return "Event_LightStateSwitched"; }
	const LightState GetLightState() const { return m_LightState; }
};

#endif