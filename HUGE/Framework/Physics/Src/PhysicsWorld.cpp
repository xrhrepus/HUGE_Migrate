#include "Precompiled.h"
#include "Constraint.h"
#include "Particle.h"
#include "PhysicsWorld.h"

void H::Physics::PhysicsWorld::Initialize(const Settings & settings)
{
	mSettings = settings;
}

void H::Physics::PhysicsWorld::Update(float deltatime)
{
	mTimer += deltatime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
		SatisfyConstrains();
	}
}

void H::Physics::PhysicsWorld::DebugDraw() const
{
	using namespace H::Graphics;

	for (auto p : mParticles)
	{
		float r = p->radius;
		//SimpleDraw::AddShpere(Sphere{ p->position, p->radius }, Colors::Cyan);
		SimpleDraw::AddBox(AABB{ p->position,{r,r,r} }, Colors::Cyan);

	}
	for (auto c : mConstraint)
	{
		c->DebugDraw();
	}
 }

void H::Physics::PhysicsWorld::AddParticle(Particle * p)
{
	mParticles.push_back(p);
 }

void H::Physics::PhysicsWorld::AddConstraint(Constraint * c)
{
	mConstraint.push_back(c);
}

void H::Physics::PhysicsWorld::ClearDynamic()
{
	for (auto c : mConstraint)
	{
		if (c)
		{
			delete c;
		}
	}
	mConstraint.clear();

	for (auto p : mParticles)
	{
		if (p)
		{
			delete p;
		}
	}
	mParticles.clear();

}

H::Math::Plane* H::Physics::PhysicsWorld::AddPlane(const H::Math::Plane& plane)
{
	mPlanes.push_back(plane);
	return &mPlanes.back();
}
H::Math::OBB* H::Physics::PhysicsWorld::AddOBB(const H::Math::OBB& obb)
{
	mOBBs.push_back(obb);
	return &mOBBs.back();
}



void H::Physics::PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticles)
		p->acceleration = mSettings.gravity;

}

void H::Physics::PhysicsWorld::Integrate()
{
	const float gamma = 1.0f - mSettings.drag;
	const float timeStepSqr = mSettings.timeStep * mSettings.timeStep;

	for (auto p : mParticles)
	{
		Math::Vector3 displacement = (p->position - p->lastPosition) * gamma + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = p->position + displacement;
	}

}

void H::Physics::PhysicsWorld::SatisfyConstrains()
{
	for (int i = 0; i < mSettings.iteration; ++i)
	{
		for (auto c : mConstraint)
		{
			c->Apply();
		}
	}

	for (auto plane : mPlanes)
	{
		 
		for (auto p : mParticles)
		{
			if (Math::Dot(p->position, plane.n) <= plane.d		// Detect is the position is on the negative side of the plane.
				&& Math::Dot(p->lastPosition, plane.n) > plane.d)	// Detect is the lastPosition is on the positive side(different side with [position]) of plane .
			{//Fliping velocity
				auto velocity = p->position - p->lastPosition;
				auto velocityPerpendicular = plane.n * Math::Dot(velocity, plane.n);
				auto velocityParallel = velocity - velocityPerpendicular;												// Get new velocity by flip the perpendicular velocity
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}
		}
	}


	for (auto obb : mOBBs)
	{
		 
		for (auto p : mParticles)
		{
			if (Math::Intersect(p->position, obb))
			{
				auto velocity = p->position - p->lastPosition;
				auto direction = Math::Normalize(velocity);

				Math::Ray ray{ p->lastPosition, direction };
				Math::Vector3 point, normal;
				Math::GetContactPoint(ray, obb, point, normal);

				auto velocityPerpendicular = normal * Math::Dot(velocity, normal);
				auto velocityParallel = velocity - velocityPerpendicular;												// Get new velocity by flip the perpendicular velocity
				auto newVelocity = (velocityParallel * (1.0f - mSettings.drag)) - (velocityPerpendicular * p->bounce);
				p->SetPosition(p->position - velocityPerpendicular);
				p->SetVelocity(newVelocity);
			}
		}
	}


}
