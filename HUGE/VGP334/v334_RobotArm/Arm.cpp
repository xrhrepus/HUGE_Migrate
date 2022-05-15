#include "Arm.h"
#include "MyAppState.h"
using namespace H::Graphics;
using namespace H::Math;

void Arm::Init()
{
	mTexture = TextureManager::Get()->LoadTexture("sponge_bob.png");

 	mBones.reserve(100);
	mBoneTransCtrls.reserve(100);
	mBoneRotCtrls.reserve(100);
	mBoneMatrix.reserve(100);
	AddArm(-1);

	//AnimationBuilder ab;
	//mAnim = ab
	//	.AddRotationKey(H::Math::Quaternion({ 0.0,0.0f,0.0f,1.0f }), 0.0f)
	//	.AddRotationKey(H::Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, H::Math::Constants::Pi), 0.5f*  mAnim_LoopTime)
	//	.AddRotationKey(H::Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, H::Math::Constants::TwoPi), mAnim_LoopTime)
 //		.Build();

 
}

void Arm::Terminate()
{
}

void Arm::AddArm(int parentindex)
{
	//add mesh
	mMeshes.push_back(MeshBuilder::CreateCube(mArmExtend.x, mArmExtend.y, mArmExtend.z, { mArmExtend.z,0.0f,0.0f }));
	mMeshBufs.emplace_back();
	mMeshBufs.back().Initialize(mMeshes.back());

	//add bone
	mBoneMatrix.emplace_back();
	mBones.emplace_back();
	//mBones.push_back(Bone());

	auto& thisBone = mBones[mNumMeshes];
	thisBone.name = "bone" + std::to_string(mNumMeshes);
	thisBone.index = mNumMeshes;
	thisBone.parentIndex = parentindex;
	thisBone.parent = thisBone.parentIndex > -1 ? &(mBones[thisBone.parentIndex]) : nullptr;
	thisBone.toParentTransform = H::Math::Matrix4::Identity();

	if (parentindex > -1)
	{
		mBones[thisBone.parentIndex].children.push_back(&thisBone);
		mBones[thisBone.parentIndex].childIndices.push_back(thisBone.index);
	}
 

	//add debug ctrl
	mBoneTransCtrls.emplace_back(H::Math::Vector3());
	mBoneRotCtrls.emplace_back(H::Math::Vector3());

	UpdateTransforms(&thisBone, mBoneMatrix);
	mNumMeshes++;

}

void Arm::Render(H::Graphics::TypedConstantBuffer<TransformData>& transBuf, TransformData& transData, Matrix4& view, Matrix4& perspective)
{
	Matrix4 comp;
	int index = 0;
	for (const auto& mbf : mMeshBufs)
	{

		comp = mBoneMatrix[index] * mAnim.GetTransform(mAnim_Timer) * (view) * (perspective);
		
		transData.world = H::Math::Transpose(mBoneMatrix[index] );
		transData.wvp = H::Math::Transpose(comp);
		transBuf.Set(transData);

		mbf.Render();
		index++;
	}

}

void Arm::Update(float dt)
{
	mAnim_Timer = mAnim_Timer < mAnim_LoopTime ? mAnim_Timer + dt : 0.0f;
}

