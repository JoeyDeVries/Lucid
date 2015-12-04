#ifndef LIGHT_SWITCH_COMPONENT_H
#define LIGHT_SWITCH_COMPONENT_H
#include "ActorComponent.h"
#include "Event_LightStateSwitched.h"



class LightSwitchComponent : public ActorComponent
{
private:
	LightState m_LightState;
public:
	LightSwitchComponent();
	~LightSwitchComponent();

	bool VInit();
	void VUpdate(float deltaTime);	
};

#endif