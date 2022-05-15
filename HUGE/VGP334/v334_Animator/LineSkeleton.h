#pragma once
#include <HUGE/Inc/HUGE.h>

class LineSkeleton
{
public:
 	void Init(std::filesystem::path filename);
	void Terminate();

 
	void Render( );
	void Update(float dt);
	void DeBugUI();


	int mBone_currChoose = 23;
	//size_t mNumBones = 4;

private:
	Skeleton mSkeleton;

	std::vector<H::Math::Vector3> mBonePos;

	std::vector<H::Math::Vector3> mBoneTransform;
	std::vector<H::Math::Vector3> mBoneRotation;


	std::vector<H::Math::Matrix4> mBoneMatrix;

};

 