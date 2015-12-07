#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H
#include "SpriteNode.h"
#include <glm/glm.hpp>

class LightNode : public SpriteNode
{
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;
    float m_Attenuation;
public:
    LightNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, float depth, glm::vec2 scale, glm::vec3 diffuse, glm::vec3 specular, float attenuation);

    glm::vec3& const GetDiffuse()     { return m_Diffuse; }
    glm::vec3& const GetSpecular()    { return m_Specular; }
    float&     const GetAttenuation() { return m_Attenuation; }

	void SetDiffuse(const glm::vec3& diffuse)     { m_Diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular)   { m_Specular = specular; }
	void SetAttenuation(const float& attenuation) { m_Attenuation = attenuation; }

	bool IsVisible(Scene *scene) const;

    //virtual void Initialize(Scene *scene);
    //virtual void Render(Scene *scene);
};

#endif