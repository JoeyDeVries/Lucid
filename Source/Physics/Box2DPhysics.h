#ifndef BOX_2D_PHYSICS
#define BOX_2D_PHYSICS
#include "IPhysics.h"
#include "Box2DDebugDrawer.h"
#include <Box2D/Box2D.h>
#include <memory>
#include <map>
#include <set>
#include <utility>

class Box2DContactListener;

class Box2DPhysics : public IPhysics
{
    friend Box2DContactListener;
    // physics world
    b2World* m_World;
    Box2DContactListener* m_ContactListener;
    Box2DDebugDrawer*     m_DebugDrawer;
    // body-actor relations
    typedef unsigned int ActorID;
    std::map<ActorID, b2Body*> m_ActorIDToBody;
    std::map<b2Body*, ActorID> m_BodyToActorID;
    // collisions
    typedef std::pair<b2Body*, b2Body*> CollisionPair;
    std::set<CollisionPair> m_Collisions;
    void SendCollisionPairAddEvent(b2Body* body1, b2Body* body2);
    void SendCollisionPairRemoveEvent(b2Body* body1, b2Body* body2);

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
    virtual void AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic = false);
    virtual void AddBox(std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool fixedRotation = true);
    virtual void RemoveActor(unsigned int ActorID);
    // debugging
    virtual void RenderDiagnostics();
    // physics world modifiers/forces
    virtual void ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    virtual void ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    virtual void ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons);
};

#endif