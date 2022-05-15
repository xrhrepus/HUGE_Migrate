#ifndef HUGE_TRANSFORMCOMPONENT_H
#define HUGE_TRANSFORMCOMPONENT_H
#include "Component.h"

namespace H
{
	class TransformComponent : public Component
	{
	public:
		META_CLASS_DECLARE

		
		void Initialize() override;
		void Render() override;
		void DebugUI()override;

		Math::Vector3 position;
		Math::Quaternion rotation;

	};

}
#endif // !HUGE_TRANSFORMCOMPONENT_H



