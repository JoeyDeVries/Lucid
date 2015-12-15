#include "Box2DPhysics.h"
#include "../Application/GameApplication.h"
#include "Box2DContactListener.h"
#include "Box2DDebugDrawer.h"
#include "Event_ActorMoved.h"
#include "Event_PostCollisionAdd.h";
#include "Event_PostCollisionRemove.h";

// simple conversion functions to convert from pixels to meters and vica versa
// necessary as Box2D works best with real metrics
const float M2P = 40.0f;
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
            actor->SetPosition(bodyPos);
            actor->SetRotation(bodyRot);
            // send event
			GameApplication::GetInstance()->GetEventManager()->QueueEvent(std::shared_ptr<Event_ActorMoved>(new Event_ActorMoved(id, bodyPos, bodyRot)));
        }
    }
}

void Box2DPhysics::RemoveQueuedItems()
{
    for (auto it = m_RemovalQueue.begin(); it != m_RemovalQueue.end(); ++it)
    {
        if (m_ActorIDToBody.find(*it) != m_ActorIDToBody.end())
        {
            std::vector<b2Contact*> toRemove;
            for (auto colIt = m_Collisions.begin(); colIt != m_Collisions.end(); ++colIt)
                if ((*colIt)->GetFixtureA()->GetBody() == m_ActorIDToBody[*it] || (*colIt)->GetFixtureA()->GetBody() == m_ActorIDToBody[*it])
                    toRemove.push_back(*colIt);
            for(int i = 0; i < toRemove.size(); ++i)
                m_Collisions.remove(toRemove[i]);
            m_World->DestroyBody(m_ActorIDToBody[*it]);
            m_BodyToActorID.erase(m_ActorIDToBody[*it]);
            m_ActorIDToBody.erase(*it);
        }
    }
    m_RemovalQueue.clear();
}

void Box2DPhysics::Reset()
{
    b2Body* body = m_World->GetBodyList();
    while (body)
    {
        body = body->GetNext();
        m_World->DestroyBody(body);
    }

    m_World->ClearForces();
    m_Collisions.clear();
}

b2Body* Box2DPhysics::FindBody(ActorID actorID)
{
    if(m_ActorIDToBody.find(actorID) != m_ActorIDToBody.end())
        return m_ActorIDToBody[actorID];
    else
        return nullptr;
}

unsigned int Box2DPhysics::FindActorID(b2Body* body)
{
    if (m_BodyToActorID.find(body) != m_BodyToActorID.end())
        return m_BodyToActorID.at(body);
    else
        return 0;
}

void Box2DPhysics::AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic, bool isSensor)
{

}

void Box2DPhysics::AddBox(std::shared_ptr<Actor> actor, float density, std::string type, bool fixedRotation, bool isSensor, float hitboxScale)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(PixelsToMeters(actor->GetPosition().x + actor->GetScale().x * 0.5), PixelsToMeters(actor->GetPosition().y + actor->GetScale().y * 0.5)); // position should be center of object instead of top-left
    if(type == "dynamic")
        bodyDef.type = b2_dynamicBody;
    else if(type == "kinematic")
        bodyDef.type = b2_kinematicBody;
    bodyDef.fixedRotation = fixedRotation;
    b2Body* body = m_World->CreateBody(&bodyDef);
    body->SetUserData(this);

    b2PolygonShape shape;
    shape.SetAsBox(PixelsToMeters(hitboxScale * actor->GetScale().x * 0.5), PixelsToMeters(hitboxScale * actor->GetScale().y * 0.5));
   
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
	fixture.isSensor = isSensor;
    
    body->CreateFixture(&fixture);

    m_BodyToActorID[body] = actor->GetID();
    m_ActorIDToBody[actor->GetID()] = body;
}

void Box2DPhysics::AddPolygon(std::shared_ptr<Actor> actor, std::vector<glm::vec2> vertices, float density, bool dynamic, bool fixedRotation)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(PixelsToMeters(actor->GetPosition().x + actor->GetScale().x * 0.5), PixelsToMeters(actor->GetPosition().y + actor->GetScale().y * 0.5)); // position should be center of object instead of top-left
    if(dynamic)
        bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = fixedRotation;
    b2Body* body = m_World->CreateBody(&bodyDef);
    body->SetUserData(this);

    // create 8 vertices 
    b2Vec2 verts[8];
    for(unsigned int i = 0; i < vertices.size(); ++i)
        verts[i].Set(PixelsToMeters(vertices[i].x * actor->GetScale().x * 0.5), PixelsToMeters(vertices[i].y * actor->GetScale().y * 0.5));
    b2PolygonShape shape;    
    shape.Set(verts, 8);
   
    b2FixtureDef fixture;
    fixture.shape = &shape;
    fixture.density = density;
    
    body->CreateFixture(&fixture);

    m_BodyToActorID[body] = actor->GetID();
    m_ActorIDToBody[actor->GetID()] = body;
}

