#ifndef ACTOR_H
#define ACTOR_H
#include <memory>

#include "ActorComponentDefinitions.h"
#include "ActorFactory.h"
#include "ActorComponent.h"

#include <glm/glm.hpp>

class ActorComponent;

class Actor // Should NOT be subclassed: all variaty comes from its components. It's only purpose is to manage its components
{
    friend class ActorFactory;
    typedef std::map<std::string, std::shared_ptr<ActorComponent>> ActorComponents;
private:
    unsigned int m_id;
    ActorComponents m_components;

    glm::vec2 m_Position;
    glm::vec2 m_Scale;
    int       m_Depth;
    float     m_Rotation;

    // Only allow ActorFactory to access this function; possible by
    // ActorFactory being declared as friend
    void addComponent(std::shared_ptr<ActorComponent> component);
    // Sets the ID of the current actor
    void setID(unsigned int id);
public:
    Actor(void);
    ~Actor(void);
    void Destroy(void);
    void Update(float deltaTime);
    glm::vec2& GetPosition();
    glm::vec2& GetScale();
    int&       Depth();
    float&     GetRotation();
    unsigned int GetID();
    template<class ComponentType> std::weak_ptr<ComponentType> GetComponent(std::string type);
private:

};

#endif