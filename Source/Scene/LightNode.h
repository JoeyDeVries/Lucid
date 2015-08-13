#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H
#include "SceneNode.h"
#include <glm/glm.hpp>

class LightNode : public SceneNode
{
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;
    float m_Attenuation;
public:
    LightNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, glm::vec3 diffuse, glm::vec3 specular, float attenuation);

    glm::vec3& const GetDiffuse()     { return m_Diffuse; }
    glm::vec3& const GetSpecular()    { return m_Specular; }
    float&     const GetAttenuation() { return m_Attenuation; }

    virtual void Initialize(Scene *scene);
    virtual void Render(Scene *scene);
};

#endif