#ifndef ACTOR_COMPONENT_H
#define ACTOR_COMPONENT_H
#include <memory>
#include "ActorFactory.h"
#include "Actor.h"

class Actor;

class ActorComponent
{
    friend class ActorFactory;
// DATA
public:
    std::string m_type;
protected:
    std::shared_ptr<Actor> m_owner;
// FUNCTIONS
public:
    virtual ~ActorComponent(void) { }
    virtual bool VInit() = 0;
    virtual void VUpdate(float delta) { }
private:
    void setOwner(std::shared_ptr<Actor> actor) { m_owner = actor; }
};

#endif