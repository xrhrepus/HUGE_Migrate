#include "MBAObject.h"
#include "Graphics/Src/MeshIO.h"
using namespace H;
namespace
{
	void UpdateTransforms(const Bone* bone, std::vector<H::Math::Matrix4>& bonematrix/*, const Matrix4& animTF*/)
	{
		if (bone->parent)
		{
			bonematrix[bone->index] =  (bone->toParentTransform   * bonematrix[bone->parentIndex] )/* *bonematrix[bone->index]*/;
		}
		else
		{
			bonematrix[bone->index] =    bone->toParentTransform /* * bonematrix[bone->index]*/;
		}

		for (auto child : bone->children)
			UpdateTransforms(child, bonematrix );
	}

	void BlendAnimTransforms(  Matrix4& from,   Matrix4& to, float frompercent)
	{
		from *= (1 - frompercent);
		from._14 = 0.0f; from._24 = 0.0f; from._34 = 0.0f; from._44 = 1.0f;

		to *= (frompercent);
		to._14 = 0.0f; to._24 = 0.0f; to._34 = 0.0f; to._44 = 1.0f;
	}
}
void MBAObject::Init()
{
 
	mModel.Initialize(PATH_MODEL);
	MeshIO::LoadSkeleton(mSkeleton, PATH_SKELETON);

	//mBoneMatrices.resize(mSkeleton.bones.size());
	for (size_t i = 0; i < mSkeleton.bones.size(); i++)
	{
		mBoneMatrices.emplace_back(Matrix4::Identity());
	}
	UpdateTransforms(mSkeleton.root, mBoneMatrices);

	boneTransformBuf.Initialize();

 
	mAnimator.Init();
}

void MBAObject::Terminate()
{
	mModel.Terminate();
	
}

void MBAObject::Update(float deltatime)
{
 
	mAnimator.Update(deltatime);

}
void MBAObject::Render(const H::Graphics::TransformData& tfdata, const H::Graphics::DirectionalLight& light,const H::Graphics::Material& mdata)
{
 	auto sknEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Skinning>("Skinning");
	sknEffect->Bind();

	sknEffect->SetValue_BoneTransformBuffer(mAnimator.ComputeToParentTransform(mBoneMatrices));

	for (const auto& mesh : mModel.meshData)
	{
		const auto& material = mModel.materialData[mesh.materialIndex];

		
		sknEffect->SetValue_TransformBuffer(tfdata);
		sknEffect->SetValue_LightBuffer(light);
		sknEffect->SetValue_MaterialBuffer(mdata);

		sknEffect->SetValue_DiffuseMap(material.diffuseMapId);
		sknEffect->SetValue_SpecularMap(material.specularMapId);
		sknEffect->SetValue_NormalMap(material.normalMapId);

		mesh.meshBuffer.Render();
	}
 }

 

void MBAObject::DebugUI()
{
	ImGui::Begin("Move");
	if (ImGui::Button("Stop"))
	{
		mSpeed = 0.0f;
	}

	ImGui::DragFloat("speed", &mSpeed, 0.1f);
	ImGui::DragFloat3("dir", mDirection.v.data(), 0.1f);
	ImGui::End();

	mAnimator.DebugUI();
	mTransform.DebugUI();
}

void MBAObject::Control(const Vector3& camDir, float dt)
{
	
}

void MBAObject::Move(const Vector3 & camDir, float dt)
{
	mDirection = 
		H::Math::MatrixRotationQuaternion(
			H::Math::QuaternionRotationAxis(
				H::Math::Vector3::yAxis(), H::Math::Constants::DegToRad * mTransform.mRotation.y)) 
		* -H::Math::Vector3::zAxis() ;
	//Vector3 prevDir = mTransform.mDirection;
	//Vector3 targDir{ camDir.x, mTransform.mPosition.y,camDir.z };
	//Vector3 steer = camDir - mTransform.mDirection;
	////mTransform.mPrevDirection = mTransform.mDirection;
	//mTransform.mDirection += (steer * dt) * 1.5f;
	////H::Math::Normalize(mTransform.mDirection);
	mTransform.mPosition += mDirection * mSpeed;

	SimpleDraw::AddLine(mTransform.mPosition, mTransform.mPosition + mDirection * 50.0f, Colors::Yellow);//
	SimpleDraw::AddBox({ mTransform.mPosition + mDirection*50.0f,5.0f }, Colors::LightGreen);
 
 
}
