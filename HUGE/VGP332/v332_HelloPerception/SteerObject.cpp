#include "SteerObject.h"

using namespace H::AI;
#pragma region WolfFunction
Sheep::Sheep(H::AI::AIWorld & world)
	:Agent(world)
{
	viewAngle = H::Math::Constants::Pi * 0.5f;
	viewRadius = 150.0f;
	cosViewAngle = cosf(viewAngle);
	heading = { 1.0f,0.0f };
}
void Sheep::Init(std::vector<Circle>* cptrs)
{
	position = { 600.0f,600.0f };
	destination = { 0.0f,0.0f };
	
	mTextureId = TextureManager::Get()->LoadTexture(L"sheep64.png");

	maxSpeed = 300.0f;

	//SteeringModule
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

	//PerceptionModule
	mPerceptionModule = std::make_unique<H::AI::PerceptionModule>(*this, [](H::AI::MemoryRecord&){});
	mPerceptionModule->AddSensor<VisualSensor>("VisualSensor");



	//PassDataPtr();

 }
void Sheep::PassDataPtr()
{
	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->d1 = &data1;
	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->d2 = &data2;
	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->d3 = &data3;
	static_cast<ObsAvoid*>(mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid"))->d4 = &data4;
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

 	Move(deltatime);
	mPerceptionModule->Update(deltatime);
}
void Sheep::Render()
{

//	SimpleDraw::AddScreenCircle({ cv , wRad }, Colors::Cyan);
//SimpleDraw::AddScreenCircle({ agent.destination , 5.0f }, Colors::Cyan);
	SimpleDraw::AddScreenLine(position, position + heading * viewRadius , Colors::Magenta);


	H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mTextureId)), { position.x - 32.0f, position.y - 32.0f });

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

#pragma region steer


//[Steering]==================================================

H::Math::Vector2 Seek::Calculate(H::AI::Agent & agent)
{
	auto desierdV = H::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed ;
	return desierdV - agent.velocity;
}

H::Math::Vector2 Arrive::Calculate(H::AI::Agent & agent)
{
	float distance = H::Math::Distance(agent.destination, agent.position);
 
	float desiredSpeed;

	if (distance > arriveDistance)
	{
		desiredSpeed = agent.maxSpeed;
	}
	else
	{
		desiredSpeed = agent.maxSpeed * (distance / arriveDistance);
		//desiredSpeed = agent.maxSpeed * H::Math::Truncate(distance,arriveDistance);

	}
 


 	auto desierdV = H::Math::Normalize(agent.destination - agent.position) * desiredSpeed;
 	return desierdV - agent.velocity;
}
H::Math::Vector2 Wander::Calculate(H::AI::Agent & agent)
{

	localWT = H::Math::Normalize(localWT + Vector2( (float)(rand() % 100 + (-50)) ,(float)(rand() % 100 + (-50)) )) * wRad;

	Vector2 wanderTarget = localWT + Vector2{ 0.0f, wDist };

	Matrix4 l2w = agent.LocalToWorld();

	Vector3 newdest = H::Math::TransformCoord(Vector3{ wanderTarget }, l2w);
	agent.destination.x = newdest.x;
	agent.destination.y = newdest.y;

	//SimpleDraw::AddScreenCircle({ wanderTarget,50.0f }, Colors::Cyan);
	Vector2 cv;
	
	cv.x = (H::Math::TransformCoord(Vector3{ 0.0f, wDist,0.0f },l2w)).x;
	cv.y = (H::Math::TransformCoord(Vector3{ 0.0f, wDist,0.0f }, l2w)).y;


	//SimpleDraw::AddScreenCircle({ cv , wRad }, Colors::Cyan);
	//SimpleDraw::AddScreenCircle({ agent.destination , 5.0f }, Colors::Cyan);
	//SimpleDraw::AddScreenLine(agent.position , agent.destination, Colors::Cyan);

 

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

	agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.radius });
	for (auto n : agent.neighbors)
	{
		if (n == &agent)
			continue;

		H::Math::Vector2 sepVec = H::Math::Normalize(agent.position - n->position);
		float dst = H::Math::Magnitude(sepVec);
		sepVec /= dst;
		desierdV += sepVec;
	}
	desierdV *= force;
	SimpleDraw::AddScreenCircle({ agent.position + desierdV, 5.0f }, Colors::Cyan);
	SimpleDraw::AddScreenLine(agent.position, agent.position + desierdV, Colors::Cyan);

	return desierdV - agent.velocity;
}

H::Math::Vector2 Alignment::Calculate(H::AI::Agent & agent)
{
	H::Math::Vector2 desierdV;
	agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.radius });

	H::Math::Vector2 avgHeading;
	for (auto n : agent.neighbors)
	{
		if (n == &agent)
			continue;
		avgHeading += n->heading;
	}

	if (agent.neighbors.size() > 1)
	{
		avgHeading /= agent.neighbors.size() - 1;
		desierdV = H::Math::Normalize(avgHeading - agent.heading);
		desierdV *= force;

		SimpleDraw::AddScreenCircle({ agent.position + desierdV, 5.0f }, Colors::Magenta);
		SimpleDraw::AddScreenLine(agent.position, agent.position + desierdV, Colors::Magenta);
		return desierdV - agent.velocity;

	}

	return Vector2();
}

H::Math::Vector2 Cohesion::Calculate(H::AI::Agent & agent)
{
	H::Math::Vector2 desierdV;
	agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.radius });

	H::Math::Vector2 avgPosition;
	for (auto n : agent.neighbors)
	{
		if (n == &agent)
			continue;
		avgPosition += n->position;
	}
	if (agent.neighbors.size() > 1)
	{
		avgPosition /= agent.neighbors.size() - 1;
		desierdV = H::Math::Normalize(avgPosition - agent.position);
		desierdV *= force;

		SimpleDraw::AddScreenCircle({ agent.position + desierdV, 5.0f }, Colors::LawnGreen);
		SimpleDraw::AddScreenLine(agent.position, agent.position + desierdV, Colors::LawnGreen);


		return desierdV - agent.velocity;
	}

	return Vector2();
}
#pragma endregion

void VisualSensor::Update(Agent & agent, MemoryRecords & memory, float deltatime)
{
	
	//agent.neighbors = agent.world.GetNeighborhood({ agent.position, agent.viewRadius });
	const Agents& agnts = agent.world.GetAllAgents();
	for (auto n : agnts)
	{
		if (n == &agent)
			continue;
		if (H::Math::Distance(n->position , agent.position) < agent.viewRadius)
		{
			if (H::Math::Dot(H::Math::Normalize(n->position - agent.position), agent.heading)
		> agent.cosViewAngle)
			{
				SimpleDraw::AddScreenLine(agent.position, n->position, Colors::LawnGreen);
				SimpleDraw::AddScreenRect({ n->position,{60.0f,60.0f} }, Colors::Magenta);
			}

		}

	}


	//1 dist check, 2 angle , 3 raycast
	//identipt = 0.0 
	//identiptforgetTime = = ..
	//float cosVireagnle = cos viewagnel * 0.5f

	//for ( all sth)
	//if distSqr > viewrange^2 , continue
	//if agent.heading dot normal(sth.pos - agent.pos) < socviewrange continue
	//

	//find in mem
	//if found - update record
	// else - add new record;
}
