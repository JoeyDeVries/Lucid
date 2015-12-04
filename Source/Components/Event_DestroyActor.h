#ifndef DESTROY_ACTOR_H
#define DESTROY_ACTOR_H

#include "../Communication/IEventData.h"

class Event_DestroyActor : public IEventData
{
private:
    ActorID m_ActorID;
public:
    static const EventType s_EventType;
    Event_DestroyActor(ActorID actorID) : m_ActorID(actorID) { }

    virtual const EventType& GetEventType() { return s_EventType; } 
    virtual std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_DestroyActor(m_ActorID)); } 
    virtual const std::string GetName() const { return "Event_DestroyActor"; } 
    const ActorID GetActorID() const { return m_ActorID; } 

};

#endif