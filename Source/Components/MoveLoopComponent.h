#ifndef MOVE_LOOP_COMPONENT_H
#define MOVE_LOOP_COMPONENT_H
#include "ActorComponent.h"
#include <glm/glm.hpp>

class MoveLoopComponent : public ActorComponent
{
private:
    glm::vec2 m_BeginPosition;
    glm::vec2 m_EndPosition;
    float m_Speed;
    float m_CurrentPosition;
    bool m_Forward;
    bool m_Paused;
public:
    MoveLoopComponent();
    ~MoveLoopComponent();

    glm::vec2 GetBeginPosition();
    glm::vec2 GetEndPosition();
    float GetSpeed();

    void SetBeginPosition(glm::vec2 position);
    void SetEndPosition(glm::vec2 position);
    void SetSpeed(float speed);

    // allow other components/controls to temporarily override the moveloop component with custom movement
    void Pause();
    void Resume();

    bool VInit();
    void VUpdate(float deltaTime);
};

#endif