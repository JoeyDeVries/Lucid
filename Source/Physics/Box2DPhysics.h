#ifndef BOX_2D_PHYSICS
#define BOX_2D_PHYSICS
#include "IPhysics.h"
#include "Box2DDebugDrawer.h"
#include <Box2D/Box2D.h>
#include "../Components/Actor.h"
#include <memory>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <vector>

class Box2DPhysics;
class Box2DContactListener;

class Box2DPhysics : public IPhysics
{
    friend Box2DContactListener;
    // physics world
    b2World* m_World;
    Box2DContactListener* m_ContactListener;
    Box2DDebugDrawer*     m_DebugDrawer;
    // body-actor relations
    std::map<ActorID, b2Body*> m_ActorIDToBody;
    std::map<b2Body*, ActorID> m_BodyToActorID;
    // collisions
	std::list<b2Contact*> m_Collisions;
    void SendCollisionAddEvent(b2Contact* contact);
    void SendCollisionRemoveEvent(b2Contact * contact);

public:
    Box2DPhysics();
    ~Box2DPhysics();

    // initialization and maintenance of physics world
    virtual void Initialize();
    virtual void Update();
    virtual void SyncVisibleScene();
    // body-actor relations
    b2Body* FindBody(ActorID);
    ActorID FindActorID(b2Body*);
    // integration of physics objects
    virtual void AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool isSensor = false);
    virtual void AddBox(std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool fixedRotation = true, bool isSensor = false, float hitBoxScale = 1.0f);
    void AddPolygon(std::shared_ptr<Actor> actor, std::vector<glm::vec2> vertices, float density, bool dynamic = false, bool fixedRotation = true);
    void AddCharacter(std::shared_ptr<Actor> actor, float density);
    virtual void RemoveActor(unsigned int ActorID);
    // debugging
    void RenderDiagnostics();
    // physics world modifiers/forces
    virtual void ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    virtual void ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    virtual void ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons);
	// per body physics data in game-world coordinates
	glm::vec2 GetLinearVelocity(unsigned int ActorID);
	float GetBodyMass(unsigned int ActorID);
	// collision check functions
	bool IsBodiesColliding(const b2Body* bodyA, const b2Body* bodyB);
	
};

#endif