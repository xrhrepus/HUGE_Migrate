#include "Precompiled.h"
#include "TerrainService.h"
#include "CameraService.h"
#include "World.h"

using namespace H;

META_DERIVED_BEGIN(TerrainService, Service)
META_FIELD_BEGIN
META_FIELD(mNumRows, "NumRows")
META_FIELD(mNumCols, "NumCols")
META_FIELD(mCellSize, "CellSize")

META_FIELD_END
META_CLASS_END


void H::TerrainService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();
	//apply deserialize data to terrain
	mTerrain.Initialize(mNumRows,mNumCols,mCellSize);
}

void H::TerrainService::Terminate()
{
	mTerrain.Terminate();
}

void H::TerrainService::Render()
{
	auto& cam = (mCameraService->GetActiveCamera());
	mTerrain.Render(cam);
}

void H::TerrainService::DebugUI()
{
	mTerrain.DrawEditorUI();
}
