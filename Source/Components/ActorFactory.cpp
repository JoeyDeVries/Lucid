#include "ActorFactory.h"
#include "ControlComponent.h"
#include "LightSwitchComponent.h"
#include "StateBlockComponent.h"
#include "CompleteCheckComponent.h"
#include<iostream>

// actor component creator functions
ActorComponent* CreateControlComponent()       { return new ControlComponent; }
ActorComponent* CreateLightSwitchComponent()   { return new LightSwitchComponent; }
ActorComponent* CreateStateBlockComponent()    { return new StateBlockComponent; }
ActorComponent* CreateCompleteCheckComponent() { return new CompleteCheckComponent; }


ActorFactory::ActorFactory(void)
{
	m_actorComponentCreators["Control"] = CreateControlComponent;
	m_actorComponentCreators["LightSwitch"] = CreateLightSwitchComponent;
	m_actorComponentCreators["StateBlock"] = CreateStateBlockComponent;
	m_actorComponentCreators["CompleteCheck"] = CreateCompleteCheckComponent;
}

ActorFactory::~ActorFactory(void)
{
    auto creatorIt = m_actorComponentCreators.begin();
    //while (creatorIt != m_actorComponentCreators.end())
    //{
    //}
}

// TODO(Joey): Data-driven development should depic the actor types
std::shared_ptr<Actor> ActorFactory::CreateActor(DEFAULT_ACTOR_TYPES actorType)
{
    std::shared_ptr<Actor> actor;
    switch (actorType)
    {
    case ACTOR_EMPTY: // for empty (no-interaction) objects (like background)
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        return actor;
    case ACTOR_STATIC:
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
       /* std::shared_ptr<ActorComponent> component = createComponent("Control");
        actor->addComponent(component);
        component->setOwner(actor);*/
        return actor;
    }
	case ACTOR_STATE_BLOCK:
	{
		actor = std::shared_ptr<Actor>(new Actor());
		actor->setID(++m_lastActorID);
		std::shared_ptr<ActorComponent> component = createComponent("StateBlock");
		actor->addComponent(component);
		component->setOwner(actor);
		return actor;
	}
    case ACTOR_PLAYER:
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        std::shared_ptr<ActorComponent> component = createComponent("Control");
		actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
	case ACTOR_LANTERN:
	{
		actor = std::shared_ptr<Actor>(new Actor());
		actor->setID(++m_lastActorID);
		std::shared_ptr<ActorComponent> component = createComponent("LightSwitch");
		actor->addComponent(component);
		component->setOwner(actor);
		return actor;
	}
	case ACTOR_COMPLETE_CHECK:
		actor = std::shared_ptr<Actor>(new Actor());
		actor->setID(++m_lastActorID);
		std::shared_ptr<ActorComponent> component = createComponent("CompleteCheck");
		actor->addComponent(component);
		component->setOwner(actor);
		return actor;
    }
}

std::shared_ptr<ActorComponent> ActorFactory::createComponent(std::string component)
{
    std::shared_ptr<ActorComponent> pComponent;

    auto findIt = m_actorComponentCreators.find(component);
    if (findIt != m_actorComponentCreators.end())
    {
        ActorComponentCreator creator = findIt->second;
        ActorComponent *component = creator();
        component->m_Type = findIt->first;
        pComponent.reset(component);
    }
    else
    {
        std::cout << "Couldn't find ActorComponent named " + component << std::endl;
        return std::shared_ptr<ActorComponent>();
    }

    // initialize the component if we found one
    if (pComponent)
    {
        if (!pComponent->VInit())
        {
            std::cout << "Component failed to initialzie: " + component << std::endl;
        }
    }

    // pComponent will be null if the component wasn't found. This isn't necessarily
    // an error since you might have a custom CreateComponent() function in a subclass
    return pComponent;
}

unsigned int ActorFactory::getNextActorID()
{
    return (++m_lastActorID);
}