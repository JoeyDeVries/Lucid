#ifndef CONTROL_COMPONENT_H
#define CONTROL_COMPONENT_H
// any actor that has this control component attached is controlled via user input
// a similar component can be developed for AI control


#include "ActorComponent.h"


class ControlComponent : public ActorComponent
{
private:
    float m_Velocity;
public:
    ControlComponent();
    bool VInit(void);
    void VUpdate(float deltaTime);

    void SetVelocity(float velocity);
};

#endif