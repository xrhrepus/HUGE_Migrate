#pragma once

#include "Ship.h"


class BattleShip : public Ship
{
public:
	BattleShip(H::AI::AIWorld& world);
	~BattleShip() = default;
	void Init()override;
	void Render()override;

	void Move(float deltatime)	override;
	void Aim()override;

	void Steer()override;
	void Fire()override;
	void Die()override;
	void InflictDamage()override;

	void PassDataPtr(const std::vector<std::unique_ptr<float>>& datas)override;
private:

	std::vector<Vector2> mMesh;
};
 