#ifndef MOVE_LOOP_COMPONENT_H
#define MOVE_LOOP_COMPONENT_H
#include "ActorComponent.h"
#include <glm/glm.hpp>

class MoveLoopComponent : public ActorComponent
{
private:
    glm::vec2 m_BeginPosition;
    glm::vec2 m_EndPosition;
public:
    MoveLoopComponent();
    ~MoveLoopComponent();

    glm::vec2 GetBeginPosition();
    glm::vec2 GetEndPosition();
    void SetBeginPosition(glm::vec2 position);
    void SetEndPosition(glm::vec2 position);

    bool VInit();
    void VUpdate(float deltaTime);
};

#endif