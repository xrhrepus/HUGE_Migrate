#pragma once
#include <HUGE/Inc/HUGE.h>
class LineBone
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void DrawBone(const H::Math::Vector3& from, const H::Math::Vector3& to) const;
	void DrawBoneRecursive(H::Graphics::Bone* root) const;
	void Update(float deltatime);
	void DebugUI();

private:
	float mSize{ 3.0f };
	H::Graphics::Color mColor{ H::Graphics::Colors::Yellow };
	//H::Math::Vector3 mFrom{ H::Math::Vector3::Zero()};
	//H::Math::Vector3 mTo{ H::Math::Vector3::Zero() };
	H::Graphics::Bone* mRootBone;
};
 