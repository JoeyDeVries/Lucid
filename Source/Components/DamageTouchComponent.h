#ifndef DAMAGE_TOUCH_COMPONENT_H
#define DAMAGE_TOUCH_COMPONENT_H
#include "ActorComponent.h"
#include "../Communication/IEventData.h"
#include <memory>

class DamageTouchComponent : public ActorComponent
{
private:
    int m_DamageAmount;
    float m_TimeSinceLastDamage;
public:
    DamageTouchComponent();
    ~DamageTouchComponent();

    int GetDamageAmount();
    void SetDamageAmount(int amount);

    bool VInit();
    void VUpdate(float deltaTime);

    void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
};

#endif