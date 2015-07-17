#include "Scene.h"


Scene::Scene()
{
    m_Root.reset(new SceneNode(0, "root", "NONE", glm::mat4()));

}

Scene::~Scene()
{

}

void Scene::Restore()
{
    if(m_Root)
        m_Root->Restore(this);
}

void Scene::Update(float deltaTime)
{
    if(!m_Root)
        return;

    m_Root->Update(this, deltaTime);

}

void Scene::Render()
{
   /* if (m_Root && m_Camera)
    {

    }*/
}

std::shared_ptr<ISceneNode> Scene::FindActor(unsigned int ActorID)
{
    SceneActorMap::iterator i = m_ActorMap.find(ActorID);
    if (i == m_ActorMap.end())
    {
        return std::shared_ptr<ISceneNode>();
    }
    return i->second;
}

bool Scene::AddChild(unsigned int ActorID, std::shared_ptr<ISceneNode> child)
{
    m_ActorMap[ActorID] = child;
    // cast child into different node types and add to other managers if necessary (like if it is a light node, then add it to light manager)
    // [...]
    // otherwise add it as child to Root node
    return m_Root->AddChild(child);
}

bool Scene::RemoveChild(unsigned int ActorID)
{
    std::shared_ptr<ISceneNode> kid = FindActor(ActorID);
    // Check if it is any of the different node types (like light) and act accordingly)
    // [..]
    // Otherwise, erase as normal
    m_ActorMap.erase(ActorID);
    return m_Root->RemoveChild(ActorID);
}

void Scene::PushAndSetMatrix(const glm::mat4 model)
{

}

void Scene::PopMatrix()
{

}

const glm::mat4& Scene::GetTopMatrix()
{
    return glm::mat4();
}
