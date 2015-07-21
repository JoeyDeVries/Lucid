#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H
#include <map>
#include <string>

#include "ActorComponentDefinitions.h"
#include "Actor.h"
#include "ActorComponent.h"


// definitions
typedef ActorComponent *(*ActorComponentCreator)(void);
typedef std::map<std::string, ActorComponentCreator> actorComponentCreatorMap;



class ActorFactory
{
// DATA
private:
    unsigned int m_lastActorID = 1;
    actorComponentCreatorMap m_actorComponentCreators;
// FUNCTIONS
public:
    ActorFactory(void);
    ~ActorFactory(void);
    std::shared_ptr<Actor> CreateActor(DEFAULT_ACTOR_TYPES actorType);
private:
    std::shared_ptr<ActorComponent> createComponent(std::string component);
    unsigned int getNextActorID(void);
};


#endif