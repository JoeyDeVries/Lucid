#include "MoveLoopComponent.h"
#include "../Application/GameApplication.h"

MoveLoopComponent::MoveLoopComponent() : m_BeginPosition(0.0f), m_EndPosition(0.0f)
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

void MoveLoopComponent::SetBeginPosition(glm::vec2 position)
{
    m_BeginPosition = position;
}
void MoveLoopComponent::SetEndPosition(glm::vec2 position)
{
    m_EndPosition = position;
}

bool VInit()
{
    return true;
}

void VUpdate(float deltaTime)
{
    // lerp between begin and end position and reverse if end position is reached (get current pos from m_Owner)
}