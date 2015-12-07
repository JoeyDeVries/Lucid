#include "Scene.h"
#include "../Application/GameApplication.h"
#include "../Scene/LightNode.h"

Scene::Scene()
{
    m_Renderer.reset(new Renderer);
    m_Root.reset(new RootNode);
    m_Camera.reset(new Camera);
    m_LightManager.reset(new LightManager);
	m_TextRenderer.reset(new TextRenderer);
}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	m_TextRenderer->Initialize();
    // initializes each scene node
    m_Root->Initialize(this);    
}

void Scene::Restore()
{
    if(m_Root)
        m_Root->Restore(this);
    if (m_Camera)
    {
        m_Camera->SetProjection(GameApplication::GetInstance()->ScreenWidth(), GameApplication::GetInstance()->ScreenHeight());
    }
}

void Scene::Update(float deltaTime)
{
    if(!m_Root)
        return;

    m_Root->Update(this, deltaTime);

}

void Scene::Render()
{
    if (m_Root && m_Camera)
    {
        m_Renderer->PreRender();
		m_Camera->CalculateViewMatrix();
		m_Root->Render(this);
    }
}

std::shared_ptr<ISceneNode> Scene::GetSceneNode(unsigned int ActorID)
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
    std::shared_ptr<LightNode> pLight = std::dynamic_pointer_cast<LightNode>(child);
    if (pLight)
    {   // child is a LightNode; also add a reference to the LightManager
         GetLightManager()->AddLight(pLight);  
    }
    // otherwise add it as child to Root node
    return m_Root->AddChild(child);
}

bool Scene::RemoveChild(unsigned int ActorID)
{
    std::shared_ptr<ISceneNode> kid = GetSceneNode(ActorID);
    // Check if it is any of the different node types (like light) and act accordingly)
    std::shared_ptr<LightNode> pLight = std::dynamic_pointer_cast<LightNode>(kid);
    if (pLight)
    {    // child is a LightNode; also remove reference to the LightManager
         GetLightManager()->RemoveLight(pLight);  
    }
    // Otherwise, erase as normal
    m_ActorMap.erase(ActorID);
    return m_Root->RemoveChild(ActorID);
}

void Scene::PushAndSetMatrix(const glm::mat4 model)
{
    m_MatrixStack.Push(model);
}

void Scene::PopMatrix()
{
    m_MatrixStack.Pop();
}

const glm::mat4& Scene::GetTopMatrix()
{
    return m_MatrixStack.GetTopMatrix();
}
