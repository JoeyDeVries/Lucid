#include "LightNode.h"
#include "../Application/GameApplication.h"
#include "../Resources/ResourceManager.h"


LightNode::LightNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, float depth, glm::vec2 scale, glm::vec3 diffuse, glm::vec3 specular, float attenuation)
    : SpriteNode(ActorID, name, renderPass, position, depth, scale, 0.0f)
{
    m_Diffuse = diffuse;
    m_Specular = specular;
    m_Attenuation = attenuation;
}

bool LightNode::IsVisible(Scene *scene) const
{
	return true;
}
