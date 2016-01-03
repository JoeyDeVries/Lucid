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
#ifndef EVENT_LEVEL_COMPLETE_H
#define EVENT_LEVEL_COMPLETE_H
#include "../Communication/IEventData.h"

class Event_LevelComplete : public IEventData
{
private:
public:
	static const EventType s_EventType;
	Event_LevelComplete() { }

	const EventType& GetEventType() { return s_EventType; }
	std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_LevelComplete()); }
	const std::string GetName() const { return "Event_LevelComplete"; }
};

#endif