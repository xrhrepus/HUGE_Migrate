#ifndef HUGE_PHYSICS_PARTICLE_H
#define HUGE_PHYSICS_PARTICLE_H

namespace H::Physics
{
	struct Particle
	{
		H::Math::Vector3 position = H::Math::Vector3::Zero();
		H::Math::Vector3 lastPosition = H::Math::Vector3::Zero();
		H::Math::Vector3 acceleration = H::Math::Vector3::Zero();
		float radius = 1.0f;
		float invMass = 0.0f;
		float bounce = 0.0f;

		void SetPosition(const Vector3& pos);
 
		void SetVelocity(const Vector3& vel);
 
	};
}


#endif // !HUGE_PHYSICS_PARTICLE_H

