#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H
#include "ActorComponent.h"
#include <glm/glm.hpp>
#include <memory>

class AIComponent : public ActorComponent
{
private:
    // the radius at which the AI will attack the player
    float m_AttackRadius;
    // its maximum start position to move while walking/attacking (often coordinated with MoveLoopComponent)
    glm::vec2 m_BeginPosition;
    // its maximum end position to move while walking/attack
    glm::vec2 m_EndPosition;
    // denotes whether at this point of time the player is in its vicinity
    bool m_Attacking;
public:
    AIComponent();
    ~AIComponent();

    float GetAttackRadius();
    glm::vec2 GetBeginPosition();
    glm::vec2 GetEndPosition();
    bool IsAttacking();

    void SetAttackRadius(float radius);
    void SetBeginPosition(glm::vec2 position);
    void SetEndPosition(glm::vec2 position);
    void SetAttacking(bool attacking);

    bool VInit() override;
    void VUpdate(float deltaTime) override;
};

#endif