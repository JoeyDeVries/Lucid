#ifndef LIFE_COMPONENT_H
#define LIFE_COMPONENT_H
#include "ActorComponent.h"

class LifeComponent : public ActorComponent
{
private:
    int m_CurrentHealth;
    int m_MaxHealth;
public:
    LifeComponent();
    ~LifeComponent();

    int GetCurrentHealth();
    int GetMaxHealth();
    void SetCurrentHealth(int health);
    void SetMaxHealth(int health);

    bool VInit();
    void VUpdate(float deltaTime);

    void Heal(int amount);
    void Damage(int amount);
};

#endif