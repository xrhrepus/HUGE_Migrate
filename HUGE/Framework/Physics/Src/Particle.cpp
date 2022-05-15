#include "Precompiled.h"
#include "Particle.h"
using namespace H::Physics;

void Particle::SetPosition(const Vector3& pos)
{
	//pos = laspos no motion
	position = pos;
	lastPosition = pos;
}

void Particle::SetVelocity(const Vector3& vel)
{
	lastPosition = position - vel;
}
