#include "SceneNode.h"

#include "Scene.h"

SceneNode::SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, glm::vec2 scale, float rotation)
{
    m_ActorID = ActorID;
    m_Name = name;
    m_RenderPass = renderPass;
    m_Position = position;
    m_Scale = scale;
    m_Rotation = rotation;
    CalculateModel();
}

void SceneNode::CalculateModel()
{
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(m_Position, 0.0));

    model = glm::translate(model, glm::vec3(0.5f * m_Scale.x, 0.5f * m_Scale.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * m_Scale.x, -0.5f * m_Scale.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(m_Scale, 1.0));

    m_Model = model;
}

void SceneNode::Initialize(Scene * scene)
{
    // update projection matrix 
    if(m_Material.GetShader())
        m_Material.GetShader()->SetMatrix4("projection", scene->GetCamera()->GetProjection(), true);

    // do the same for all children
    SceneNodeList::iterator begin = m_Children.begin();
    SceneNodeList::iterator end   = m_Children.end();
    while (begin != end)
    {
        (*begin)->Initialize(scene);
        ++begin;
    }
}

void SceneNode::Restore(Scene *scene)
{
    SceneNodeList::iterator begin = m_Children.begin();
    SceneNodeList::iterator end   = m_Children.end();
    while (begin != end)
    {
        (*begin)->Restore(scene);
        ++begin;
    }
}

void SceneNode::Update(Scene *scene, float deltaTime)
{
    SceneNodeList::iterator begin = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (begin != end)
    {
        (*begin)->Update(scene, deltaTime);
        ++begin;
    }
}

bool  SceneNode::IsVisible(Scene *scene) const
{
    return true;
}

void  SceneNode::PreRender(Scene *scene)
{
    scene->PushAndSetMatrix(m_Model);
}

void  SceneNode::Render(Scene *scene)
{
    // Render stuff here (could do basic sprite rendering here; and later propagate it to subclasses)
    m_Material.GetShader()->Use();
    m_Material.GetShader()->SetMatrix4("model", m_Model);
    //m_Material.GetShader()->SetMatrix4("view", scene->Camera()->GetView());
    m_Material.PreRender();
    scene->GetRenderer()->RenderQuad();
}

void  SceneNode::PostRender(Scene *scene)
{
    scene->PopMatrix();
}

bool  SceneNode::AddChild(std::shared_ptr<ISceneNode> child)
{
    m_Children.push_back(child);
    
    // Re-update radius of current node's sphere based on child's radius
    // [...]
    return true;
}

bool  SceneNode::RemoveChild(unsigned int actorID)
{
    SceneNodeList::iterator begin = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (begin != end)
    {
        if ((*begin)->ActorID() == actorID)
        {
            begin = m_Children.erase(begin); // also updates iterator
            return true;
        }
    }
    return false;
}

void  SceneNode::RenderChildren(Scene *scene)
{
    SceneNodeList::iterator begin = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    while (begin != end)
    {
        (*begin)->PreRender(scene);

        if ((*begin)->IsVisible(scene))
        {
            (*begin)->Render(scene);
            (*begin)->RenderChildren(scene);
        }

        (*begin)->PostRender(scene);
        ++begin;
    }
}