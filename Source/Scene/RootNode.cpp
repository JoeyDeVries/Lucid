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

void RootNode::Render(Scene *scene, Renderer *renderer)
{
	// first sort children before rendering based on depth (shader|material)
	std::sort(m_Children.begin(), m_Children.end(), SortSceneNodes);
	
	// Update lighting parameters
	scene->GetLightManager()->UpdateShader(scene, ResourceManager::GetInstance()->GetShader("sprite"));

    // configure all render passes here
    // - render all children into post-processing framebuffer
    renderer->PrePostProcessRender();
        RenderChildren(scene, renderer);
    renderer->PostPostProcessRender();
    // - render quad with post-processing effect enabled
    renderer->PostProcessQuadRender();
}

void RootNode::RenderChildren(Scene *scene, Renderer *renderer)
{
    SceneNodeList::iterator begin = m_Children.begin();
    SceneNodeList::iterator end = m_Children.end();
    bool state_block_pass = false; // check when we're rendering state blocks; as they're all in a unique depth layer we only switch state once
    while (begin != end)
    {
        (*begin)->PreRender(scene);

        if ((*begin)->GetName() == "state_block")
        {
            if (!state_block_pass)
            { // this is the first time we hit a state block, switch state once
                state_block_pass = true;
                scene->GetLightManager()->FocusOnLightType("lantern"); // light blocks should only be rendered by lantern light only
                scene->GetLightManager()->UpdateShader(scene, (*begin)->GetMaterial()->GetShader());
            }
        }
        else if(state_block_pass)
        { // we're no longer in state block pass, reset original state
            state_block_pass = false;
            scene->GetLightManager()->RemoveFocus();
            scene->GetLightManager()->UpdateShader(scene, (*begin)->GetMaterial()->GetShader()); // assuming next material uses the same shaders
        }

        if ((*begin)->IsVisible(scene))
        {
            (*begin)->Render(scene, renderer);
            (*begin)->RenderChildren(scene, renderer);
        }

        (*begin)->PostRender(scene);
        ++begin;
    }
}

void RootNode::PostRender(Scene *scene)
{
    // empty
}