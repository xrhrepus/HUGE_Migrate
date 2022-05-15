#pragma once
#include <HUGE\Inc\HUGE.h>

 
class Arm
{
public:
	void Init();
	void Terminate();

	void AddArm(int parnetindex);

	void Render(H::Graphics::TypedConstantBuffer<TransformData>& transBuf, TransformData& transData, Matrix4& view, Matrix4& perspective);
	void Update(float dt);
	void DeBugUI();


	int mNumMeshes = 0;
	Vector3 mArmExtend = {5.0f,2.0f,3.0f};
	int mBone_currChoose = 0;

private:
	TextureId mTexture;

 	//mesh
	std::vector<Mesh> mMeshes;
	std::vector<MeshBuffer> mMeshBufs;

	//ani
	float mAnim_Timer;
	float mAnim_LoopTime = 10.0f;
	float mAnim_addTime = 0.0f;
	H::Graphics::Animation mAnim;
	AnimationBuilder mAnimbuilder;

	//bone
	std::vector<Bone> mBones;
	std::vector<H::Math::Matrix4> mBoneMatrix;

	std::vector<H::Math::Vector3>mBoneTransCtrls;
	std::vector<H::Math::Vector3>mBoneRotCtrls;

};
 