#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "ActorComponent.h"


class TransformComponent : public ActorComponent
{
// DATA
public:
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::vec3 Rotation;
    GLfloat RotationAngle;
// FUNCTIONS
public:
    TransformComponent(void);
    bool VInit(void);
    //void VUpdate(float delta);
};

#endif