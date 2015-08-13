#include "SpriteNode.h"
#include "../Physics/Event_ActorMoved.h"
#include "../Application/GameApplication.h"
#include "../FastDelegate.h"


SpriteNode::SpriteNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale, float rotation)
    : SceneNode(ActorID, name, renderPass, position, depth, scale, rotation)
{

}

void SpriteNode::Initialize(Scene *scene)
{
     // update projection matrix 
    if(m_Material.GetShader())
        m_Material.GetShader()->SetMatrix4("projection", scene->GetCamera()->GetProjection(), true);

    // register for ActorMoved events to update render data based on physics
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &SpriteNode::ActorMoved);
    GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_ActorMoved::s_EventType);

    // call base intialize function (to initialize all children)
    SceneNode::Initialize(scene);
}

void SpriteNode::Update(Scene *scene, float deltaTime)
{
    // Update (animation perhaps?)
    // Also update all children
    SceneNode::Update(scene, deltaTime);
}

void SpriteNode::Render(Scene *scene)
{
    // Render stuff here (could do basic sprite rendering here; and later propagate it to subclasses)
    m_Material.GetShader()->Use();
    m_Material.GetShader()->SetMatrix4("model", m_Model);
    //m_Material.GetShader()->SetMatrix4("view", scene->Camera()->GetView());
    m_Material.PreRender();
    scene->GetRenderer()->RenderQuad();
}

void SpriteNode::ActorMoved(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_ActorMoved> eventActorMoved = std::dynamic_pointer_cast<Event_ActorMoved>(eventData);
    if (eventActorMoved->GetActorID() == ActorID())
    {
        //std::cout << "(" << eventActorMoved->GetNewPosition().x << ", " << eventActorMoved->GetNewPosition().y << std::endl;
        SetPosition(eventActorMoved->GetNewPosition());
        SetRotation(eventActorMoved->GetNewRotation());
    }
}