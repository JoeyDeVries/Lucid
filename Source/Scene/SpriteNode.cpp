#include "SpriteNode.h"
#include "../Physics/Event_ActorMoved.h"
#include "../Application/GameApplication.h"
#include "../FastDelegate.h"
#include "Scene.h"

SpriteNode::SpriteNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale, float rotation)
    : SceneNode(ActorID, name, renderPass, position, depth, scale, rotation), m_Animation(false), m_Reverse(false), m_ActiveAnimation("idle")
{

}

SpriteNode::~SpriteNode()
{
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &SpriteNode::ActorMoved);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener, Event_ActorMoved::s_EventType);
}

void SpriteNode::Initialize(Scene *scene)
{
     // update projection matrix 
	if (m_Material)
	{
		m_Material->Initialize();
		m_Material->GetShader()->SetMatrix4("projection", scene->GetCamera()->GetProjection());
        m_Material->GetShader()->SetInteger("Reverse", false);
		if (m_Animation)
		{
			// (assuming this remains same for all state animations, otherwise update this each time anim state changes)
			m_Animations[m_ActiveAnimation]->Init(m_Material->GetShader()); // sets nr animation_data files 
		}
	}

    // register for ActorMoved events to update render data based on physics if physics is enabled
    if (GameApplication::GetInstance()->GetPhysics()->FindBody(m_ActorID))
    {
        EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &SpriteNode::ActorMoved);
        GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_ActorMoved::s_EventType);
    }

    // call base intialize function (to initialize all children)
    SceneNode::Initialize(scene);
}

void SpriteNode::Update(Scene *scene, float deltaTime)
{
    // Update all animations
	for(auto it = m_Animations.begin(); it != m_Animations.end(); ++it)
		it->second->Update(deltaTime);
    // Also update all children
    SceneNode::Update(scene, deltaTime);
}

void SpriteNode::Render(Scene *scene, Renderer *renderer)
{
	// Render stuff here (could do basic sprite rendering here; and later propagate it to subclasses)
	if (m_Material)
	{
		m_Material->GetShader()->Use();
		m_Material->GetShader()->SetMatrix4("model", m_Model);
		m_Material->GetShader()->SetMatrix4("view", scene->GetCamera()->GetView());
        m_Material->PreRender();

		if (m_Animation) // if spriteNode has animation(s) set
		{
			m_Material->GetShader()->SetInteger("animation", true);
			m_Animations[m_ActiveAnimation]->ToShader(m_Material->GetShader());
            //m_Material->SetDiffuse(m_Animations[m_ActiveAnimation]->GetDiffuse());
            //m_Material->SetSpecular(m_Animations[m_ActiveAnimation]->GetSpecular());
            //m_Material->SetNormal(m_Animations[m_ActiveAnimation]->GetNormal());
		}

        if(m_Reverse)
            m_Material->GetShader()->SetInteger("reverse", true);

		renderer->RenderQuad();

        if(m_Reverse)
            m_Material->GetShader()->SetInteger("reverse", false);
		if (m_Animation)
			m_Material->GetShader()->SetInteger("animation", false);
	}	
}

void SpriteNode::SetAnimation(bool enable)
{
	m_Animation = enable;
}
bool SpriteNode::HasAnimation()
{
	return m_Animation;
}

void SpriteNode::SetReverse(bool reverse)
{   
    m_Reverse = reverse;
}
bool SpriteNode::GetReverse()
{
    return m_Reverse;
}

void SpriteNode::AddAnimation(std::shared_ptr<Animation> animation, std::string state)
{
	m_Animations[state] = animation;
}

std::shared_ptr<Animation> SpriteNode::GetAnimation(std::string state)
{
	return m_Animations[state];
}

void SpriteNode::ActivateAnimation(std::string state)
{
	m_ActiveAnimation = state;
    // also set current spritesheets as active
    m_Material->SetDiffuse(m_Animations[m_ActiveAnimation]->GetDiffuse());
    m_Material->SetSpecular(m_Animations[m_ActiveAnimation]->GetSpecular());
    m_Material->SetNormal(m_Animations[m_ActiveAnimation]->GetNormal());
}


void SpriteNode::ActorMoved(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_ActorMoved> eventActorMoved = std::dynamic_pointer_cast<Event_ActorMoved>(eventData);
    if (eventActorMoved->GetActorID() == GetActorID())
    {
        //std::cout << "(" << eventActorMoved->GetNewPosition().x << ", " << eventActorMoved->GetNewPosition().y << std::endl;
        SetPosition(eventActorMoved->GetNewPosition());
        SetRotation(eventActorMoved->GetNewRotation());
    }
}