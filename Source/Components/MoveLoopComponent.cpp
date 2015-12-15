#include "MoveLoopComponent.h"
#include "../Application/GameApplication.h"

#include <iostream>

MoveLoopComponent::MoveLoopComponent() : m_BeginPosition(0.0f), m_EndPosition(0.0f), m_Speed(0.25f), m_CurrentPosition(0.0f), m_Forward(true)
{

}

MoveLoopComponent::~MoveLoopComponent()
{

}

glm::vec2 MoveLoopComponent::GetBeginPosition()
{
    return m_BeginPosition;
}
glm::vec2 MoveLoopComponent::GetEndPosition()
{
    return m_EndPosition;
}
float MoveLoopComponent::GetSpeed()
{
    return m_Speed;
}

void MoveLoopComponent::SetBeginPosition(glm::vec2 position)
{
    m_BeginPosition = position;
}
void MoveLoopComponent::SetEndPosition(glm::vec2 position)
{
    m_EndPosition = position;
}
void MoveLoopComponent::SetSpeed(float speed)
{
    m_Speed = speed;
}

bool MoveLoopComponent::VInit()
{
    return true;
}

void MoveLoopComponent::VUpdate(float deltaTime)
{
    // first check if we need to switch movement direction
    if(m_CurrentPosition >= 1.0f || m_CurrentPosition <= 0.0f)
        m_Forward = !m_Forward;

    // lerp between begin and end position and reverse if end position is reached (get current pos from m_Owner)
    if(m_Forward)
        m_CurrentPosition += deltaTime * m_Speed;
    else
        m_CurrentPosition -= deltaTime * m_Speed;
    glm::vec2 newPos = glm::mix(m_BeginPosition, m_EndPosition, std::max(std::min(m_CurrentPosition, 1.0f), 0.0f));
    glm::vec2 diff = newPos - m_Owner->GetPosition();

    GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID())->SetLinearVelocity(b2Vec2(diff.x, diff.y));
}