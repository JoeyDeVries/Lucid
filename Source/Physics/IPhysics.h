#ifndef I_PHYSICS_H
#define I_PHYSICS_H
#include <memory>
#include "../Components/Actor.h"
#include <glm/glm.hpp>

class IPhysics
{
public:
    // initialization and maintenance of physics world
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void SyncVisibleScene() = 0;
    // integration of physics objects
    virtual void AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool isSensor = false) = 0;
    virtual void AddBox(std::shared_ptr<Actor> actor, float density, std::string type = "static", bool fixedRotation = true, bool isSensor = false, float hitBoxScale = 1.0f) = 0;
    virtual void RemoveActor(unsigned int ActorID) = 0;
    // debugging
    virtual void RenderDiagnostics() = 0;
    // physics world modifiers/forces
    virtual void ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center) = 0;
    virtual void ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center) = 0;
    virtual void ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons) = 0;
};

#endif