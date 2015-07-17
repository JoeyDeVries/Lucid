#include "SceneNode.h"


SceneNode::SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::mat4 model)
{
    m_ActorID = ActorID;
    m_Name = name;
    m_RenderPass = renderPass;
    m_Model = model;
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
    //scene->PushAndSetMatrix(m_Model);
}

void  SceneNode::Render(Scene *scene)
{
    // Render stuff here (could do basic sprite rendering here; and later propagate it to subclasses)
    // [...] (use Scene's renderer to render stuff ;D)
}

void  SceneNode::PostRender(Scene *scene)
{
    //scene->PopMatrix();
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
            (*begin)->RenderChildren(scene);
        }

        (*begin)->PostRender(scene);
        ++begin;
    }
}