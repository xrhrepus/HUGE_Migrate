#include "Precompiled.h"
#include "CameraService.h"
#include "SkydomeService.h"
#include "World.h"

using namespace H;

META_DERIVED_BEGIN(SkydomeService, Service)
META_NO_FIELD
//META_FIELD_BEGIN
//META_FIELD(mNumRows, "NumRows")
//META_FIELD(mNumCols, "NumCols")
//META_FIELD(mCellSize, "CellSize")

//META_FIELD_END
META_CLASS_END

void H::SkydomeService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();
	//apply deserialize data to terrain
	mSkydome.Initialize();

}

void H::SkydomeService::Terminate()
{
	mSkydome.Terminate();

}

void H::SkydomeService::Render()
{
	auto& cam = (mCameraService->GetActiveCamera());
	mSkydome.Render(cam);
}

void H::SkydomeService::DebugUI()
{
	mSkydome.DrawEditorUI();
}
