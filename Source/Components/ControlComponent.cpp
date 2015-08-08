#include "ControlComponent.h"

#include "../Application/GameApplication.h" // Should eventually be done via events to remove this dependancy

#include <iostream>
#include "../FastDelegate.h"

ControlComponent::ControlComponent() : m_Velocity(0.0), m_IsJumping(false) 
{

}

ControlComponent::~ControlComponent()
{
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &ControlComponent::Test);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener, Event_DestroyActor::s_EventType);
}

bool ControlComponent::VInit(void)
{
    m_Velocity = 750.0;
    // register a callback function
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &ControlComponent::Test);
    GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_DestroyActor::s_EventType);
    return true;
}

void ControlComponent::VUpdate(float deltaTime)
{
    glm::vec2 bodyCenter = m_Owner->GetPosition() + m_Owner->GetScale() * 0.5f;
    // Gets input from resourcemanager (TODO: later manage input via events, to remove dependancy on ResourceManager: listen to keypresses/releases)
    if(GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_D))
        GameApplication::GetInstance()->GetPhysics()->ApplyForce(m_Owner->GetID(), glm::vec2(50.0, 0.0) * m_Velocity * deltaTime, bodyCenter);
        //m_Owner->GetPosition().x += m_Velocity * deltaTime;
    if(GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_A))
        GameApplication::GetInstance()->GetPhysics()->ApplyForce(m_Owner->GetID(), glm::vec2(-50.0, 0.0) * m_Velocity * deltaTime, bodyCenter);
    static int jumpCounter;
    if (GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE) && !m_IsJumping)
    {
        GameApplication::GetInstance()->GetPhysics()->ApplyImpulse(m_Owner->GetID(), glm::vec2(0.0, -1500.0), bodyCenter);
        m_IsJumping = true;
        jumpCounter = 5000;
    }
    if(jumpCounter > 0)
        jumpCounter--;
    else
        m_IsJumping = false;
    //m_Owner->GetPosition().x -= m_Velocity * deltaTime;
    // Since position has changed, change corresponding SceneNode as well 
    // TODO: Send an event that position has changed, let SceneNode subscribe to these events with this ActorID!
    // TODO: Before sending out event, be sure to check Physics component if movement is allowed! otherwise do nothing
    GameApplication::GetInstance()->GetScene()->FindActor(m_Owner->GetID())->SetPosition(m_Owner->GetPosition());
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

