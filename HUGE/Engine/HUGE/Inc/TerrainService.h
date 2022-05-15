#ifndef HUGE_TERRAINSERVICE_H
#define HUGE_TERRAINSERVICE_H
#include "Service.h"
#include "Terrain.h"

namespace H
{
	class CameraService;

	class TerrainService : public Service
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
		Terrain mTerrain;
		int mNumRows = 20;
		int mNumCols = 20;
		float mCellSize = 10.0f;
	};
 

}

#endif // !HUGE_TERRAINSERVICE_H

