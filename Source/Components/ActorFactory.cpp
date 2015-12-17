#include "ActorFactory.h"
#include "ControlComponent.h"
#include "LightSwitchComponent.h"
#include "StateBlockComponent.h"
#include "CompleteCheckComponent.h"
#include "TextOnTouchComponent.h"
#include "LifeComponent.h"
#include "DamageTouchComponent.h"
#include "MoveLoopComponent.h"
#include "AIComponent.h"
#include<iostream>

// actor component creator functions
ActorComponent *CreateControlComponent()       { return new ControlComponent; }
ActorComponent *CreateLightSwitchComponent()   { return new LightSwitchComponent; }
ActorComponent *CreateStateBlockComponent()    { return new StateBlockComponent; }
ActorComponent *CreateCompleteCheckComponent() { return new CompleteCheckComponent; }
ActorComponent *CreateTextOnTouchComponent()   { return new TextOnTouchComponent; }
ActorComponent *CreateLifeComponent()          { return new LifeComponent; }
ActorComponent *CreateDamageTouchComponent()   { return new DamageTouchComponent; }
ActorComponent *CreateMoveLoopComponent()      { return new MoveLoopComponent; }
ActorComponent *CreateAIComponent()            { return new AIComponent; }


ActorFactory::ActorFactory(void)
{
	m_actorComponentCreators["Control"] = CreateControlComponent;
	m_actorComponentCreators["LightSwitch"] = CreateLightSwitchComponent;
	m_actorComponentCreators["StateBlock"] = CreateStateBlockComponent;
	m_actorComponentCreators["CompleteCheck"] = CreateCompleteCheckComponent;
    m_actorComponentCreators["TextOnTouch"] = CreateTextOnTouchComponent;
    m_actorComponentCreators["Life"] = CreateLifeComponent;
    m_actorComponentCreators["DamageTouch"] = CreateDamageTouchComponent;
    m_actorComponentCreators["MoveLoop"] = CreateMoveLoopComponent;
    m_actorComponentCreators["AI"] = CreateAIComponent;
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
    case ACTOR_STATIC: // for empty (no-interaction) objects (like graphics tiles and/or background)
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
        // allow the user to have control over player actors
        std::shared_ptr<ActorComponent> component = createComponent("Control");
        actor->addComponent(component);
        component->setOwner(actor);
        // give the player actor life
        std::shared_ptr<ActorComponent> life = createComponent("Life");
        actor->addComponent(life);
        life->setOwner(actor);
        return actor;
    }
    case ACTOR_ENEMY:
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        // give the enemy actor life
        std::shared_ptr<ActorComponent> component = createComponent("Life");
        actor->addComponent(component);
        component->setOwner(actor);
        std::shared_ptr<ActorComponent> moveComponent = createComponent("MoveLoop");
        actor->addComponent(moveComponent);
        moveComponent->setOwner(actor);
        std::shared_ptr<ActorComponent> aiComponent = createComponent("AI");
        actor->addComponent(aiComponent);
        aiComponent->setOwner(actor);
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
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        std::shared_ptr<ActorComponent> component = createComponent("CompleteCheck");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_SIGNPOST:
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        std::shared_ptr<ActorComponent> component = createComponent("TextOnTouch");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_DEATHTOUCH:
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        std::shared_ptr<ActorComponent> component = createComponent("DamageTouch");
        std::dynamic_pointer_cast<DamageTouchComponent>(component)->SetDamageAmount(1000); // kill actor/player on touch
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_MOVE_LOOP:
    {
        actor = std::shared_ptr<Actor>(new Actor());
        actor->setID(++m_lastActorID);
        std::shared_ptr<ActorComponent> component = createComponent("MoveLoop");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    }
    return actor;
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