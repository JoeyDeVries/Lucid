#include "RootNode.h"
#include "Scene.h"
#include "../Resources/ResourceManager.h"

#include <algorithm>

RootNode::RootNode() : SceneNode(0, "root", "ROOT", glm::vec2(), 0)
{
}

void RootNode::Initialize(Scene *scene)
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

void RootNode::PreRender(Scene *scene)
{
    // empty
}

// sorting predicate used for sorting the scene nodes
bool SortSceneNodes(std::shared_ptr<ISceneNode> a, std::shared_ptr<ISceneNode> b)
{
	return a->GetDepth() < b->GetDepth();
}

void RootNode::Render(Scene *scene)
{
	// first sort children before rendering based on depth (shader|material)
	std::sort(m_Children.begin(), m_Children.end(), SortSceneNodes);
	
	// Update lighting parameters
	scene->GetLightManager()->UpdateShader(scene, ResourceManager::GetInstance()->GetShader("sprite"));

    // configure all render passes here
    // - render all children into post-processing framebuffer
    scene->GetRenderer()->PrePostProcessRender();
        RenderChildren(scene);
    scene->GetRenderer()->PostPostProcessRender();
    // - render quad with post-processing effect enabled
    scene->GetRenderer()->PostProcessQuadRender();
}

void RootNode::PostRender(Scene *scene)
{
    // empty
}