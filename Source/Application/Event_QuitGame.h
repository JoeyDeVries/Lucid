#ifndef EVENT_QUIT_GAME_H
#define EVENT_QUIT_GAME_H

#include "../Communication/IEventData.h"

class Event_QuitGame : public IEventData
{
private:
public:
    static const EventType s_EventType;
    Event_QuitGame() { }

    virtual const EventType& GetEventType() { return s_EventType; }
    virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_QuitGame()); }
    virtual const std::string GetName() const { return "Event_QuitGame"; }

};

#endif