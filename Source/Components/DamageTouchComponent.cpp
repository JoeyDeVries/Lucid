#include "DamageTouchComponent.h"
#include "LifeComponent.h"
#include "../Application/GameApplication.h"
#include "../Physics/Event_PostCollisionAdd.h"

DamageTouchComponent::DamageTouchComponent() : m_DamageAmount(0), m_TimeSinceLastDamage(0.0f)
{

}

DamageTouchComponent::~DamageTouchComponent()
{
    // de-register event
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &DamageTouchComponent::OnPostCollisionAdd);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener, Event_PostCollisionAdd::s_EventType);
}

int DamageTouchComponent::GetDamageAmount()
{
    return m_DamageAmount;
}
void DamageTouchComponent::SetDamageAmount(int amount)
{
    m_DamageAmount = amount;
}

bool DamageTouchComponent::VInit()
{
    // register event listeners
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &DamageTouchComponent::OnPostCollisionAdd);
    GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_PostCollisionAdd::s_EventType);
    return true;
}

void DamageTouchComponent::VUpdate(float deltaTime)
{
    // make sure damage ticks are only once per second
    if(m_TimeSinceLastDamage >= 0.0f)
        m_TimeSinceLastDamage -= deltaTime;
}

void DamageTouchComponent::OnPostCollisionAdd(std::shared_ptr<IEventData> eventData)
{
    if (m_TimeSinceLastDamage <= 0.0f)
    {
        std::shared_ptr<Event_PostCollisionAdd> pEvent = std::dynamic_pointer_cast<Event_PostCollisionAdd>(eventData);
        if (pEvent)
        {
            std::shared_ptr<Actor> player = GameApplication::GetInstance()->GetImportantActor("player");
            const b2Body *currBody = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
            const b2Body *playerBody = GameApplication::GetInstance()->GetPhysics()->FindBody(player->GetID());
            if (GameApplication::GetInstance()->GetPhysics()->IsBodiesColliding(currBody, playerBody))
            {   // player has been damaged, get life component; deduce life (component will send event if player died)
                std::weak_ptr<LifeComponent> pWeakComponent = player->GetComponent<LifeComponent>("Life");
                std::shared_ptr<LifeComponent> pComponent(pWeakComponent);
                if (pComponent)
                {
                    pComponent->Damage(m_DamageAmount);
                    // also put delay on next damage tick
                    m_TimeSinceLastDamage = 1.0f;
                }
            }
        }
    }
}

