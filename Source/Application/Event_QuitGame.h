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
#ifndef EVENT_QUIT_GAME_H
#define EVENT_QUIT_GAME_H

#include "../Communication/IEventData.h"

/*
    Fires whenever the game should be closed
*/
class Event_QuitGame : public IEventData
{
public:   
    static const EventType s_EventType;  // the type of event

    Event_QuitGame() { }

    // returns the type of event
    virtual const EventType& GetEventType() { return s_EventType; }
    // returns the event name
    virtual const std::string GetName() const { return "Event_QuitGame"; }
    // copies the event 
    virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_QuitGame()); }
};
#endif