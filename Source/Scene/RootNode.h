#ifndef ROOT_NODE_H
#define ROOT_NODE_H

#include "SceneNode.h"


class RootNode : public SceneNode
{
    RootNode(std::string name);

    virtual void Initialize(Scene *scene);
    virtual void PreRender(Scene *scene);
    virtual void Render(Scene *scene);
    virtual void PostRender(Scene *scene);
};

#endif