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
//
//void LightNode::Initialize(Scene *scene)
//{ 
//     // update projection matrix 
//    if(m_Material)
//	{
//		// set default light texture
//		m_Material->SetDiffuse(ResourceManager::GetInstance()->GetTexture("light"));
//		m_Material->Initialize();
//        m_Material->GetShader()->SetMatrix4("projection", scene->GetCamera()->GetProjection());
//	}
//
//   // SceneNode::Initialize(scene); // not necessary: lights will not have any children
//}
//
//void LightNode::Render(Scene *scene)
//{
//    // Render stuff here (could do basic sprite rendering here; and later propagate it to subclasses)
//	if (m_Material)
//	{
//		m_Material->GetShader()->Use();
//		m_Material->GetShader()->SetMatrix4("model", m_Model);
//		//m_Material.GetShader()->SetMatrix4("view", scene->Camera()->GetView());
//		m_Material->PreRender();
//		scene->GetRenderer()->RenderQuad();
//	}
//}
