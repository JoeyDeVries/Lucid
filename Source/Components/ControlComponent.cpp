#include "ControlComponent.h"

#include "../Application/GameApplication.h" // Should eventually be done via events to remove this dependancy

#include <iostream>
#include "../FastDelegate.h"

ControlComponent::ControlComponent()
{

}

ControlComponent::~ControlComponent()
{
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &ControlComponent::Test);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener, Event_DestroyActor::s_EventType);
}

bool ControlComponent::VInit(void)
{
    m_Velocity = 100.0;
    // register a callback function
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &ControlComponent::Test);
    GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_DestroyActor::s_EventType);
    return true;
}

void ControlComponent::VUpdate(float deltaTime)
{
    // Gets input from resourcemanager (TODO: later manage input via events, to remove dependancy on ResourceManager: listen to keypresses/releases)
    if(GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_D))
        m_Owner->Position().x += m_Velocity * deltaTime;
    if(GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_A))
        m_Owner->Position().x -= m_Velocity * deltaTime;
    // Since position has changed, change corresponding SceneNode as well 
    // TODO: Send an event that position has changed, let SceneNode subscribe to these events with this ActorID!
    // TODO: Before sending out event, be sure to check Physics component if movement is allowed! otherwise do nothing
    GameApplication::GetInstance()->GetScene()->FindActor(m_Owner->GetID())->SetPosition(m_Owner->Position());
}

void ControlComponent::SetVelocity(float velocity)
{
    m_Velocity = velocity;
}

void ControlComponent::Test(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_DestroyActor> evtDestroyActor = std::dynamic_pointer_cast<Event_DestroyActor>(eventData);
    std::cout << "EVENT: ActorDestroyed. Destroyed ActorID:" << evtDestroyActor->GetActorID() << " .. Received ActorID: " << m_Owner->GetID() << std::endl;
}

