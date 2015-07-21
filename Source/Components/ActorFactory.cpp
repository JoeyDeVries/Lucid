#include "ActorFactory.h"

#include<iostream>

// actor component creator functions
ActorComponent* CreateTransformComponent() { return new TransformComponent; }



ActorFactory::ActorFactory(void)
{
    m_actorComponentCreators["Transform"] = CreateTransformComponent;
}

ActorFactory::~ActorFactory(void)
{
    auto creatorIt = m_actorComponentCreators.begin();
    //while (creatorIt != m_actorComponentCreators.end())
    //{
    //}
}

std::shared_ptr<Actor> ActorFactory::CreateActor(DEFAULT_ACTOR_TYPES actorType)
{
    std::shared_ptr<Actor> actor;
    switch (actorType)
    {
    case ACTOR_EMPTY:
        return actor;
    case ACTOR_STATIC:
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        return actor;
    case ACTOR_PLAYER:
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        std::shared_ptr<ActorComponent> component = createComponent("Transform");
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
        pComponent.reset();
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