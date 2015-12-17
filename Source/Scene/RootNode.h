#ifndef ROOT_NODE_H
#define ROOT_NODE_H

#include "SceneNode.h"


class RootNode : public SceneNode
{
public:
    RootNode();

    virtual void Initialize(Scene *scene);
    virtual void PreRender(Scene *scene);
    virtual void Render(Scene *scene, Renderer *renderer);
    virtual void PostRender(Scene *scene);

    // override children render component to control less general render cases
    virtual void RenderChildren(Scene *scene, Renderer *renderer);
};

#endif