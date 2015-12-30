#include "LightSwitchComponent.h"
#include "Event_LightStateSwitched.h"
#include "../Application/GameApplication.h"


LightSwitchComponent::LightSwitchComponent() : m_LightState(LightState::OFF), m_Initialized(false)
{
	
}

LightSwitchComponent::~LightSwitchComponent()
{

}

bool LightSwitchComponent::VInit()
{ 
	return true;
}

void LightSwitchComponent::VUpdate(float deltaTime)
{
	// check keys, and respond accordingly (if none of the keys are pressed, set to default)
	LightState old = m_LightState;
	if (GameApplication::GetInstance()->IsKeyPressed('1'))
	{
		m_LightState = LightState::WHITE;
	}
	else if (GameApplication::GetInstance()->IsKeyPressed('2'))
	{
		m_LightState = LightState::RED;
	}
	else if (GameApplication::GetInstance()->IsKeyPressed('3'))
	{
		m_LightState = LightState::GREEN;
	}
	else if (GameApplication::GetInstance()->IsKeyPressed('4'))
	{
		m_LightState = LightState::BLUE;
	}
	else if (GameApplication::GetInstance()->IsKeyPressed('`'))
	{
		m_LightState = LightState::OFF;
	}
	// check if light state changed and if so, update actor and send event
	if (old != m_LightState || !m_Initialized)
	{
        GameApplication::GetInstance()->GetAudio()->PlaySound("audio/light_switch.mp3");
		// chance values accordingly in accompying LightNode
		std::shared_ptr<LightNode> lightNode = std::dynamic_pointer_cast<LightNode>(
			GameApplication::GetInstance()->GetScene()->GetSceneNode(m_Owner->GetID())
		);
		if (lightNode)
		{
			switch (m_LightState)
			{
			case LightState::OFF:
				lightNode->SetDiffuse(glm::vec3(0.0));
				break;
			case LightState::WHITE:
				lightNode->SetDiffuse(glm::vec3(1.0));
				break;
			case LightState::RED:
				lightNode->SetDiffuse(glm::vec3(1.0, 0.2, 0.2));
				break;
			case LightState::GREEN:
				lightNode->SetDiffuse(glm::vec3(0.2, 1.0, 0.2));
				break;
			case LightState::BLUE:
				lightNode->SetDiffuse(glm::vec3(0.2, 0.2, 1.0));
				break;
			}
		}
		GameApplication::GetInstance()->GetEventManager()->QueueEvent(std::shared_ptr<Event_LightStateSwitched>(new Event_LightStateSwitched(m_LightState)));
        m_Initialized = true;
	}
}