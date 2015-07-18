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
    glm::vec3 targetPos = m_Target->Position();
    float targetScale = m_Target->Scale();
    glm::mat4 view;
    view = glm::translate(view, -glm::vec3(targetPos.x * targetScale, targetPos.y * targetScale, 0.0));
    m_View = view;
}
