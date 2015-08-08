#include "Box2DPhysics.h"
#include "../Application/GameApplication.h"
#include "Box2DContactListener.h"
#include "Box2DDebugDrawer.h"
#include "Event_ActorMoved.h"

// simple conversion functions to convert from pixels to meters and vica versa
// necessary as Box2D works best with real metrics
const float M2P = 25.0f;
float PixelsToMeters(float pixel)
{
    return pixel * (1.0 / M2P);
}

float MetersToPixels(float meters)
{
    return meters * M2P; 
}

Box2DPhysics::Box2DPhysics()
{
    m_World = new b2World(b2Vec2(0.0, 9.81f));
    m_ContactListener = new Box2DContactListener;
    m_DebugDrawer     = new Box2DDebugDrawer;
}

Box2DPhysics::~Box2DPhysics()
{
    delete m_World;
    delete m_ContactListener;
    delete m_DebugDrawer;
}

void Box2DPhysics::Initialize()
{
    m_World->SetContactListener(m_ContactListener);
    m_World->SetDebugDraw(m_DebugDrawer);
    m_DebugDrawer->SetFlags(Box2DDebugDrawer::e_shapeBit);
}

void Box2DPhysics::Update()
{
    m_World->Step(1.0f / 60.0f, 6, 2);
}

bool IsVec2Equal(glm::vec2 a, glm::vec2 b, float range = 0.005)
{
    if (abs(a.x - b.x) > range)
        return false;
    if (abs(a.y - b.y) > range)
        return false;
    return true;
}

void Box2DPhysics::SyncVisibleScene()
{
    for (std::map<ActorID, b2Body*>::const_iterator it = m_ActorIDToBody.begin(); it != m_ActorIDToBody.end(); ++it)
    {
        ActorID id = it->first;
        glm::vec2 bodyPos = glm::vec2(MetersToPixels(it->second->GetPosition().x), MetersToPixels(it->second->GetPosition().y));
        float     bodyRot = it->second->GetAngle();
        std::shared_ptr<Actor> actor = GameApplication::GetInstance()->GetActor(id);
        // bodyPos is from center of object, first transform to top-left of object so it corresponds with game-logic 
        bodyPos -= actor->GetScale() * 0.5f;
        // due to floating point precision I can't directly compare the positions, compare them given some range interval
        if (!IsVec2Equal(actor->GetPosition(), bodyPos)) // something changed, update scene
        {
            // set actor to correct position and start event that actor has moved (scene will listen to this event for render changes)
            actor->GetPosition() = bodyPos;
            actor->GetRotation() = bodyRot;
            // send event
            GameApplication::GetInstance()->GetEventManager()->QueueEvent(std::shared_ptr<Event_ActorMoved>(new Event_ActorMoved(id, bodyPos, bodyRot)));
        }
    }
}

b2Body* Box2DPhysics::FindBody(ActorID actorID)
{
    return m_ActorIDToBody[actorID];
}

unsigned int Box2DPhysics::FindActorID(b2Body* body)
{
    return m_BodyToActorID[body];
}

void Box2DPhysics::AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic)
{

}

void Box2DPhysics::AddBox(std::shared_ptr<Actor> actor, float density, bool dynamic, bool fixedRotation)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(PixelsToMeters(actor->GetPosition().x + actor->GetScale().x * 0.5), PixelsToMeters(actor->GetPosition().y + actor->GetScale().y * 0.5)); // position should be center of object instead of top-left
    if(dynamic)
        bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = fixedRotation;
    b2Body* body = m_World->CreateBody(&bodyDef);
    body->SetUserData(this);

    b2PolygonShape shape;
    shape.SetAsBox(PixelsToMeters(actor->GetScale().x * 0.5), PixelsToMeters(actor->GetScale().y * 0.5));
   
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
    
    body->CreateFixture(&fixture);

    m_BodyToActorID[body] = actor->GetID();
    m_ActorIDToBody[actor->GetID()] = body;
}

void Box2DPhysics::RemoveActor(unsigned int ActorID)
{

}

void Box2DPhysics::RenderDiagnostics()
{
    m_World->DrawDebugData();
}

void Box2DPhysics::ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center)
{
    m_ActorIDToBody[ActorID]->ApplyForce(b2Vec2(PixelsToMeters(force.x), PixelsToMeters(force.y)), b2Vec2(PixelsToMeters(center.x), PixelsToMeters(center.y)), true); 
}

void Box2DPhysics::ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center)
{
    m_ActorIDToBody[ActorID]->ApplyLinearImpulse(b2Vec2(PixelsToMeters(force.x), PixelsToMeters(force.y)), b2Vec2(PixelsToMeters(center.x), PixelsToMeters(center.y)), true);
}

void Box2DPhysics::ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons)
{

}

void Box2DPhysics::SendCollisionPairAddEvent(b2Body* body1, b2Body* body2)
{

}

void Box2DPhysics::SendCollisionPairRemoveEvent(b2Body* body1, b2Body* body2)
{

}