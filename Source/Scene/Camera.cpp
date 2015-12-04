#include "Camera.h"

Camera::Camera()
{
    m_Projection = glm::mat4();
    m_View = glm::mat4();
}


void Camera::SetProjection(float width, float height, float near, float far)
{
    this->width = width;
    this->height = height;
    m_Projection = glm::ortho(0.0f, width, height, 0.0f, near, far);
}

void Camera::CalculateViewMatrix()
{
    // Simple approach: calculate the amount of translation based on the target's position (leaving him always in center)
    // TODO: only translate when character outside of some specified bounding box
    glm::vec2 targetPos   = m_Target->GetPosition();
    glm::vec2 targetScale = m_Target->GetScale();
    glm::mat4 view;
    view = glm::translate(view, -glm::vec3(targetPos * targetScale, 0.0));
    m_View = view;
}

