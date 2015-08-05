#ifndef CAMERA_H
#define CAMERA_H
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneNode.h"

class Camera
{
protected:
    float width, height;
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    std::shared_ptr<SceneNode> m_Target;
public:
    Camera();

    void SetTarget(std::shared_ptr<SceneNode> target) { m_Target = target; }

    void SetProjection(float width, float height, float near = 0.0f, float far = 10.0f);
    void CalculateViewMatrix();

    glm::mat4 GetProjection() { return m_Projection; }
    glm::mat4 GetView() { return m_View; }
};




#endif