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
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "IEventData.h"

#include <memory>
#include <list>
#include <map>

/*
    A double buffed Event Queue (common design pattern for inter-component communication).
    The EventManager double queue allows any arbitrary process to register to special events
    at which point it will receive event updates as soon as the event occurs.
    This is implemented using FastDelegates for the event callbacks as detailed and implemented
    by Don Clugston.
    A double Queue system is used to prevent callback loops (events spawning new events that again
    spawn new events, potentially causing an infinite loop of events; ignoring other events).
*/
class EventManager
{
    typedef std::list<EventListenerDelegate>       EventListenerList; 
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<std::shared_ptr<IEventData>> EventQueue;
private:
    // State
    EventListenerMap m_EventListeners;  // holds list of event callback functions
    EventQueue       m_Queues[2];       // use two queues to prevent infinite loops 
    int              m_ActiveQueue;     // index of active processing queue; events are then queued onto the alternate queue.
public:
    EventManager() { m_ActiveQueue = 0; }

    // registers a delegate function to the given event type. When the event type
    // is triggered the delegate gets called. Returns true if succesful.
    bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
    // removes a delegate / event type pairing from the internal tables. 
    // returns false if no event of the given type was found
    bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);
    // triggers all event delegates registered to the event, bypassing the queue entirely
    bool TriggerEvent(const std::shared_ptr<IEventData>& pEvent) const;
    // queues an event that at runtime during the Update() function calls all registered
    // delegate functions corresponding to the event.
    bool QueueEvent(const std::shared_ptr<IEventData>& pEvent);

    // processes the queue and calls the relevant event callback delegates
    bool Update();
    // clears both event queues 
    void Clear();
};
#endif