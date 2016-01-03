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

// EventQueue pattern
class EventManager
{
    typedef std::list<EventListenerDelegate> EventListenerList; 
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::list<std::shared_ptr<IEventData>> EventQueue;
private:
    std::shared_ptr<EventManager> m_Instance;

    EventListenerMap m_EventListeners;
    EventQueue m_Queues[2]; // Use two queues to prevent infinite loops (if event spawns new events, place it in other queue; otherwise it could loop infinitely)
    int m_ActiveQueue; // index of active processing queue; events are then queued onto the alternate queue.

    
public:
    EventManager() { m_ActiveQueue = 0; }
    //std::shared_ptr<EventManager> GetInstance() 
    //{ 
    //    if (!m_Instance) 
    //        m_Instance.reset(new EventManager()); 
    //    return m_Instance; 
    //}

    // Registers a delegate function to the given event type. When the event type
    // is triggered the delegate gets called. Returns true if succesful.
    bool AddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
    // Removes a delegate / event type pairing from the internal tables. 
    // Returns false if no event of the given type was found
    bool RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);

    // Triggers all event delegates registered to the event, bypassing the queue entirely
    bool TriggerEvent(const std::shared_ptr<IEventData>& pEvent) const;
    // Queues an event that at runtime during the Update() function calls all registered
    // delegate functions corresponding to the event.
    bool QueueEvent(const std::shared_ptr<IEventData>& pEvent);

    bool Update();
    void Clear();
};




#endif
