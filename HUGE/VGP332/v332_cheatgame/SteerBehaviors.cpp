#include "SteerBehaviors.h"

//[Steering]==================================================
 

H::Math::Vector2 Seek::Calculate(H::AI::Agent & agent)
{
	auto desierdV = H::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed * (force);
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
	auto desierdV = H::Math::Normalize(-agent.heading) * desiredSpeed;

	return desiredSpeed == 0 ? H::Math::Vector2{0.0f} : desierdV - agent.velocity;
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


	Seek::force = *force;
	return Seek::Calculate(agent);
}

H::Math::Vector2 ObsAvoid::Calculate(H::AI::Agent & agent)
{
	agent.GetNeighborFromWorld();
	Vector2 compDesirForce(0.0f);
	for (const auto n : agent.neighbors)
	{
		if (n == &agent || n->mTag == 1)
		{
			continue;
		}
		Vector2 agentToCircle = n->position - agent.position;
		//SimpleDraw::AddScreenLine(agent.position, c->mCentre, Colors::Red);

		if (Dot(agentToCircle, agent.heading) > 0.0f) //if front ?
		{
			Vector2 circle_localY = (H::Math::Projection(agentToCircle, agent.heading));
			Vector2 circle_localX = (circle_localY - agentToCircle);

			//SimpleDraw::AddScreenLine(agent.position, (circle_localY + agent.position), Colors::Magenta);
			//SimpleDraw::AddScreenLine(c.centre + circle_localX, c.centre, Colors::LawnGreen);


			float xDist = H::Math::Magnitude(circle_localX); // circle x local 
			float outterRadius = (n->radius + n->radius);

			if (xDist < outterRadius)//if x overlap ?
			{

				float boxlen = avoidDist; // length of collision box
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

					compDesirForce += desierdV;
					//return desierdV - agent.velocity;

				}

			}


		}

	}
	auto& obss = agent.world.GetObstabcles();
	for (const auto& c : obss)
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
			float outterRadius = (c.radius + 15.0f);

			if (xDist < outterRadius)//if x overlap ?
			{

				float boxlen = avoidDist; // length of collision box
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
					compDesirForce += desierdV;

					//return desierdV - agent.velocity;1

				}

			}


		}



	}




	return compDesirForce;
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

