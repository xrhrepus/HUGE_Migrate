#ifndef HUGE_PHYSICS_PHYSICSWORLD_H
#define HUGE_PHYSICS_PHYSICSWORLD_H

namespace H::Physics
{
	class Constraint;

	struct Settings
	{
		Math::Vector3 gravity{ 0.0f,-9.8f,0.0f };
		float timeStep = 1.0f / 60.0f;
		float drag = 0.0f;
		int iteration = 1;
	};

	class PhysicsWorld
	{
	public:
		void Initialize(const Settings& settings);

		void Update(float deltatime);
		void DebugDraw() const;

		void AddParticle(Particle* p);
		void AddConstraint(Constraint* c);

		void ClearDynamic();

		H::Math::Plane* AddPlane(const H::Math::Plane& plane); 
		H::Math::OBB* AddOBB(const H::Math::OBB& obb); 



	private:

		void AccumulateForces();
		void Integrate();
		void SatisfyConstrains();
		std::vector<Particle*> mParticles;
		std::vector<Constraint*> mConstraint;

		std::vector<H::Math::Plane> mPlanes;
		std::vector<H::Math::OBB> mOBBs;


		Settings mSettings;
		float mTimer = 0.0f;

	};

}

 

#endif // !HUGE_PHYSICS_PHYSICSWORLD_H



