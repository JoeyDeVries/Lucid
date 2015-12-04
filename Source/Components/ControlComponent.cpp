#include "ControlComponent.h"

#include "../Application/GameApplication.h" // Should eventually be done via events to remove this dependancy

#include <iostream>
#include <algorithm>
#include "../FastDelegate.h"
#include "../Physics/Event_PostCollisionAdd.h" 
#include "../Physics/Event_PostCollisionRemove.h" 

ControlComponent::ControlComponent() : m_Velocity(0.0), m_IsJumping(false), m_OnGround(false)
{

}

ControlComponent::~ControlComponent()
{
	// remove callback functions from event queue
    EventListenerDelegate listener1 = fastdelegate::MakeDelegate(this, &ControlComponent::PostCollisionAdd);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener1, Event_PostCollisionAdd::s_EventType);
    EventListenerDelegate listener2 = fastdelegate::MakeDelegate(this, &ControlComponent::PostCollisionRemove);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener2, Event_PostCollisionRemove::s_EventType);
}

bool ControlComponent::VInit(void)
{
    m_Velocity = 3.0;
    // register callback functions
	EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &ControlComponent::PostCollisionAdd);
	GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_PostCollisionAdd::s_EventType);
	EventListenerDelegate listener2 = fastdelegate::MakeDelegate(this, &ControlComponent::PostCollisionRemove);
	GameApplication::GetInstance()->GetEventManager()->AddListener(listener2, Event_PostCollisionRemove::s_EventType);
    return true;
}

void ControlComponent::VUpdate(float deltaTime)
{
    glm::vec2 bodyCenter = m_Owner->GetPosition() + m_Owner->GetScale() * 0.5f;
    Box2DPhysics *physics = GameApplication::GetInstance()->GetPhysics();
    // Gets input from resourcemanager (TODO: later manage input via events, to remove dependancy on GameApplication: listen to keypresses/releases)
	float max = m_Velocity;
	if (GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_D))
	{
		b2Vec2 vel = physics->FindBody(m_Owner->GetID())->GetLinearVelocity();
		//if(m_OnGround)
			physics->FindBody(m_Owner->GetID())->SetLinearVelocity(b2Vec2(m_Velocity, vel.y));
		/*else
			if(vel.x <= max)
				physics->ApplyImpulse(m_Owner->GetID(), glm::vec2(m_Velocity * 7.0f, 0.0), bodyCenter);*/
			
	}
	if (GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_A))
	{
		b2Vec2 vel = physics->FindBody(m_Owner->GetID())->GetLinearVelocity();
		//if(m_OnGround)
			physics->FindBody(m_Owner->GetID())->SetLinearVelocity(b2Vec2(-m_Velocity, vel.y));
		/*else
			if(vel.x >= -max)
				physics->ApplyImpulse(m_Owner->GetID(), glm::vec2(-m_Velocity * 7.0f, 0.0), bodyCenter);*/
	}
    if (GameApplication::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE) && !m_IsJumping)
    {
        GameApplication::GetInstance()->GetPhysics()->ApplyImpulse(m_Owner->GetID(), glm::vec2(0.0, -750.0), bodyCenter);
		m_IsJumping = true;
    }

	// TODO(Joey): Rethink Actor->SceneNode relations, this can probably be done in a more 'clean' way
	auto actorNodeChildren = GameApplication::GetInstance()->GetScene()->GetSceneNode(m_Owner->GetID())->GetChildren();
	for (auto it = actorNodeChildren.begin(); it != actorNodeChildren.end(); ++it)
	{
		std::shared_ptr<LightNode> lantern = std::dynamic_pointer_cast<LightNode>((*it));
		if(lantern) // if one of its children is a lightNode, it is a lantern; set relative position
			lantern->SetPosition(m_Owner->GetPosition() + glm::vec2(50.0f, 20.0f));
	}
}

void ControlComponent::SetVelocity(float velocity)
{
    m_Velocity = velocity;
}

// static in assuming we only have a single control component - TODO(Joey): Make it ControlComponent specific
static int NrGroundCollisionsAdd = 0;
static int NrGroundCollisionsRemove = 0;

void ControlComponent::PostCollisionAdd(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_PostCollisionAdd> data = std::dynamic_pointer_cast<Event_PostCollisionAdd>(eventData);
	if(eventData)
	{
		const b2Contact* contact = data->GetContact();
		const b2Body* character = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
		const b2Fixture* fixtureA = contact->GetFixtureA();
		const b2Fixture* fixtureB = contact->GetFixtureB();

	/*	if (fixtureA->IsSensor() && fixtureB->IsSensor())
			return;*/

		if (contact->IsTouching())
		{
			if (fixtureA->GetBody() == character || character == fixtureB->GetBody())
			{	// we have a collision with the character body
				bool fixtureAIsPlayer = fixtureA->GetBody() == character;
				if ((fixtureAIsPlayer && fixtureA->IsSensor()) || (!fixtureAIsPlayer && fixtureB->IsSensor()))
				{
					// only do stuff if a single one is a sensor
					if ((fixtureA->IsSensor() && !fixtureB->IsSensor()) || (!fixtureA->IsSensor() && fixtureB->IsSensor()))
					{
						NrGroundCollisionsAdd++;
						m_OnGround = true;
						m_IsJumping = false;
						//std::cout << "On ground" << std::endl;
					}
				}
			}
		}

		// prevents collision events between sensors (happens when blocks become un-collidable)
	
	}
}

void ControlComponent::PostCollisionRemove(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_PostCollisionRemove> data = std::dynamic_pointer_cast<Event_PostCollisionRemove>(eventData);
	if(eventData)
	{
		const b2Contact* contact = data->GetContact();
		b2Body* character = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
		const b2Fixture* fixtureA = contact->GetFixtureA();
		const b2Fixture* fixtureB = contact->GetFixtureB();		

		if (fixtureA->GetBody() == character || character == fixtureB->GetBody())
		{	// one of the collision removals is the character, if it is the sensor; we're not on the ground anymore
			bool fixtureAIsPlayer = fixtureA->GetBody() == character;
			if ((fixtureAIsPlayer && fixtureA->IsSensor()) || (!fixtureAIsPlayer && fixtureB->IsSensor()))
			{
				// only do stuff if a single one is a sensor
				if ((fixtureA->IsSensor() && !fixtureB->IsSensor()) || (!fixtureA->IsSensor() && fixtureB->IsSensor()))
				{
					NrGroundCollisionsRemove++;
					if (NrGroundCollisionsRemove >= NrGroundCollisionsAdd)
					{	// prevents user getting stuck at floor due to multiple ground-collisions between each block
						m_OnGround = false;
						//std::cout << "Off ground" << std::endl;

						// TODO(Joey): geef extra userdata(enum) van type block en do : if(color_block && player_sensor) { do extra GroundCollisionRemove; }
					}
				}
			}
		}

		// prevents collision events between sensors (happens when blocks become un-collidable)
		/*if (fixtureA->IsSensor() && fixtureB->IsSensor())
			m_IsJumping = false;*/
	}
	
}
