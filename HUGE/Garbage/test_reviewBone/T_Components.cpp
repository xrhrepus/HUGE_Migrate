#include "T_Components.h"
Matrix4 T_Transform::computeTransform() const
{
	return  Matrix4::scaling(scale) * H::Math::MatrixRotationQuaternion(rotationQuaternion) * Matrix4::translation(pos);
}

void T_Transform::DebugUI()
{
	if (ImGui::CollapsingHeader("TTransform"))
	{
		bool rotChanged = false;

		ImGui::DragFloat3("pos", pos.v.data(), 0.1f);
		if (ImGui::DragFloat3("rot", rot.v.data(), 0.1f))
		{
			rotChanged = true;
		}
		ImGui::Text("Quaternion:[ x: %f , y: %f , z: %f , w: %f ]", rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z, rotationQuaternion.w);

		ImGui::DragFloat3("scale", scale.v.data(), 0.1f);

		if (rotChanged)
		{
			rotationQuaternion =
				H::Math::QuaternionRotationAxis(Vector3::yAxis(), rot.y) *
				H::Math::QuaternionRotationAxis(Vector3::xAxis(), rot.x) *
				H::Math::QuaternionRotationAxis(Vector3::zAxis(), rot.z);
		}
	}

}

void T_Light::DebugUI()
{
	if (ImGui::CollapsingHeader("Directional light"))
	{
		bool dirChange = false;
		if (ImGui::DragFloat3("dir", light.direction.v.data(), 0.1f))
		{
			dirChange = true;
		}
		ImGui::ColorEdit3("light diffuse", &light.diffuse.r, 0.1f);
		ImGui::ColorEdit3("light ambient", &light.ambient.r, 0.1f);
		ImGui::ColorEdit3("light specular", &light.specular.r, 0.1f);

		if (dirChange)
		{
			light.direction = H::Math::Normalize(light.direction);
		}
	}
}
