#include "LightNode.h"
#include "../Application/GameApplication.h"


LightNode::LightNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position,  glm::vec3 diffuse, glm::vec3 specular, float attenuation)
    : SceneNode(ActorID, name, renderPass, position, 0, glm::vec2(20.0), 0.0f)
{
    m_Diffuse = diffuse;
    m_Specular = specular;
    m_Attenuation = attenuation;
}

void LightNode::Initialize(Scene *scene)
{
     // update projection matrix 
    if(m_Material.GetShader())
        m_Material.GetShader()->SetMatrix4("projection", scene->GetCamera()->GetProjection(), true);
 
    // SceneNode::Initialize(scene); // not necessary: lights will not have any children
}

void LightNode::Render(Scene *scene)
{
    // Render stuff here (could do basic sprite rendering here; and later propagate it to subclasses)
    //m_Material.GetShader()->Use();
    //m_Material.GetShader()->SetMatrix4("model", m_Model);
    ////m_Material.GetShader()->SetMatrix4("view", scene->Camera()->GetView());
    //m_Material.PreRender();
    //scene->GetRenderer()->RenderQuad();
}
