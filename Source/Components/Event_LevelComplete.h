#ifndef EVENT_LEVEL_COMPLETE_H
#define EVENT_LEVEL_COMPLETE_H
#include "../Communication/IEventData.h"

class Event_LevelComplete : public IEventData
{
private:
public:
	static const EventType s_EventType;
	Event_LevelComplete() { }

	const EventType& GetEventType() { return s_EventType; }
	std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_LevelComplete()); }
	const std::string GetName() const { return "Event_LevelComplete"; }
};

#endif