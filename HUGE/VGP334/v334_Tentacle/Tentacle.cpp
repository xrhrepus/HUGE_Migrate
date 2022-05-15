#include "Tentacle.h"
#include "MyAppState.h"

void Tentacle::Init()
{
	mTexture = TextureManager::Get()->LoadTexture("sponge_bob.png");

	mBonemesh = MeshBuilder::CreateFourBoneTentacle(60.0f, 10.0f, 10, { 0.0f });
	mBoneMeshBuffer.Initialize(mBonemesh);

	mBones.resize(mNumBones);
	mBoneRotCtrls.resize(mNumBones);
	mBoneMatrix.resize(mNumBones);
	mBoneTranslateMatrix.resize(mNumBones);
	//add bone
	//first
	mBones[0].name = "bone_0";
	mBones[0].index = 0;
	mBones[0].parent = nullptr;
	mBones[0].parentIndex = -1;
	mBones[0].toParentTransform = H::Math::Matrix4::Identity();
	mBones[0].offsetTransform = Matrix4::Identity();
	UpdateTransforms(&mBones[0], mBoneMatrix);

	for (int i = 1; i < mNumBones; i++)
	{
		mBones[i].name = "bone_" + std::to_string(i);
		mBones[i].index = i;
		mBones[i].parent = &mBones[i - 1];
		mBones[i].parentIndex = i - 1;
		mBones[i].toParentTransform = H::Math::Matrix4::Identity(); //tm.H::Math::Matrix4::Translation({ 0.0f, 0.0f, 0.0f });
		mBones[i].offsetTransform =/* Matrix4::Identity() **/ Matrix4::translation({ 0.0f,0.0f,20.0f * i });
 		//mBones[i].toParentTransform = H::Math::Matrix4::translation({ 0.0f, 0.0f, 50.0f });

		mBones[i - 1].children.push_back(&mBones[i]);
		mBones[i - 1].childIndices.push_back(mBones[i].index);

		UpdateTransforms(&mBones[i], mBoneMatrix);

	}

	mBoneTranslateMatrix[0] = Matrix4::Identity();
	mBoneTranslateMatrix[1] = Matrix4::Identity() * Matrix4::translation({ 0.0f,0.0f,20.0f });
	mBoneTranslateMatrix[2] = Matrix4::Identity() * Matrix4::translation({ 0.0f,0.0f,40.0f });
	mBoneTranslateMatrix[3] = Matrix4::Identity() * Matrix4::translation({ 0.0f,0.0f,60.0f });
	


}

void Tentacle::Terminate()
{
	mBoneMeshBuffer.Terminate();
}

 
void Tentacle::Render(H::Graphics::TypedConstantBuffer<tTransformData>& transBuf, tTransformData& transData,
					  H::Graphics::TypedConstantBuffer<tBoneTransformData>& boneTransBuf,
					  Matrix4& view, Matrix4& perspective)
{

	tBoneTransformData btd;
	for (size_t i = 0; i < mBoneMatrix.size(); i++)
	{
		btd.boneTransform[i] = H::Math::Transpose(mBoneMatrix[i]) ;

	}
	boneTransBuf.Set(btd);
	boneTransBuf.BindVS(4);
	boneTransBuf.BindPS(4);

	Matrix4 comp = mMeshTransform * view * perspective;
	transData.world = H::Math::Transpose(mMeshTransform);
	transData.wvp = H::Math::Transpose(comp);



	transBuf.Set(transData);
	transBuf.BindVS(0);
	transBuf.BindPS(0);

	mBoneMeshBuffer.Render();
}

 

void Tentacle::Update(float dt)
{
}

