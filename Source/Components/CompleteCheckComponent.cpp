#include "CompleteCheckComponent.h"
#include "Event_LevelComplete.h"
#include "../Physics/Event_PostCollisionAdd.h"
#include "../Application/GameApplication.h"

CompleteCheckComponent::CompleteCheckComponent()
{

}

CompleteCheckComponent::~CompleteCheckComponent()
{
	// remove event
	EventListenerDelegate listener1 = fastdelegate::MakeDelegate(this, &CompleteCheckComponent::OnPostCollisionAdd);
	GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener1, Event_PostCollisionAdd::s_EventType);
}

bool CompleteCheckComponent::VInit()
{
	// register event
	EventListenerDelegate listener1 = fastdelegate::MakeDelegate(this, &CompleteCheckComponent::OnPostCollisionAdd);
	GameApplication::GetInstance()->GetEventManager()->AddListener(listener1, Event_PostCollisionAdd::s_EventType);
	return true;
}

void CompleteCheckComponent::VUpdate(float delta)
{

}

void CompleteCheckComponent::OnPostCollisionAdd(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_PostCollisionAdd> data = std::dynamic_pointer_cast<Event_PostCollisionAdd>(eventData);
	if (eventData)
	{
		// get relevant bodies
		const b2Contact* contact = data->GetContact();
		const b2Body* currentBody = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
		const b2Body* playerBody = GameApplication::GetInstance()->GetPhysics()->FindBody(GameApplication::GetInstance()->GetImportantActor("player")->GetID());
		// determine if both of them collided
		const b2Body* bodyA = contact->GetFixtureA()->GetBody();
		const b2Body* bodyB = contact->GetFixtureB()->GetBody();
		if ((bodyA == currentBody && bodyB == playerBody) || (bodyA == playerBody && bodyB == currentBody))
		{	// both relevant bodies collided, send level complete event
			GameApplication::GetInstance()->GetEventManager()->QueueEvent(std::shared_ptr<Event_LevelComplete>(new Event_LevelComplete));
		}
	}
}