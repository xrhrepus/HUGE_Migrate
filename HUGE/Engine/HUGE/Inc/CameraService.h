#ifndef HUGE_CAMERASERVICE_H
#define HUGE_CAMERASERVICE_H

#include "Service.h"

namespace H
{
	class CameraService : public Service
	{
	public:
		META_CLASS_DECLARE;
		void DebugUI() override;

		H::Graphics::Camera* AddCamera(const char* name);
		H::Graphics::Camera* FindCamera(const char* name);

		void SetActiveCamera(const char* name);
		H::Graphics::Camera& GetActiveCamera();
		const H::Graphics::Camera& GetActiveCamera()const;

	private:
		struct Entry
		{
			explicit Entry(const char* name)
				:name(name)
			{}

			std::string name;
			H::Graphics::Camera camera;
		};

		std::vector<Entry> mCameraList;
		int mActiveCameraIndex = 0;
	};

 }

#endif // !HUGE_CAMERASERVICE_H

