#pragma once
#include <HUGE/Inc/HUGE.h>
#include "C_Transform.h"
class LineSkeleton
{
public:
	LineSkeleton(std::vector<H::Math::Matrix4>& boneMats, Skeleton& Skeleton);
 	void Init();
	//void Init(std::filesystem::path filename);

	void Terminate();

 
	void Render( );
	void Update(float dt);
	void DeBugUI();


	int mBone_currChoose = 23;
	size_t mNumBones = 0;

private:
	Skeleton& mSkeleton;
	std::vector<H::Math::Matrix4>& mBoneMatrix;

	std::vector<H::Math::Vector3> mBonePos;

	bool draw = false;
	bool updating = false;
	bool enableEdit = false;
 	C_Transform mTransformCompute;
 
 
};

 