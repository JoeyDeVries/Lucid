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
#include "EventManager.h"

bool EventManager::AddListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
    // get the list of events of the given EventType
    EventListenerList& eventListeners = m_EventListeners[type]; // no need to check if type exists; a default list is created if empty and its reference returned
    // check if delegate isn't already registered for this EvenType
    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
    {
        if (eventDelegate == (*it)) // fast delegates have proper comparrison operators
        {
            // error: event delegate is already registred; don't want to call the same delegate twice
            return false;
        }
    }
    eventListeners.push_back(eventDelegate);
    return true;
}

bool EventManager::RemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
    bool success = false;
    auto findIt = m_EventListeners.find(type);
    if (findIt != m_EventListeners.end())
    {
        EventListenerList& eventListeners = m_EventListeners[type];
        for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
        {
            if (eventDelegate == (*it))
            {
                eventListeners.erase(it);
                success = true;
                // no need to continue; no duplicate delegates allowed
                break; 
            }
        }
    }
    return success;
}

bool EventManager::TriggerEvent(const std::shared_ptr<IEventData>& pEvent) const
{
    bool processed = false;
    auto findIt = m_EventListeners.find(pEvent->GetEventType());
    if (findIt != m_EventListeners.end())
    {
        const EventListenerList& eventListeners = findIt->second;
        for (EventListenerList::const_iterator it = eventListeners.begin(); it != eventListeners.end(); ++it)
        {
            EventListenerDelegate listener = (*it);
            listener(pEvent);
            processed = true;
        }
    }
    return processed;
}

bool EventManager::QueueEvent(const std::shared_ptr<IEventData>& pEvent)
{
    // only process events if there are any event listeners associated with it
    auto findIt = m_EventListeners.find(pEvent->GetEventType());
    if (findIt != m_EventListeners.end())
    {
        m_Queues[m_ActiveQueue].push_back(pEvent);
        return true;
    }
    return false;
}

bool EventManager::Update()
{
    // swap queues
    int activeQueue = m_ActiveQueue;
    m_ActiveQueue = (m_ActiveQueue + 1) % 2;
    m_Queues[m_ActiveQueue].clear(); // clear new queue after swap

    // keep processing events in queue if not empty
    while (!m_Queues[activeQueue].empty())
    {
        // pop the front of queue
        std::shared_ptr<IEventData> pEvent = m_Queues[activeQueue].front();
        m_Queues[activeQueue].pop_front();

        const EventType& eventType = pEvent->GetEventType();
        
        // find all delegate functions registered with this event
        auto findIt = m_EventListeners.find(eventType);
        if (findIt != m_EventListeners.end())
        {
            const EventListenerList &eventListeners = findIt->second;
            // call each event
            for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
            {
                EventListenerDelegate listener = (*it);
                listener(pEvent);
            }
        }
    }
    return true;
}

void EventManager::Clear()
{
    m_Queues[0].clear();
    m_Queues[1].clear();
}