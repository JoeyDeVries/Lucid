#ifndef EVENT_START_LEVEL_H
#define EVENT_START_LEVEL_H

#include "../Communication/IEventData.h"

class Event_StartLevel : public IEventData
{
private:
    std::string m_LevelPath;
public:
    static const EventType s_EventType;
    Event_StartLevel(std:: string level) : m_LevelPath(level) { }

    virtual const EventType& GetEventType() { return s_EventType; }
    virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_StartLevel(m_LevelPath)); }
    virtual const std::string GetName() const { return "Event_LevelPath"; }
    virtual const std::string GetLevelPath() const { return m_LevelPath; }
};

#endif