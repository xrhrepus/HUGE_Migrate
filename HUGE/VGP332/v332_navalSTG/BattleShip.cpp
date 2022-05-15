#include "BattleShip.h"
#include "SteerBehaviors.h"

BattleShip::BattleShip(H::AI::AIWorld & world)
	:Ship(world)
{
}

void BattleShip::Init()
{
	destination = { 0.0f,0.0f };
	maxSpeed = 5.0f;
	mSteeringModule = std::make_unique<H::AI::SteeringModule>(*this);

	mSteeringModule->AddBehavior<Seek>("Seek")->SetActive(true);

}

void BattleShip::Render()
{
	SimpleDraw::AddScreenRect({ position,{2.0f,2.0f} }, Colors::Cyan);

	//hull
	const float lUnit = 5.0f;
	auto l2w = LocalToWorld();

	Vector2 v0(-lUnit, 2.5f * lUnit);
	Vector2 v1(lUnit, 2.5f * lUnit);
	Vector2 v2(0.0f, 8.0f * lUnit);//head

	Vector2 v3(-lUnit, -lUnit * 4.0f);
	Vector2 v4(lUnit, -lUnit * 4.0f );
	Vector2 v5(0.0f, -lUnit * 6.0f);

	Vector3 v3_0{ H::Math::TransformCoord(Vector3{v0},l2w) };
	Vector3 v3_1{ H::Math::TransformCoord(Vector3{v1},l2w) };
	Vector3 v3_2{ H::Math::TransformCoord(Vector3{v2},l2w) };

	Vector3 v3_3{ H::Math::TransformCoord(Vector3{v3},l2w) };
	Vector3 v3_4{ H::Math::TransformCoord(Vector3{v4},l2w) };
	Vector3 v3_5{ H::Math::TransformCoord(Vector3{v5},l2w) };


	v0.x = v3_0.x;
	v0.y = v3_0.y;

	v1.x = v3_1.x;
	v1.y = v3_1.y; 
	
	v2.x = v3_2.x;
	v2.y = v3_2.y;

	v3.x = v3_3.x;
	v3.y = v3_3.y;

	v4.x = v3_4.x;
	v4.y = v3_4.y;

	v5.x = v3_5.x;
	v5.y = v3_5.y;

	//SimpleDraw::AddScreenRect({ position,{10.0f,20.0f} },Colors::Cyan);
	//SimpleDraw::AddScreenLine(v0, v1, Colors::Cyan);
	SimpleDraw::AddScreenLine(v1,v2, Colors::Cyan);
	SimpleDraw::AddScreenLine(v2, v0, Colors::Cyan);

	SimpleDraw::AddScreenLine(v0, v3, Colors::Cyan);
	SimpleDraw::AddScreenLine(v1, v4, Colors::Cyan);

	//SimpleDraw::AddScreenLine(v3, v4, Colors::Cyan);
	SimpleDraw::AddScreenLine(v4, v5, Colors::Cyan);
	SimpleDraw::AddScreenLine(v5, v3, Colors::Cyan);

	//guns
	v0 = { 0.0f, lUnit * 5.0f };
	v1 = { 0.0f, lUnit * 3.0f };
	v2 = { 0.0f, lUnit * -4.5f };

	v3_0 = { H::Math::TransformCoord(Vector3{v0},l2w) };
	v3_1 = { H::Math::TransformCoord(Vector3{v1},l2w) };
	v3_2 = { H::Math::TransformCoord(Vector3{v2},l2w) };


	v0.x = v3_0.x;
	v0.y = v3_0.y;

	v1.x = v3_1.x;
	v1.y = v3_1.y;

	v2.x = v3_2.x;
	v2.y = v3_2.y;

	SimpleDraw::AddScreenCircle({ v0,lUnit * 0.5 }, Colors::Orange);
	SimpleDraw::AddScreenCircle({ v1,lUnit * 0.5 }, Colors::Orange);
	SimpleDraw::AddScreenCircle({ v2,lUnit * 0.5 }, Colors::Orange);


}

void BattleShip::Move(float deltatime)
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

void BattleShip::Aim()
{
}

void BattleShip::Steer()
{
}

void BattleShip::Fire()
{
}

void BattleShip::Die()
{
}

void BattleShip::InflictDamage()
{
}

void BattleShip::PassDataPtr(const std::vector<std::unique_ptr<float>>& datas)
{
	mSteeringModule->GetBehavior<Seek>("Seek")->force = datas[0].get();
}
