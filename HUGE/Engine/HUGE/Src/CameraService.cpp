#include "Precompiled.h"
#include "CameraService.h"
using namespace H;
META_DERIVED_BEGIN(CameraService, Service)
META_FIELD_BEGIN
META_FIELD_END
META_CLASS_END

void H::CameraService::DebugUI()
{
	mCameraList[mActiveCameraIndex].camera.DebugUI();
}

H::Graphics::Camera * H::CameraService::AddCamera(const char * name)
{
	auto cam = FindCamera(name);
	if (cam == nullptr)
	{
		auto& entry = mCameraList.emplace_back(name);
		cam = &entry.camera;
	}
	return cam;
}

H::Graphics::Camera * H::CameraService::FindCamera(const char * name)
{
	auto iter = std::find_if(mCameraList.begin(), mCameraList.end(), [name](auto& entry)
	{
		return entry.name == name;
	});
	return (iter == mCameraList.end()) ? nullptr : &iter->camera;
}

void H::CameraService::SetActiveCamera(const char * name)
{
	for (size_t i = 0; i < mCameraList.size(); ++i)
	{
		auto& entry = mCameraList[i];
		if (entry.name == name)
		{
			mActiveCameraIndex = static_cast<int>(i);
			break;
		}
	}
 
}
H::Graphics::Camera& H::CameraService::GetActiveCamera()
{
	ASSERT(mActiveCameraIndex < mCameraList.size(), "[CameraService] no active camera");
	return mCameraList[mActiveCameraIndex].camera;
}

const H::Graphics::Camera& H::CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCameraIndex < mCameraList.size(), "[CameraService] no active camera");
	return mCameraList[mActiveCameraIndex].camera;
}
