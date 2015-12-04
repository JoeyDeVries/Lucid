#include "SceneNode.h"
#include "Scene.h"

#include "../Application/GameApplication.h"

#include <iostream>

const float PI = 3.14159265359;

SceneNode::SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale, float rotation)
{
    m_ActorID    = ActorID;
    m_Name       = name;
    m_RenderPass = renderPass;
    m_Position   = position;
    m_Depth      = depth;
    m_Scale      = scale;
    m_Rotation   = rotation;
    CalculateModel();
}

void SceneNode::CalculateModel()
{
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(m_Position, -m_Depth)); 

    model = glm::translate(model, glm::vec3(0.5f * m_Scale.x, 0.5f * m_Scale.y, 0.0f)); // Move origin of rotation to center of quad
    model = glm::rotate(model, m_Rotation * 180.0f/PI, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
    model = glm::translate(model, glm::vec3(-0.5f * m_Scale.x, -0.5f * m_Scale.y, 0.0f)); // Move origin back

    model = glm::scale(model, glm::vec3(m_Scale, 1.0));

    m_Model = model;
}

void SceneNode::Initialize(Scene * scene)
{
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
        if ((*begin)->GetActorID() == actorID)
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