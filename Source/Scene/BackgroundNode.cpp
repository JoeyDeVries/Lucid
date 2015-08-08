#include "BackgroundNode.h"

#include "../Application/GameApplication.h"

BackgroundNode::BackgroundNode(unsigned int ActorID) : SceneNode(ActorID, "Background", "BACKGROUND", glm::vec2(0.0), 9.0f)
{

}

void BackgroundNode::CalculateModel()
{
    // Only do scaling operations, no need to translate/position for background (except depth)
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0, 0.0, -m_Depth));
    model = glm::scale(model, glm::vec3(m_Scale, 1.0));
    m_Model = model;
}

void BackgroundNode::Initialize(Scene *scene)
{
    SetScale(glm::vec2(GameApplication::GetInstance()->ScreenWidth(), GameApplication::GetInstance()->ScreenHeight()));
}

void BackgroundNode::Render(Scene *scene)
{
    // Render background
    m_Material.GetShader()->Use();
    m_Material.GetShader()->SetMatrix4("model", m_Model);
    //m_Material.GetShader()->SetMatrix4("view", scene->Camera()->GetView());
    m_Material.PreRender();
    scene->GetRenderer()->RenderQuad();
}