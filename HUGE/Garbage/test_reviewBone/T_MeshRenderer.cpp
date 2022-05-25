#include "T_MeshRenderer.h"

#pragma region Abstract render material
void T_Material::Bind() const
{
	mShaderEffect->SetContextInfo(*mShaderEffectContext);
	mShaderEffect->Bind();
}

void T_Material::UnBind() const
{
	mShaderEffect->UnBind();
}
void T_Material::SetTransform(const TransformData & data)
{
	//do nothing default
}
void T_Material::SetCamera(const Camera & cam)
{
	//do nothing default
}
void T_Material::SetLight(const DirectionalLight & light)
{
	//do nothing default
}
#pragma endregion

#pragma region Render mat

void Standard_Material::Init()
{
	mShaderEffect = (&ShaderEffectManager::Get()->GetEffect("Standard"));
	mShaderEffectContext = mShaderEffect->CreateShaderEffectContext();
}

void Standard_Material::SetTransform(const TransformData& data)
{
	mShaderEffectContext->transformData = data;
}

void Standard_Material::SetCamera(const Camera& cam)
{
	mShaderEffectContext->camera = &cam;
}

void Standard_Material::SetLight(const DirectionalLight& light)
{
	static_cast<ShaderEffect_Standard::SE_Context_Standard&>(*mShaderEffectContext).directionalLight = light;
}

void Standard_Material::SetDiffuseTexture(TextureId tid)
{
	static_cast<ShaderEffect_Standard::SE_Context_Standard&>(*mShaderEffectContext).diffuse = tid;
}

void Standard_Material::DebugUI()
{
	if (ImGui::CollapsingHeader("Render Material"))
	{
		mShaderEffectContext->DebugUI();
	}
}

#pragma endregion

#pragma region Mesh renderer
T_MeshRenderer::T_MeshRenderer()
{
}

void T_MeshRenderer::SetCamera(const Camera& cam)
{
	mCurrentCam = &cam;
}

void T_MeshRenderer::SetMesh(const Mesh& mesh)
{
	mMesh = &mesh;
	mMeshBuffer.Initialize(*mMesh);
}

void T_MeshRenderer::SetMaterial(T_Material& mat)
{
	mMat = &mat;
}

void T_MeshRenderer::SetLight(T_Light& light)
{
	mLight = &light;
}

void T_MeshRenderer::SetTransform(T_Transform& transform)
{
	mTransform = &transform;
}

void T_MeshRenderer::PreRender()
{
	Matrix4 vm = mCurrentCam->GetViewMatrix();
	Matrix4 pm = mCurrentCam->GetPerspectiveMatrix();
	Matrix4 world = mTransform->computeTransform();
	mMat->SetTransform(TransformData{ H::Math::Transpose(world) ,H::Math::Transpose(world * vm * pm) ,mCurrentCam->GetPosition() });
	mMat->SetCamera(*mCurrentCam);
	mMat->SetLight(mLight->light);
	mMat->Bind();
}

void T_MeshRenderer::Render() const
{
	mMeshBuffer.Render();
}

void T_MeshRenderer::PostRender()
{
	mMat->UnBind();
}

void T_MeshRenderer::DebugUI()
{
	ImGui::Begin("Mesh Renderer");
	mLight->DebugUI();
	mTransform->DebugUI();
	mMat->DebugUI();
	ImGui::End();
}

#pragma endregion

Matrix4 T_Transform::computeTransform() const
{
	return  Matrix4::scaling(scale)  * H::Math::MatrixRotationQuaternion(rotationQuaternion)* Matrix4::translation(pos);
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
				H::Math::QuaternionRotationAxis(Vector3::yAxis(), rot.y)*
				H::Math::QuaternionRotationAxis(Vector3::xAxis(), rot.x)*
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
