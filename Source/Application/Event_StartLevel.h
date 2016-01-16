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
#ifndef EVENT_START_LEVEL_H
#define EVENT_START_LEVEL_H

#include "../Communication/IEventData.h"

/*
    Fires whenever a (new) level should be started/loaded
*/
class Event_StartLevel : public IEventData
{
private:    
    std::string m_LevelPath; // file path to level/map data
public:   
    static const EventType s_EventType;  // the type of event

    Event_StartLevel(std:: string level) : m_LevelPath(level) { }

    // returns the type of event
    virtual const EventType& GetEventType() { return s_EventType; }
    // returns the event name
    virtual const std::string GetName() const { return "Event_LevelPath"; }
    // copies the event 
    virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_StartLevel(m_LevelPath)); }
    // returns the path to the level/map data
    virtual const std::string GetLevelPath() const { return m_LevelPath; }
};
#endif