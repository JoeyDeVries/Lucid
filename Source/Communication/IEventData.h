#ifndef I_EVENT_DATA_H
#define I_EVENT_DATA_H
#include "../FastDelegate.h"
#include <string>
#include <memory>

// don't use global enum of event types but use Global UID's attached to each specific event type
// this way there is no need to recompile a large array of files for every event type change
// {1E97F18B-B2EE-40C9-9A68-AE5B9481C914} TODO for each event; pick first number as EventType
// DEFINE_GUID(<<name>>, 
// 0x1e97f18b, 0xb2ee, 0x40c9, 0x9a, 0x68, 0xae, 0x5b, 0x94, 0x81, 0xc9, 0x14);
// Generated via: Tools -> Create GUID -> DEFINE ... -> Copy
typedef unsigned int EventType;
typedef unsigned int ActorID;
class IEventData;
typedef fastdelegate::FastDelegate1<std::shared_ptr<IEventData>> EventListenerDelegate;

class IEventData
{
private:
    const float m_TimeStamp;
public:
    IEventData(const float timestamp = 0.0) : m_TimeStamp(timestamp) { }
    ~IEventData() { }
    virtual const EventType& GetEventType() = 0;
    virtual float GetTimeStamp() { return m_TimeStamp; }
    virtual std::shared_ptr<IEventData> Copy() const = 0;
    virtual const std::string GetName() const = 0;
};

#endif