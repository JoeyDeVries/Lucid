/*******************************************************************
** Copyright (C) 2014-2015 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Lucid.
** https://github.com/JoeyDeVries/Lucid
**
** Lucid is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#include "ActorFactory.h"

#include "CompleteCheckComponent.h"
#include "LightSwitchComponent.h"
#include "TextOnTouchComponent.h"
#include "DamageTouchComponent.h"
#include "StateBlockComponent.h"
#include "MoveLoopComponent.h"
#include "ControlComponent.h"
#include "LifeComponent.h"
#include "AIComponent.h"

#include <iostream>

// actor component creator functions
ActorComponent *CreateCompleteCheckComponent() { return new CompleteCheckComponent; }
ActorComponent *CreateLightSwitchComponent()   { return new LightSwitchComponent; }
ActorComponent *CreateTextOnTouchComponent()   { return new TextOnTouchComponent; }
ActorComponent *CreateDamageTouchComponent()   { return new DamageTouchComponent; }
ActorComponent *CreateStateBlockComponent()    { return new StateBlockComponent; }
ActorComponent *CreateMoveLoopComponent()      { return new MoveLoopComponent; }
ActorComponent *CreateControlComponent()       { return new ControlComponent; }
ActorComponent *CreateLifeComponent()          { return new LifeComponent; }
ActorComponent *CreateAIComponent()            { return new AIComponent; }


ActorFactory::ActorFactory()
{
	m_actorComponentCreators["CompleteCheck"] = CreateCompleteCheckComponent;
	m_actorComponentCreators["LightSwitch"]   = CreateLightSwitchComponent;
    m_actorComponentCreators["TextOnTouch"]   = CreateTextOnTouchComponent;
    m_actorComponentCreators["DamageTouch"]   = CreateDamageTouchComponent;
	m_actorComponentCreators["StateBlock"]    = CreateStateBlockComponent;
    m_actorComponentCreators["MoveLoop"]      = CreateMoveLoopComponent;
	m_actorComponentCreators["Control"]       = CreateControlComponent;
    m_actorComponentCreators["Life"]          = CreateLifeComponent;
    m_actorComponentCreators["AI"]            = CreateAIComponent;
}

ActorFactory::~ActorFactory()
{
    auto creatorIt = m_actorComponentCreators.begin();
}

std::shared_ptr<Actor> ActorFactory::CreateActor(DEFAULT_ACTOR_TYPES actorType)
{
    std::shared_ptr<Actor> actor(new Actor());
    actor->setType(actorType);
    actor->setID(++m_lastActorID);
    switch (actorType)
    {
    case ACTOR_EMPTY: // for empty (no-interaction) objects (like background)
        return actor;
    case ACTOR_STATIC: // for empty (no-interaction) objects (like graphics tiles and/or background)
    {
        return actor;
    }
    case ACTOR_STATE_BLOCK:
    {
        // enables the actor to disable/enable collision based on light state
        std::shared_ptr<ActorComponent> component = createComponent("StateBlock");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_PLAYER:
    {
        // enables the user to have control over player actors
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
        // give the enemy actor life
        std::shared_ptr<ActorComponent> component = createComponent("Life");
        actor->addComponent(component);
        component->setOwner(actor);
        // allows the enemy to move in a looped pattern
        std::shared_ptr<ActorComponent> moveComponent = createComponent("MoveLoop");
        actor->addComponent(moveComponent);
        moveComponent->setOwner(actor);
        // enables the enemy to break loop movement to attack player
        std::shared_ptr<ActorComponent> aiComponent = createComponent("AI");
        actor->addComponent(aiComponent);
        aiComponent->setOwner(actor);
        // enables the enemy to damage the player
        std::shared_ptr<ActorComponent> dmgComponent = createComponent("DamageTouch");
        actor->addComponent(dmgComponent);
        dmgComponent->setOwner(actor);
        return actor;
    }
    case ACTOR_LANTERN:
    {
        // enables the actor to switch its light type
        std::shared_ptr<ActorComponent> component = createComponent("LightSwitch");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_COMPLETE_CHECK:
    {
        // enables the actor to finish the level on player-actor touch
        std::shared_ptr<ActorComponent> component = createComponent("CompleteCheck");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_SIGNPOST:
    {
        // enables the actor to display text when the player touches the actor
        std::shared_ptr<ActorComponent> component = createComponent("TextOnTouch");
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_DEATHTOUCH:
    {
        // kills the player if it touches the actor
        std::shared_ptr<ActorComponent> component = createComponent("DamageTouch");
        std::dynamic_pointer_cast<DamageTouchComponent>(component)->SetDamageAmount(1000); // high enough to kill any player/monster
        actor->addComponent(component);
        component->setOwner(actor);
        return actor;
    }
    case ACTOR_MOVE_LOOP:
    {
        // enables the actor to move in a predefined loop pattern
        std::shared_ptr<ActorComponent> component = createComponent("MoveLoop");
        actor->addComponent(component);
        component->setOwner(actor);
        // enables the actor to disable/enable collision based on light state
        std::shared_ptr<ActorComponent> stateComponent = createComponent("StateBlock");
        actor->addComponent(stateComponent);
        stateComponent->setOwner(actor);
        return actor;
    }
    default:
        actor.reset();
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