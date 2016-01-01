#ifndef EVENT_START_LEVEL_H
#define EVENT_START_LEVEL_H

#include "../Communication/IEventData.h"

// fires whenever a (new) level should be started/loaded
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