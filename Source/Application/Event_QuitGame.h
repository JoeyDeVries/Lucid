#ifndef EVENT_QUIT_GAME_H
#define EVENT_QUIT_GAME_H

#include "../Communication/IEventData.h"

// Fires whenever the game should be closed
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