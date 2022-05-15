#pragma once
#include <HUGE/Inc/HUGE.h>
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
	void DrawBone(const H::Math::Vector3& from, const H::Math::Vector3& to)const;
private:
	Skeleton& mSkeleton;
	std::vector<H::Math::Matrix4>& mBoneMatrices;

	std::vector<H::Math::Vector3> mBonePos;

	bool draw = false;
	bool lineMode = false;
	bool updating = false;
	bool enableEdit = false;
  
	float mSize{ 3.0f };
	H::Graphics::Color mColor{ H::Graphics::Colors::Yellow };

};

 