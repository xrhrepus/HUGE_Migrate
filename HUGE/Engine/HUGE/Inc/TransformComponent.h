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

		Matrix4 computeTransform() const;
		Math::Vector3 position;
		Math::Vector3 rotation;
		Math::Vector3 scale{ 1.0f,1.0f,1.0f };
		Math::Quaternion rotationQuat{ 0.0f,0.0f ,0.0f ,1.0f };

	};

}
#endif // !HUGE_TRANSFORMCOMPONENT_H



