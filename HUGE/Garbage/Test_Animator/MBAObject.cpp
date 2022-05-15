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

	//
	mSkinningCtx.directionalLight.direction = {1.0f,0.0f,0.0f};
	mSkinningCtx.directionalLight.diffuse = { 1.0f,1.0f, 1.0f, 1.0f };
	mSkinningCtx.directionalLight.specular = { 1.0f,1.0f, 1.0f, 1.0f };
	mSkinningCtx.directionalLight.ambient = { 1.0f,1.0f, 1.0f, 1.0f };
	mSkinningCtx.material.diffuse = { 1.0f,1.0f, 1.0f, 1.0f };
	mSkinningCtx.material.specular = { 1.0f,1.0f, 1.0f, 1.0f };
	mSkinningCtx.material.ambient = { 1.0f,1.0f, 1.0f, 1.0f };
	mSkinningCtx.material.power = 1.0f;
 
	mLineSkeleton.Init();
}

void MBAObject::Terminate()
{
	mModel.Terminate();
	mLineSkeleton.Terminate();
}

void MBAObject::Update(float deltatime)
{
	mBoneSecExtraTransform.Update(deltatime);
	mLineSkeleton.Update(deltatime);
	mAnimator.Update(deltatime);
}
void MBAObject::Render(const Matrix4& viewMat, const Matrix4& projMat, const Camera& cam)
{
	//compute transform
	mDirection.dir = H::Math::TransformCoord(Vector3::zAxis(), H::Math::MatrixRotationQuaternion(mTransform.rotationQuaternion));
	Matrix4 w = mTransform.GetTransformMatrix();
	Matrix4 comp = w * viewMat * projMat;
	mSkinningCtx.transformData.world = H::Math::Transpose(w);
	mSkinningCtx.transformData.wvp = H::Math::Transpose(comp);
	mSkinningCtx.transformData.viewPosition = cam.GetPosition();
	//light,mat in debugui

	//bone transform
	mSkinningCtx.boneTransform = mAnimator.ComputeToParentTransform(mBoneMatrices);

	//texture
	if (mDrawModel)
	{
		RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
		SamplerManager::Get()->GetSampler("LinearWrap")->BindPS(0);


		auto sknEffect = ShaderEffectManager::Get()->GetEffect<ShaderEffect_Skinning>("Skinning");
		//for (auto& md : mModel.materialData)
		//{
		//	mSkinningCtx.diffuse = md.diffuseMapId;
		//	mSkinningCtx.specular = md.specularMapId;
		//	mSkinningCtx.normal = md.normalMapId;
		//	//render
		//	sknEffect->SetContextInfo(&mSkinningCtx);
		//	sknEffect->Bind();

		//}
		////mModel.Render();
		//mModel.meshData[1].meshBuffer.Render();
		//mModel.meshData[2].meshBuffer.Render();
		//mModel.meshData[3].meshBuffer.Render();

		//sknEffect->UnBind();
		
 
		/*for (auto& md : mModel.meshData)
		{
			mSkinningCtx.meshBuffer = &md.meshBuffer;
			mSkinningCtx.diffuse = mModel.materialData[md.materialIndex].diffuseMapId;
			mSkinningCtx.specular = mModel.materialData[md.materialIndex].specularMapId;
			mSkinningCtx.normal = mModel.materialData[md.materialIndex].normalMapId;
			sknEffect->Render(&mSkinningCtx);
		}*/
		for (size_t i = 0; i < 2; i++)
		{
			auto& md = mModel.meshData[i];
			mSkinningCtx.meshBuffer = &md.meshBuffer;
			mSkinningCtx.diffuse = mModel.materialData[md.materialIndex].diffuseMapId;
			mSkinningCtx.specular = mModel.materialData[md.materialIndex].specularMapId;
			mSkinningCtx.normal = mModel.materialData[md.materialIndex].normalMapId;
			sknEffect->Render(&mSkinningCtx);

		}
 	}

	mLineSkeleton.Render();

}

 

void MBAObject::DebugUI()
{
	ImGui::Begin("MBAObject");
	ImGui::Checkbox("Draw Model", &mDrawModel);
	if (ImGui::CollapsingHeader("Skinning Context"))
	{
		if(ImGui::TreeNode("Light"))
		{
			bool directionChanged = false;
			if (ImGui::DragFloat3("Direction##Light", mSkinningCtx.directionalLight.direction.v.data(), 0.1f))
				directionChanged = true;
			if (directionChanged)
			{
				mSkinningCtx.directionalLight.direction = Math::Normalize(mSkinningCtx.directionalLight.direction);
			}

			ImGui::ColorEdit4("Ambient##Light", &mSkinningCtx.directionalLight.ambient.r);
			ImGui::ColorEdit4("Diffuse##Light", &mSkinningCtx.directionalLight.diffuse.r);
			ImGui::ColorEdit4("Specular##Light", &mSkinningCtx.directionalLight.specular.r);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Material"))
		{
			ImGui::ColorEdit4("Ambient##Material", &mSkinningCtx.material.ambient.r);
			ImGui::ColorEdit4("Diffuse##Material", &mSkinningCtx.material.diffuse.r);
			ImGui::ColorEdit4("Specular##Material", &mSkinningCtx.material.specular.r);
			ImGui::DragFloat("Power##Material", &mSkinningCtx.material.power, 1.0f, 0.0f, 100.0f);
			ImGui::TreePop();
		}
 
		if (ImGui::TreeNode("Transform"))
		{
			mTransform.DebugUI();
 			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Bonesec_ExtraTransform"))
		{
			mBoneSecExtraTransform.DebugUI();
			ImGui::TreePop();
		}

	}
	mLineSkeleton.DeBugUI();
	mAnimator.DebugUI();
	ImGui::End();
}
