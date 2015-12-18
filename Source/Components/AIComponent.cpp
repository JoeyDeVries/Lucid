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


// local variables for VUpdate-only state logic
glm::vec2 oldPos;
int restoreIterations = 0;
// -------------------------------------------
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
            oldPos = m_Owner->GetPosition();
            // check if the actor has a moveloop component, and disable it during player vicinity
            std::cout << "Start attacking player" << std::endl;
            std::weak_ptr<MoveLoopComponent> pWeak = m_Owner->GetComponent<MoveLoopComponent>("MoveLoop");
            if (!pWeak.expired())
            {
                std::shared_ptr<MoveLoopComponent> pComponent(pWeak);
                pComponent->Pause();
            }         
        }
        // move to player
        glm::vec2 force = glm::normalize(difference) * glm::vec2(1500.0f) * deltaTime;
        GameApplication::GetInstance()->GetPhysics()->SetLinearVelocity(m_Owner->GetID(), force);
        std::cout << "(" << force.x << ", " << force.y << ")" << std::endl;
    }
    else
    {
        if (m_Attacking)
        {   // player left attack vicinity
            m_Attacking = false;
            std::cout << "stop attacking player" << std::endl;
            
            // intitiate restore operations before resuming move loop control
            restoreIterations = 60; // restore in 1 frame
        }
        // if restoring, slowly move owner back to original position before attacking player
        if (restoreIterations > 0)
        {
            glm::vec2 lerp = glm::mix(m_Owner->GetPosition(), oldPos, 0.05f);
            glm::vec2 diff = lerp - m_Owner->GetPosition();
            GameApplication::GetInstance()->GetPhysics()->SetLinearVelocity(m_Owner->GetID(), diff * 40.0f);
            restoreIterations--;
            if (restoreIterations == 0)
            { // if a loop componenti s present, resume normal loop movement             
                std::weak_ptr<MoveLoopComponent> pWeak = m_Owner->GetComponent<MoveLoopComponent>("MoveLoop");
                if (!pWeak.expired())
                {
                    std::shared_ptr<MoveLoopComponent> pComponent(pWeak);
                    pComponent->Resume();
                }
            }
        }
    }

    
}
