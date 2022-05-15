#include "SteerObject.h"

#pragma region SheepFunction
Sheep::Sheep(H::AI::AIWorld & world)
	:Agent(world)
{

}
void Sheep::Init(std::vector<Circle>* cptrs)
{
	position = { 600.0f,600.0f };
	destination = { 0.0f,0.0f };

	mTextureId = TextureManager::Get()->LoadTexture(L"sheep32.png");

	maxSpeed = 300.0f;
	mSteeringModule = std::make_unique<H::AI::SteeringModule>(*this);

	mSteeringModule->AddBehavior<Wander>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<Seek>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<Arrive>("Arrive")->SetActive(false);
	mSteeringModule->AddBehavior<ObsAvoid>("ObsAvoid")->SetActive(false);
	mSteeringModule->AddBehavior<Separation>("Separation")->SetActive(false);
	mSteeringModule->AddBehavior<Alignment>("Alignment")->SetActive(false);
	mSteeringModule->AddBehavior<Cohesion>("Cohesion")->SetActive(false);



	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->cPtrs = cptrs;
	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->half_width = &radius;
	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->avoidDist = &avoidDist;

	//PassDataPtr();

 }
void Sheep::PassDataPtr(float* a, float* b, float* c, float* d)
{
	mSteeringModule->GetBehavior<Seek>("Seek")->force = a;
	mSteeringModule->GetBehavior<Wander>("Wander")->force = a;

	//float asdasd = *(mSteeringModule->GetBehavior<Seek>("Seek")->force);

	//static_cast<Seek*>(mSteeringModule->GetBehavior<Seek>("Seek"))->force = a;
	static_cast<Separation*>(mSteeringModule->GetBehavior<Separation>("Separation"))->force = b;
	static_cast<Alignment*>(mSteeringModule->GetBehavior<Alignment>("Alignment"))->force = c;
	static_cast<Cohesion*>(mSteeringModule->GetBehavior<Cohesion>("Cohesion"))->force = d;
}

void Sheep::Update(float deltatime)
{
	avoidDist = maxAvoidDist * (H::Math::Magnitude(velocity) / maxSpeed);
	if (position.y < -100.0f)
	{
		position.y = 820.0f;
	}
	if (position.y > 820.0f)
	{
		position = -100.0f;
	}
	if (position.x < -100.0f)
	{
		position.x = 1380.0f;
	}
	if (position.x > 1380.0f)
	{
		position.x = -100.0f;
	}

	GetNeighborFromWorld();
 	Move(deltatime);
}
void Sheep::Render()
{
	//SimpleDraw::AddScreenCircle({ position,radius }, Colors::DarkMagenta);
	//SimpleDraw::AddScreenLine(position,position + heading * H::Math::Magnitude(velocity), Colors::Magenta);
	//SimpleDraw::AddScreenRect({ position,radius }, Colors::Magenta);
	//SimpleDraw::AddScreenCircle({ position + heading * velocity,5.0f }, Colors::DarkCyan);

	H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mTextureId)), { position.x - mRenderOffset, position.y - mRenderOffset });

}
 void Sheep::SetDestination(H::Math::Vector2 dest)
{
	destination = dest;
}
 void Sheep::SetTextureId(H::Graphics::TextureId tid)
 {
	 mTextureId = tid;
 }

void Sheep::Move(float deltatime)
{
	auto force = mSteeringModule->Calculate();
	auto a = force / mass;
	velocity += a * deltatime;

	//Vector2 vunit = H::Math::Normalize(velocity);
	//float vmag = H::Math::Magnitude(velocity);

	//position += (vunit * (vmag> maxSpeed? maxSpeed:vmag) ) * deltatime;
	position += velocity * deltatime;

	if (H::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = H::Math::Normalize(velocity);
	}
}

#pragma endregion


//[Steering]==================================================

H::Math::Vector2 Seek::Calculate(H::AI::Agent & agent)
{
	auto desierdV = H::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed * (*force);
	return desierdV - agent.velocity;
}

