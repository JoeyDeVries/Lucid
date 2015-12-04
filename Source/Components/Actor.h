#ifndef ACTOR_H
#define ACTOR_H
#include <memory>
#include <map>

#include "ActorComponentDefinitions.h"
#include "ActorFactory.h"
#include "ActorComponent.h"

#include <glm/glm.hpp>

typedef unsigned int ActorID;

class ActorComponent;

class Actor // Should NOT be subclassed: all variaty comes from its components. It's only purpose is to manage its components
{
    friend class ActorFactory;
    typedef std::map<std::string, std::shared_ptr<ActorComponent>> ActorComponents;
private:
    ActorID		    m_id;
    ActorComponents m_components;

    glm::vec2 m_Position;
    glm::vec2 m_Scale;
    int       m_Depth;
    float     m_Rotation;

    // Only allow ActorFactory to access this function; possible by
    // ActorFactory being declared as friend
    void addComponent(std::shared_ptr<ActorComponent> component);
    // Sets the ID of the current actor
    void setID(ActorID id);
public:
    Actor(void);
    ~Actor(void);
    void Destroy(void);
    void Update(float deltaTime);

	const glm::vec2&   GetPosition() { return m_Position; }
	const glm::vec2&   GetScale()    { return m_Scale; }
	const int&         GetDepth()    { return m_Depth; }
	const float&       GetRotation() { return m_Rotation; }
	const unsigned int GetID()       { return m_id; }

	void SetPosition(glm::vec2 pos)  { m_Position = pos; }
	void SetScale(glm::vec2 scale)   { m_Scale = scale; }
	void SetDepth(int depth)         { m_Depth = depth; }
	void SetRotation(float rotation) { m_Rotation = rotation; }

	// NOTE(Joey): unfortunately in C++ template functions need to be declared and defined in the same file
	template<class ComponentType> std::weak_ptr<ComponentType> GetComponent(std::string type)
	{
		ActorComponents::iterator findIt = m_components.find(type);
		if (findIt != m_components.end())
		{
			std::shared_ptr<ActorComponent> pBase(findIt->second);
			// cast to subclass version of the pointer
			std::shared_ptr<ComponentType> pSub(std::tr1::static_pointer_cast<ComponentType>(pBase));
			std::weak_ptr<ComponentType> pWeakSub(pSub); // convert strong pointer to weak pointer
			return pWeakSub;
		}
		else
			return std::weak_ptr<ComponentType>();
	}
private:

};

#endif