void Arm::DeBugUI()
{
	ImGui::Begin("test arm");
	if (ImGui::CollapsingHeader("Bone"))
	{
		ImGui::Text("existing bone : %d", mNumMeshes);

		ImGui::Text("current bone : ");
		ImGui::SameLine();
		if (ImGui::ArrowButton("prev", 0)) mBone_currChoose -= mBone_currChoose > 0 ? 1 : 0;
		ImGui::SameLine();
		ImGui::Text(" % d ", mBone_currChoose);
		ImGui::SameLine();
		if (ImGui::ArrowButton("next", 1)) mBone_currChoose += mBone_currChoose < mNumMeshes - 1 ? 1 : 0;
		ImGui::SameLine();
		ImGui::Text("parent % d ", mBones[mBone_currChoose].parentIndex);



		if (ImGui::Button("add arm")) AddArm(mBone_currChoose);
 		if (mNumMeshes > 0)
		{
			Vector3 prevTrans = mBoneTransCtrls[mBone_currChoose];
			bool isTransChanged = false;
			isTransChanged |= (ImGui::DragFloat("translate x", &mBoneTransCtrls[mBone_currChoose].x, 0.01f));
			isTransChanged |= (ImGui::DragFloat("translate y", &mBoneTransCtrls[mBone_currChoose].y, 0.01f));
			isTransChanged |= (ImGui::DragFloat("translate z", &mBoneTransCtrls[mBone_currChoose].z, 0.01f));

			if (isTransChanged)
			{
				Matrix4 m;
				m.Translation(mBoneTransCtrls[mBone_currChoose] - prevTrans);
				mBones[mBone_currChoose].toParentTransform = /*mBoneMatrix[mBone_currChoose]*/ mBones[mBone_currChoose].toParentTransform* m;
				UpdateTransforms(&(mBones[mBone_currChoose]), mBoneMatrix);
			}


			bool rotationChanged = false;
			Vector3 prevRot = mBoneRotCtrls[mBone_currChoose];
			//ImGui::Text("[Rotation]: x[%f],y[%f],z[%f] ", mBoneRotAngles[mBone_currChoose].x, mBoneRotAngles[mBone_currChoose].y, mBoneRotAngles[mBone_currChoose].z);

			if (ImGui::DragFloat("rotation x", &mBoneRotCtrls[mBone_currChoose].x, 0.01f/*,-H::Math::Constants::Pi, H::Math::Constants::Pi*/))
			{
				//mBoneRotAngles[mBone_currChoose].x += mBoneRotCtrls[mBone_currChoose].x - prevRot.x;
				mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 1.0f,0.0f,0.0f }, mBoneRotCtrls[mBone_currChoose].x - prevRot.x)) * mBones[mBone_currChoose].toParentTransform;
				UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
				rotationChanged = true;

			}
			if (ImGui::DragFloat("rotation y", &mBoneRotCtrls[mBone_currChoose].y, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
			{
				//mBoneRotAngles[mBone_currChoose].y += mBoneRotCtrls[mBone_currChoose].y - prevRot.y;

				mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, mBoneRotCtrls[mBone_currChoose].y - prevRot.y))* mBones[mBone_currChoose].toParentTransform;

				UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
				rotationChanged = true;

			}
			if (ImGui::DragFloat("rotation z", &mBoneRotCtrls[mBone_currChoose].z, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
			{
				//mBoneRotAngles[mBone_currChoose].x += mBoneRotCtrls[mBone_currChoose].z - prevRot.z;

				mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 0.0f,0.0f,1.0f }, mBoneRotCtrls[mBone_currChoose].z - prevRot.z))* mBones[mBone_currChoose].toParentTransform;

				UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
				rotationChanged = true;
			}




			if (ImGui::Button("reset bone"))
			{
				for (auto& c : mBoneTransCtrls)
				{
					c = Vector3();
				}
				for (auto& c : mBoneRotCtrls)
				{
					c = Vector3();
				}
				mBones[mBone_currChoose].toParentTransform = H::Math::Matrix4::Identity();
				UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);

			}
			//current bone
			SimpleDraw::AddLine({ 0.0f, 0.0f ,0.0f }, { mBoneMatrix[mBone_currChoose] * Vector3{ 0.0f, 0.0f ,0.0f } }, Colors::YellowGreen);
			/*if (mBones[mBone_currChoose].parentIndex > -1)
			{
				SimpleDraw::AddLine({ mBoneMatrix[mBone_currChoose] * Vector3{ 0.0f, 0.0f ,0.0f } }, (mBoneMatrix[mBones[ mBone_currChoose].parentIndex] ) * Vector3{ 0.0f, 0.0f ,0.0f }, Colors::Red);
			}*/
			for (const auto& ci : mBones[mBone_currChoose].childIndices)
			{
				SimpleDraw::AddLine({ mBoneMatrix[mBone_currChoose] * Vector3{ 0.0f, 0.0f ,0.0f } }, (mBoneMatrix[ci]) * Vector3 { 0.0f, 0.0f, 0.0f }, Colors::Red);

			}

		}




	}

	if (ImGui::CollapsingHeader("Animation"))
	{
		ImGui::DragFloat("at time : %f", &mAnim_addTime, 0.1f, 0.0f, mAnim_LoopTime);

		if (ImGui::Button("add position key"))
		{
			mAnimbuilder.AddPositionKey({ mBoneMatrix[mBone_currChoose] * Vector3{ 0.0f, 0.0f ,0.0f } }, mAnim_addTime);
		}
		//if (ImGui::Button("add rotation key"))
		//{
		//	mAnimbuilder.AddRotationKey({ mBoneMatrix[mBone_currChoose] * Vector3{ 0.0f, 0.0f ,0.0f } }, mAnim_addTime);
		//}

		if (ImGui::Button("build animation"))
		{
			mAnim = mAnimbuilder.Build();
		}

	}
	ImGui::End();


}