H::Math::Vector2 Arrive::Calculate(H::AI::Agent & agent)
{
	float distance = H::Math::Distance(agent.destination, agent.position);
 
	float desiredSpeed = 0.0f;

	if (distance > arriveDistance)
	{
		desiredSpeed = 0.0f;
	}
	else
	{
		desiredSpeed = agent.maxSpeed * (distance / arriveDistance);

	}
 
		//desiredSpeed = agent.maxSpeed * H::Math::Truncate(distance,arriveDistance);


 	//auto desierdV = H::Math::Normalize(agent.destination - agent.position) * desiredSpeed;
	auto desierdV = H::Math::Normalize( - agent.heading) * desiredSpeed;

 	return desierdV - agent.velocity;
}
H::Math::Vector2 Wander::Calculate(H::AI::Agent & agent)
{
	//rand  point (P) on unit circle
	//p.x ^2 + p.y ^2 = 1
	//
	Vector2 P;
	P.x = (float)(rand() % 3 + (-1));
	P.y = sqrtf(1.0f - P.x * P.x);

	localWT += P * wJitter;

	localWT = H::Math::Normalize(localWT) * wRad;



	//localWT += Vector2((float)(rand() % 100 + (-50)), (float)(rand() % 100 + (-50)));

	//localWT = H::Math::Normalize(localWT) * wRad;

	Vector2 wanderTarget = localWT + Vector2{ 0.0f, wDist };

	Matrix4 l2w = agent.LocalToWorld();

	Vector3 newdest = H::Math::TransformCoord(Vector3{ wanderTarget }, l2w);
	agent.destination.x = newdest.x;
	agent.destination.y = newdest.y;

	

	//Vector2 cv;
	//cv.x = (H::Math::TransformCoord(Vector3{ 0.0f, wDist,0.0f },l2w)).x;
	//cv.y = (H::Math::TransformCoord(Vector3{ 0.0f, wDist,0.0f }, l2w)).y;
	//SimpleDraw::AddScreenCircle({ cv , wRad }, Colors::Cyan);
	//SimpleDraw::AddScreenCircle({ agent.destination , 5.0f }, Colors::Cyan);
	//SimpleDraw::AddScreenLine(agent.position , agent.destination, Colors::Cyan);
 

	Seek::force = force;
	return Seek::Calculate(agent);
}

H::Math::Vector2 ObsAvoid::Calculate(H::AI::Agent & agent)
{
	for (const auto& c : (*cPtrs))
	{
		Vector2 agentToCircle = c.centre - agent.position;
		//SimpleDraw::AddScreenLine(agent.position, c->mCentre, Colors::Red);

		if (Dot(agentToCircle, agent.heading) > 0.0f) //if front ?
		{
			Vector2 circle_localY = (H::Math::Projection(agentToCircle, agent.heading));
			Vector2 circle_localX = (circle_localY - agentToCircle);

			//SimpleDraw::AddScreenLine(agent.position, (circle_localY + agent.position), Colors::Magenta);
			//SimpleDraw::AddScreenLine(c.centre + circle_localX, c.centre, Colors::LawnGreen);


			float xDist = H::Math::Magnitude(circle_localX); // circle x local 
			float outterRadius = (c.radius + *half_width);

			if (xDist < outterRadius)//if x overlap ?
			{

				float boxlen = *avoidDist; // length of collision box
				float yDist = H::Math::Magnitude(circle_localY);// circle y local 


				if (boxlen > (yDist - outterRadius)) // if collision box is inside circle
				{
					//find intersect between [heading] and [outter circle]

					//Vector2 relative_Center(xDist, yDist);
					float intersectY_1 = yDist + sqrtf(((outterRadius)*(outterRadius)+(xDist)*(xDist)));
					float intersectY_2 = yDist - sqrtf(((outterRadius)*(outterRadius)+(xDist)*(xDist)));

					Vector2 far_intersection = { 0.0f, (H::Math::Maxf((intersectY_1),(intersectY_2))) };
					Vector2 far_intersect_world = agent.position + (agent.heading* far_intersection.y);



					Vector2 cloest_intersection = { 0.0f, (H::Math::Minf((intersectY_1),(intersectY_2))) }; // intersect in agent local

					Vector2 close_intersect_world = agent.position + (agent.heading* cloest_intersection.y);


					//SimpleDraw::AddScreenCircle({ close_intersect_world ,10.0f }, Colors::LightGreen);
					//SimpleDraw::AddScreenLine(far_intersect_world, close_intersect_world, Colors::LightGreen);
					//SimpleDraw::AddScreenCircle({ far_intersect_world ,10.0f }, Colors::Pink);


					//======
					float lateralF_ratio = 1 - (xDist / outterRadius);

					float lateralF_mag = agent.maxSpeed * lateralF_ratio * 20.0f;

					Vector2 lateralDir = H::Math::Normalize(circle_localX);
					Vector2 lateralForce = H::Math::Normalize(lateralDir) * lateralF_mag; //direction

					//SimpleDraw::AddScreenLine(agent.position, (lateralForce + agent.position), Colors::White);
 					//
					float breakF_ratio = 1 - H::Math::Truncate((H::Math::Magnitude(agentToCircle)), outterRadius);
					Vector2 breakForce = (-agent.heading) * (agent.maxSpeed * breakF_ratio);

 
					//SimpleDraw::AddScreenLine(agent.position, (breakForce + agent.position), Colors::LightPink);

					auto desierdV = lateralForce + breakForce;
					//SimpleDraw::AddScreenLine(agent.position, (desierdV + agent.position), Colors::Yellow);

					return desierdV - agent.velocity;

				}

			}


		}

	}




	return H::Math::Vector2();
}

