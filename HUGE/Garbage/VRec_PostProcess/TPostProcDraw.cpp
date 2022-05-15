#include "TPostProcDraw.h"

void TPostProcDraw::Initialize()
{
	mVS.Initialize(VertexPX::Format, POSTPROCESSING_FILE_PATH);
	mPS.Initialize(POSTPROCESSING_FILE_PATH, "PS");

	mScreenMesh = MeshBuilder::CreateNDCQuadPX();//create screen quad;
	mScreenMeshBuffer.Initialize(mScreenMesh);

}

void TPostProcDraw::Terminate()
{
	mVS.Terminate();
	mPS.Terminate();
	mScreenMeshBuffer.Terminate();

}

void TPostProcDraw::Begin()
{
	mVS.Bind();
	mPS.Bind();
}

void TPostProcDraw::Render()
{
	mScreenMeshBuffer.Render();
}

void TPostProcDraw::End()
{
	mVS.UnBind();
	mPS.UnBind();
}

void TPostProcDraw::DebugUI()
{
}
