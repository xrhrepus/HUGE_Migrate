#pragma once
#include <HUGE/Inc/HUGE.h>

struct tBoneTransformData;
struct tTransformData;

class Tentacle
{
public:
	void Init();
	void Terminate();

 
	void Render(H::Graphics::TypedConstantBuffer<tTransformData>& transBuf, tTransformData& transData,
		H::Graphics::TypedConstantBuffer<tBoneTransformData>& boneTransBuf, 
		Matrix4& view, Matrix4& perspective);
	void Update(float dt);
	void DeBugUI();


	int mBone_currChoose = 0;
	size_t mNumBones = 4;
private:
	//tentacle
	Vector3 mTranslation;

	Vector3 mRotation;

	TextureId mTexture;
	Matrix4 mMeshTransform = Matrix4::Identity();
	BoneMesh mBonemesh;
	MeshBuffer mBoneMeshBuffer;

	//bone
	std::vector<Bone> mBones;
	std::vector<H::Math::Matrix4> mBoneMatrix;
	std::vector<H::Math::Matrix4> mBoneTranslateMatrix;

 	std::vector<H::Math::Vector3>mBoneRotCtrls;

};
 