void Tentacle::DeBugUI()
{
	ImGui::Begin("test tentacle");
	
	if (ImGui::CollapsingHeader("Tentacle world transform"))
	{
		Vector3 prevTrans = mTranslation;

		if (ImGui::DragFloat("_translation x", &mTranslation.x, 0.01f))
		{
			mMeshTransform *= Matrix4::translation(mTranslation - prevTrans)  ;
		}
		if (ImGui::DragFloat("_translation y", &mTranslation.y, 0.01f))
		{
			mMeshTransform *= Matrix4::translation(mTranslation - prevTrans);

		}
		if (ImGui::DragFloat("_translation z", &mTranslation.z, 0.01f))
		{
			mMeshTransform *= Matrix4::translation(mTranslation - prevTrans);
		}



		Vector3 prevRot = mRotation;
		if (ImGui::DragFloat("_rotation x",&mRotation.x, 0.01f))
		{
			mMeshTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(Vector3::xAxis(), mRotation.x - prevRot.x))*mMeshTransform;
		}
		if (ImGui::DragFloat("_rotation y", &mRotation.y, 0.01f))
		{
			mMeshTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(Vector3::yAxis(), mRotation.y - prevRot.y))*mMeshTransform;

		}
		if (ImGui::DragFloat("_rotation z", &mRotation.z, 0.01f))
		{
			mMeshTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(Vector3::zAxis(), mRotation.z - prevRot.z))*mMeshTransform;

		}

	}
	SimpleDraw::AddLine(mMeshTransform* Vector3::xAxis() * 30.0f, mMeshTransform*Vector3::Zero(), Colors::Red);
	SimpleDraw::AddLine(mMeshTransform* Vector3::yAxis() * 30.0f, mMeshTransform*Vector3::Zero(), Colors::Cyan);
	SimpleDraw::AddLine(mMeshTransform* Vector3::zAxis() * 90.0f, mMeshTransform*Vector3::Zero(), Colors::Yellow);
	//SimpleDraw::AddLine(mTranslation.x * 30.0f, mMeshTransform*Vector3::Zero(), Colors::Red);
	//SimpleDraw::AddLine(mTranslation.y * 30.0f, mMeshTransform*Vector3::Zero(), Colors::Cyan);
	//SimpleDraw::AddLine(mTranslation.z * 30.0f, mMeshTransform*Vector3::Zero(), Colors::Green);


	if(ImGui::CollapsingHeader("Tentacle bone ctrl"))
	{
		ImGui::Text("current bone : ");
		ImGui::SameLine();
		if (ImGui::ArrowButton("prev", 0)) mBone_currChoose -= mBone_currChoose > 0 ? 1 : 0;
		ImGui::SameLine();
		ImGui::Text(" % d ", mBone_currChoose);
		ImGui::SameLine();
		if (ImGui::ArrowButton("next", 1)) mBone_currChoose += mBone_currChoose < mNumBones - 1 ? 1 : 0;



		bool rotationChanged = false;
		Vector3 prevRot = mBoneRotCtrls[mBone_currChoose];
		if (ImGui::DragFloat("rotation x", &mBoneRotCtrls[mBone_currChoose].x, 0.01f/*,-H::Math::Constants::Pi, H::Math::Constants::Pi*/))
		{
			//mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 1.0f,0.0f,0.0f }, mBoneRotCtrls[mBone_currChoose].x - prevRot.x)) * mBones[mBone_currChoose].toParentTransform;
			//mBones[mBone_currChoose].toParentTransform = H::Math::Inverse(mBoneTranslateMatrix[mBone_currChoose]) 
			//												* (Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(mMeshTransform * Vector3::xAxis(), mBoneRotCtrls[mBone_currChoose].x - prevRot.x)) 
			//												* mBones[mBone_currChoose].toParentTransform) 
			//												* mBoneTranslateMatrix[mBone_currChoose];

			Matrix4 invTrans = (H::Math::Inverse(mBones[mBone_currChoose].offsetTransform));
			Vector3 axis = mMeshTransform * Vector3::xAxis();
			Matrix4 quatRot = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(axis, mBoneRotCtrls[mBone_currChoose].x - prevRot.x));
			mBones[mBone_currChoose].toParentTransform = invTrans * quatRot * mBones[mBone_currChoose].offsetTransform * mBones[mBone_currChoose].toParentTransform;
				
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);

 			rotationChanged = true;
		}
		if (ImGui::DragFloat("rotation y", &mBoneRotCtrls[mBone_currChoose].y, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
		{

			//mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 0.0f,1.0f,0.0f }, mBoneRotCtrls[mBone_currChoose].y - prevRot.y))* mBones[mBone_currChoose].toParentTransform;
			//mBones[mBone_currChoose].toParentTransform = H::Math::Inverse(mBoneTranslateMatrix[mBone_currChoose]) 
			//	* Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(mMeshTransform * Vector3::yAxis(), mBoneRotCtrls[mBone_currChoose].y - prevRot.y))
			//	* mBones[mBone_currChoose].toParentTransform 
			//	* mBoneTranslateMatrix[mBone_currChoose];

			mBones[mBone_currChoose].toParentTransform = (H::Math::Inverse(mBones[mBone_currChoose].offsetTransform))*
				Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(mMeshTransform * Vector3::yAxis(), mBoneRotCtrls[mBone_currChoose].y - prevRot.y)) * 
				mBones[mBone_currChoose].offsetTransform * mBones[mBone_currChoose].toParentTransform;
				

 			rotationChanged = true;
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);


		}
		if (ImGui::DragFloat("rotation z", &mBoneRotCtrls[mBone_currChoose].z, 0.01f/*, -H::Math::Constants::Pi, H::Math::Constants::Pi*/))
		{

			//mBones[mBone_currChoose].toParentTransform = Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis({ 0.0f,0.0f,1.0f }, mBoneRotCtrls[mBone_currChoose].z - prevRot.z))* mBones[mBone_currChoose].toParentTransform;
			//mBones[mBone_currChoose].toParentTransform = H::Math::Inverse( mBoneTranslateMatrix[mBone_currChoose]) * Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(mMeshTransform * Vector3::zAxis(), mBoneRotCtrls[mBone_currChoose].z - prevRot.z)) * mBones[mBone_currChoose].toParentTransform * mBoneTranslateMatrix[mBone_currChoose];
			mBones[mBone_currChoose].toParentTransform = (H::Math::Inverse(mBones[mBone_currChoose].offsetTransform))*
				Math::MatrixRotationQuaternion(Math::QuaternionRotationAxis(mMeshTransform * Vector3::zAxis(), mBoneRotCtrls[mBone_currChoose].z - prevRot.z)) *
				mBones[mBone_currChoose].offsetTransform * mBones[mBone_currChoose].toParentTransform;

 			rotationChanged = true;
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);

		}
		if (rotationChanged)
		{
			//mMeshTransform = mBones[mBone_currChoose].toParentTransform * mMeshTransform;

		}


		if (ImGui::Button("reset bone"))
		{
			for (auto& c : mBoneRotCtrls)
			{
				c = Vector3();
			}
			mBones[mBone_currChoose].toParentTransform = H::Math::Matrix4::Identity();
			UpdateTransforms(&mBones[mBone_currChoose], mBoneMatrix);
 

		}


	}

	ImGui::End();

}
