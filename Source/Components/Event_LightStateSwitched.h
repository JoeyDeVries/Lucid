#ifndef EVENT_LIGHT_STATE_SWITCHED_H
#define EVENT_LIGHT_STATE_SWITCHED_H
#include "../Communication/IEventData.h"

enum LightState
{
	OFF,
	WHITE,
	RED,
	GREEN,
	BLUE
};

class Event_LightStateSwitched : public IEventData
{
private:
	LightState m_LightState;
public:
	static const EventType s_EventType;
	Event_LightStateSwitched(LightState lightState) : m_LightState(lightState) { }

	const EventType& GetEventType() { return s_EventType; }
	std::shared_ptr<IEventData> Copy() const { return std::shared_ptr<IEventData>(new Event_LightStateSwitched(m_LightState)); }
	const std::string GetName() const { return "Event_LightStateSwitched"; }
	const LightState GetLightState() const { return m_LightState; }
};

#endif