H::Math::Vector2 Separation::Calculate(H::AI::Agent & agent)
{

	H::Math::Vector2 desierdV;

	//agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.radius });
	for (auto n : agent.neighbors)
	{
		if (n == &agent)
			continue;

		H::Math::Vector2 sepVec = H::Math::Normalize(agent.position - n->position);
		float dst = H::Math::Magnitude(sepVec);
		if (dst <= 0.0f)
		{
			return Vector2();

		}
		sepVec /= dst;
		desierdV += sepVec;
	}
	if (desierdV.x != 0.0f || desierdV.y != 0.0f)
	{
		desierdV = H::Math::Normalize(desierdV)* (*force) * (agent.maxSpeed);
		//SimpleDraw::AddScreenCircle({ agent.position + desierdV, 5.0f }, Colors::Cyan);
		//SimpleDraw::AddScreenLine(agent.position, agent.position + desierdV, Colors::Cyan);

		return desierdV - agent.velocity;
	}

	return Vector2();
}

H::Math::Vector2 Alignment::Calculate(H::AI::Agent & agent)
{
	H::Math::Vector2 desierdV;
	//agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.radius });

	H::Math::Vector2 avgHeading(0.0f);
	for (auto n : agent.neighbors)
	{
		//if (n == &agent)
		//	continue;
		avgHeading += n->heading;
	}

	if (agent.neighbors.size() > 1)
	{
		avgHeading /= static_cast<float>(agent.neighbors.size() /*- 1*/);
		desierdV = H::Math::Normalize(avgHeading - agent.heading);
		desierdV *= *force * agent.maxSpeed;

		//SimpleDraw::AddScreenCircle({ agent.position + desierdV, 5.0f }, Colors::Magenta);
		//SimpleDraw::AddScreenLine(agent.position, agent.position + desierdV, Colors::Magenta);
		return desierdV - agent.velocity;

	}

	return Vector2();
}

H::Math::Vector2 Cohesion::Calculate(H::AI::Agent & agent)
{
	H::Math::Vector2 desierdV;
	//agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.radius });

	H::Math::Vector2 avgPosition;
	for (auto n : agent.neighbors)
	{
		//if (n == &agent)
		//	continue;
		avgPosition += n->position;
	}
	if (agent.neighbors.size() > 1)
	{
		avgPosition /= agent.neighbors.size() /*- 1*/;
		desierdV = H::Math::Normalize(avgPosition - agent.position);
		desierdV *= *force * agent.maxSpeed;

		//SimpleDraw::AddScreenCircle({ agent.position + desierdV, 5.0f }, Colors::LawnGreen);
		//SimpleDraw::AddScreenLine(agent.position, agent.position + desierdV, Colors::LawnGreen);


		return desierdV - agent.velocity;
	}

	return Vector2();
}
