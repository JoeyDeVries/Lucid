#ifndef STATE_BLOCK_COMPONENT_H
#define STATE_BLOCK_COMPONENT_H
#include "ActorComponent.h"
#include "Event_LightStateSwitched.h"

class StateBlockComponent : public ActorComponent
{
private:
	LightState m_BlockColor;
	LightState m_LastLightState;
	bool m_PlayerContact;

public:
	StateBlockComponent();
	~StateBlockComponent();

	virtual bool VInit();
	virtual void VUpdate(float delta);

	void SetBlockColor(LightState blockColor);

	void OnLightSwitched(std::shared_ptr<IEventData> eventData);
	void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
	void OnPostCollisionRemove(std::shared_ptr<IEventData> eventData);
};


#endif