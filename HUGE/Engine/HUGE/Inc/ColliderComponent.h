#ifndef HUGE_COLLIDERCOMPONENT_H
#define HUGE_COLLIDERCOMPONENT_H
#include "Component.h"

namespace H
{
	class ColliderComponent : public Component
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Render() override;
		void Update(float dt)override;
		void DebugUI()override;

		Math::Vector3 center;
		Math::Vector3 extend;

	};
 
}

#endif // !HUGE_COLLIDERCOMPONENT_H



