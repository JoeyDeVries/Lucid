#include "AIComponent.h"
#include <iostream>
#include "../Application/GameApplication.h"
#include "MoveLoopComponent.h"

AIComponent::AIComponent() : m_AttackRadius(0.0f), m_BeginPosition(0.0f), m_EndPosition(0.0f)
{

}

AIComponent::~AIComponent()
{

}

float AIComponent::GetAttackRadius()
{
    return m_AttackRadius;
}
glm::vec2 AIComponent::GetBeginPosition()
{
    return m_BeginPosition;
}
glm::vec2 AIComponent::GetEndPosition()
{
    return m_EndPosition;
}
bool AIComponent::IsAttacking()
{
    return m_Attacking;
}

void AIComponent::SetAttackRadius(float radius)
{
    m_AttackRadius = radius;
}
void AIComponent::SetBeginPosition(glm::vec2 position)
{
    m_BeginPosition = position;
}
void AIComponent::SetEndPosition(glm::vec2 position)
{
    m_EndPosition = position;
}
void AIComponent::SetAttacking(bool attacking)
{
    m_Attacking = attacking;
}

bool AIComponent::VInit()
{
    return true;
}

void AIComponent::VUpdate(float deltaTime)
{
    std::shared_ptr<Actor> player = GameApplication::GetInstance()->GetImportantActor("player");

    glm::vec2 difference = player->GetCenter() - m_Owner->GetCenter();
    float distance = glm::length(difference);
    if (distance <= m_AttackRadius)
    {
        if (!m_Attacking)
        {   // first time player is spotted
            m_Attacking = true;
            // check if the actor has a moveloop component, and disable it during player vicinity
            std::cout << "Start attacking player" << std::endl;
            std::weak_ptr<MoveLoopComponent> pWeak = m_Owner->GetComponent<MoveLoopComponent>("MoveLoop");
            if (!pWeak.expired())
            {
                std::shared_ptr<MoveLoopComponent> pComponent(pWeak);
                pComponent->Pause();
            }
            // then move to player
            glm::vec2 force = glm::normalize(difference) * glm::vec2(100.0f) * deltaTime;
            GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID())->SetLinearVelocity(b2Vec2(force.x, 0.0f));
        }
    }
    else
    {
        if (m_Attacking)
        {   // player left attack vicinity
            m_Attacking = false;
            std::cout << "stop attacking player" << std::endl;
            // resume normal loop movement
            std::weak_ptr<MoveLoopComponent> pWeak = m_Owner->GetComponent<MoveLoopComponent>("MoveLoop");
            if (!pWeak.expired())
            {
                std::shared_ptr<MoveLoopComponent> pComponent(pWeak);
                pComponent->Resume();
            }
        }
    }

    
}
