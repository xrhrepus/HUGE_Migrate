#include "C_Transform.h"
 
C_Transform::C_Transform()
	:position(Vector3(0.0f)),
	rotation(Vector3(0.0f)),
	scale(Vector3(1.0f)),
	rotationQuaternion(H::Math::Quaternion(0.0f,0.0f,0.0f,1.0f))
{
}

void C_Transform::Initialize()
{
}

void C_Transform::Terminate()
{
}

void C_Transform::Update(float deltatime)
{
}

void C_Transform::Render()
{
}

void C_Transform::DebugUI()
{
	if (ImGui::CollapsingHeader("C_Transform"))
	{
 		bool changed = false;
		bool rotChanged = false;

		if (ImGui::DragFloat3("pos", position.v.data(), 0.2f))
			changed = true;
		if (ImGui::DragFloat3("rot", rotation.v.data(), 0.2f))
			rotChanged = true;

		ImGui::Text("Quaternion:[ x: %f , y: %f , z: %f , w: %f ]", rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z, rotationQuaternion.w);
 
		if (ImGui::DragFloat3("sca", scale.v.data(), 0.2f))
			changed = true;


		if (ImGui::Button("reset"))
		{
			position = H::Math::Vector3::Zero();
			rotation = H::Math::Vector3::Zero();
			scale = H::Math::Vector3{ 1.0f };
			changed = true;
			rotChanged = true;

		}

		if (rotChanged)
		{
			rotationQuaternion =
				H::Math::QuaternionRotationAxis(Vector3::yAxis(), rotation.y)*
				H::Math::QuaternionRotationAxis(Vector3::xAxis(), rotation.x)*
				H::Math::QuaternionRotationAxis(Vector3::zAxis(), rotation.z);

			//rotationQuaternion = H::Math::QuaternionFromRotationMatrix(H::Math::Matrix4::rotation(rotation.x, rotation.y, rotation.z));
		}
 	}

}

void C_Transform::ResetTransform()
{
	position = Vector3{ 0.0f };
	rotation = Vector3{ 0.0f };
	scale = Vector3{ 1.0f };
	rotationQuaternion = H::Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 C_Transform::GetTransformMatrix() const
{
	return 
		H::Math::Scaling(scale)*
		H::Math::MatrixRotationQuaternion(rotationQuaternion)*
		H::Math::Translation(position);
}