void Box2DPhysics::AddCharacter(std::shared_ptr<Actor> actor, float density)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(PixelsToMeters(actor->GetPosition().x + actor->GetScale().x * 0.5), PixelsToMeters(actor->GetPosition().y + actor->GetScale().y * 0.5)); // position should be center of object instead of top-left
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
	bodyDef.allowSleep = false;
    b2Body* body = m_World->CreateBody(&bodyDef);
    body->SetUserData(this);
	body->SetBullet(true);

    // Create box top-shape
    b2PolygonShape boxShape;
    //boxShape.m_centroid.Set(0.0, -PixelsToMeters(actor->GetScale().x));
    b2Vec2 vertices[4];
    vertices[0].Set(PixelsToMeters(-actor->GetScale().x * 0.4), PixelsToMeters(-actor->GetScale().y * 0.5));
    vertices[1].Set(PixelsToMeters(-actor->GetScale().x * 0.4), PixelsToMeters(actor->GetScale().y * 0.0));
    vertices[2].Set(PixelsToMeters(actor->GetScale().x * 0.4), PixelsToMeters(actor->GetScale().y * 0.0));
    vertices[3].Set(PixelsToMeters(actor->GetScale().x * 0.4), PixelsToMeters(-actor->GetScale().y * 0.5));
    boxShape.Set(vertices, 4);
    //boxShape.SetAsBox(PixelsToMeters(actor->GetScale().x * 0.5), PixelsToMeters(actor->GetScale().y * 0.5 - 20)); // - half the width for the bottom circle
    b2FixtureDef fixture;
    fixture.shape = &boxShape;
    fixture.density = density;
    fixture.friction = 0.95f;
    body->CreateFixture(&fixture);
    // Create circle bottom-shape
    b2CircleShape circleShape;
    circleShape.m_p.Set(0.0f, PixelsToMeters(actor->GetScale().y * 0.5 - actor->GetScale().x * 0.42));
    circleShape.m_radius = PixelsToMeters(actor->GetScale().x * 0.4);
    b2FixtureDef fixture2;
    fixture2.shape = &circleShape;
    fixture2.density = density;
    fixture2.friction = 10.0f;
	// fixture2.restitution = 0.25f;
    body->CreateFixture(&fixture2);

	// Create bottom sensor to detect floor-collisions
	b2PolygonShape sensorShape;
	b2Vec2 origin = b2Vec2(0.0, PixelsToMeters(actor->GetScale().y * 0.3));
	sensorShape.SetAsBox(PixelsToMeters(actor->GetScale().x * 0.15), PixelsToMeters(actor->GetScale().y * 0.2), origin, 0.0f);
	b2FixtureDef fixture3;
	fixture3.shape = &sensorShape;
	fixture3.isSensor = true;
	body->CreateFixture(&fixture3);

	
	m_BodyToActorID[body] = actor->GetID();
    m_ActorIDToBody[actor->GetID()] = body;    
}

void Box2DPhysics::RemoveActor(unsigned int ActorID)
{
    m_RemovalQueue.push_back(ActorID);
    //m_World->DestroyBody(m_ActorIDToBody[ActorID]); // TODO(Joey): schedule them for later removal instead of instantly removing them
}

void Box2DPhysics::RenderDiagnostics()
{
    m_World->DrawDebugData();
}

void Box2DPhysics::ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center)
{
    m_ActorIDToBody[ActorID]->ApplyForceToCenter(b2Vec2(PixelsToMeters(force.x), PixelsToMeters(force.y)),  true); 
}

void Box2DPhysics::ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center)
{
    m_ActorIDToBody[ActorID]->ApplyLinearImpulse(b2Vec2(PixelsToMeters(force.x), PixelsToMeters(force.y)), b2Vec2(PixelsToMeters(center.x), PixelsToMeters(center.y)), true);
}

void Box2DPhysics::ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons)
{

}

void Box2DPhysics::SendCollisionAddEvent(b2Contact* contact)
{
	m_Collisions.push_back(contact);
	// Send collision event to event system
	GameApplication::GetInstance()->GetEventManager()->QueueEvent(std::shared_ptr<Event_PostCollisionAdd>(new Event_PostCollisionAdd(contact)));
}

void Box2DPhysics::SendCollisionRemoveEvent(b2Contact* contact)
{
	m_Collisions.remove(contact);
	// Send collision event to event system
	GameApplication::GetInstance()->GetEventManager()->QueueEvent(std::shared_ptr<Event_PostCollisionRemove>(new Event_PostCollisionRemove(contact)));
}

glm::vec2 Box2DPhysics::GetLinearVelocity(unsigned int ActorID)
{
	b2Vec2 linVel = FindBody(ActorID)->GetLinearVelocity();
	return glm::vec2(MetersToPixels(linVel.x), MetersToPixels(linVel.y));
}

float Box2DPhysics::GetBodyMass(unsigned int ActorID)
{
	return MetersToPixels(FindBody(ActorID)->GetMass());
}

// Check collisions between all fixtures of two bodies
bool Box2DPhysics::IsBodiesColliding(const b2Body* bodyA, const b2Body* bodyB)
{
	bool isColliding = false;
	// iterate through both fixture lists to determine if entire bodies collide
	for (const b2Fixture* fixA = bodyA->GetFixtureList(); fixA; fixA = fixA->GetNext())
	{
		for (const b2Fixture* fixB = bodyB->GetFixtureList(); fixB; fixB = fixB->GetNext())
		{
			for (auto it = m_Collisions.begin(); it != m_Collisions.end(); ++it)
			{
				const b2Fixture* colFixA = (*it)->GetFixtureA();
				const b2Fixture* colFixB = (*it)->GetFixtureB();
				// don't verify collisions for sensors
				/*if (fixA->IsSensor() || fixB->IsSensor())
					continue;*/

				if (fixA != fixB && (colFixA == fixA && colFixB == fixB) || (colFixA == fixB && colFixB == fixA))
					isColliding = true;
			}
		}
	}
	return isColliding;
}


