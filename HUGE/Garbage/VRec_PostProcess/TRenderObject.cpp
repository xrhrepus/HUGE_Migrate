#include "TRenderObject.h"

void TRenderObject::Initialize(const Mesh & mesh, ShaderEffect_Standard::SE_Context_Standard & ctx, std::string name)
{
	SetMeshContext(mesh, ctx);
	mName = std::move(name);
	mTransform.Initialize();
	mMeshRenderer.Initialize();

}

void TRenderObject::Initialize(std::string name)
{
	mName = std::move(name);
	mTransform.Initialize();
	mMeshRenderer.Initialize();
}

void TRenderObject::Terminate()
{
}

void TRenderObject::Update(float deltatime)
{
}

void TRenderObject::Render()
{
	auto& ctx = mMeshRenderer.mStandardContext;
	auto& tf = ctx.transformData;

	auto world = mTransform.GetTransformMatrix();
	auto comp = world * ctx.camera->GetViewMatrix() * ctx.camera->GetPerspectiveMatrix();
	tf.world = H::Math::Transpose(world);
	tf.wvp = H::Math::Transpose(comp);
	tf.viewPosition = ctx.camera->GetPosition();

	mMeshRenderer.Render();

}

void TRenderObject::DebugUI()
{
	if (ImGui::CollapsingHeader(mName.c_str()))
	{
		mTransform.DebugUI();
		mMeshRenderer.DebugUI();
	}
	
	
 }

void TRenderObject::SetMeshContext(const Mesh& mesh, ShaderEffect_Standard::SE_Context_Standard & ctx)
{
	mMeshRenderer.SetMesh(mesh);
	mMeshRenderer.SetContext(ctx);
}
