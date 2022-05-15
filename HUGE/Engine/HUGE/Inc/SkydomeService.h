#ifndef HUGE_SKYDOMESERVICE_H
#define HUGE_SKYDOMESERVICE_H
#include "Service.h"
#include "Skydome.h"

namespace H
{
	class CameraService;

	class SkydomeService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Initialize()override;
		void Terminate()override;
		//void Update(float deltatime) {}
		void Render() override;
		void DebugUI()override;

	private:
		const CameraService* mCameraService = nullptr;
		Skydome mSkydome;
		//Terrain mTerrain;
		//int mNumCols = 20;
		//float mCellSize = 10.0f;
	};
 

}

#endif // !HUGE_TERRAINSERVICE_H

