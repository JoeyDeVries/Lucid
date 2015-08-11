#include "RootNode.h"
#include "Scene.h"


RootNode::RootNode() : SceneNode(0, "root", "ROOT", glm::vec2(), 0)
{
    m_Name = "root";
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

void RootNode::Render(Scene *scene)
{
    // configure all render passes here
    // - render all children into post-processing framebuffer
    RenderChildren(scene);

    // - render quad with post-processing effect enabled
}

void RootNode::PostRender(Scene *scene)
{
    // empty
}