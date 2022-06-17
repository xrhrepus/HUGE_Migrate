#include "Precompiled.h"
#include "TransformComponent.h"

using namespace H;

META_DERIVED_BEGIN(TransformComponent,Component)
META_FIELD_BEGIN
META_FIELD(position,"Position")
META_FIELD(rotation, "Rotation")
META_FIELD_END
META_CLASS_END


void H::TransformComponent::Initialize()
{
}

void H::TransformComponent::Render()
{
	H::Graphics::SimpleDraw::AddLine(position, { position.x+10.0f,position.y,position.z }, H::Graphics::Colors::Red);
	H::Graphics::SimpleDraw::AddLine(position, { position.x ,position.y + 10.0f,position.z }, H::Graphics::Colors::Green);
	H::Graphics::SimpleDraw::AddLine(position, { position.x ,position.y,position.z + 10.0f }, H::Graphics::Colors::Blue);

}
void H::TransformComponent::DebugUI()
{
	if (ImGui::TreeNode("Transform"))
	{
		bool rotChanged = false;

		ImGui::DragFloat3("pos", position.v.data(), 0.1f);
		if (ImGui::DragFloat3("rot", rotation.v.data(), 0.1f))
		{
			rotChanged = true;
		}
		ImGui::Text("Quaternion:[ x: %f , y: %f , z: %f , w: %f ]", rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);

		ImGui::DragFloat3("scale", scale.v.data(), 0.1f);

		if (rotChanged)
		{
			rotationQuat =
				H::Math::QuaternionRotationAxis(Vector3::yAxis(), rotation.y) *
				H::Math::QuaternionRotationAxis(Vector3::xAxis(), rotation.x) *
				H::Math::QuaternionRotationAxis(Vector3::zAxis(), rotation.z);
		}
		ImGui::TreePop();
	}
 
}

Matrix4 H::TransformComponent::computeTransform() const
{
	return Matrix4::scaling(scale) * H::Math::MatrixRotationQuaternion(rotationQuat) * Matrix4::translation(position);
}
