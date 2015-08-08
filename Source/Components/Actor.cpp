#include "Actor.h"


Actor::Actor(void) : m_Position(0.0), m_Scale(1.0), m_Depth(0)
{

}

Actor::~Actor(void)
{

}

void Actor::Destroy(void)
{

}

void Actor::Update(float deltaTime)
{
    ActorComponents::iterator it = m_components.begin();
    while (it != m_components.end())
        (it++)->second->VUpdate(deltaTime);
}

glm::vec2& Actor::GetPosition()
{
    return m_Position;
}

glm::vec2& Actor::GetScale()
{
    return m_Scale;
}

int& Actor::Depth()
{
    return m_Depth;
}

float& Actor::GetRotation()
{
    return m_Rotation;
}

unsigned int Actor::GetID(void)
{
    return m_id;
}

template<class ComponentType> std::weak_ptr<ComponentType> Actor::GetComponent(std::string type)
{
    ActorComponents::iterator findIt = m_components.find(type);
    if (findIt != m_components.end())
    {
        std::shared_ptr<ComponentType> pBase(findIt->second);
        // cast to subclass version of the pointer
        std::shared_ptr<ComponentType> pSub(std::tr1::static_pointer_cast<ComponentType>(pBase));
        std::weak_ptr<ComponentType> pWeakSub(pSub); // convert strong pointer to weak pointer
        return pWeakSub;
    }
    else
        return std::weak_ptr<ComponentType>();
}

void Actor::addComponent(std::shared_ptr<ActorComponent> component)
{
    m_components[component->m_Type] = component;
}

void Actor::setID(unsigned int ID)
{
    m_id = ID;
}