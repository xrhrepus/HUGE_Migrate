#include <Graphics/Inc/Graphics.h>
 #include "TRaycast.h"
using namespace H::Graphics;
 
namespace 
{
 
}

void TRaycast::Init()
{
	auto gSys = GraphicSystem::Get();
	step_w = 2.0f / gSys->GetBackBufferWidth() ;
	step_h = -2.0f / gSys->GetBackBufferHeight();
}

Ray TRaycast::CastRay(int mouseScreenX, int mouseScreenY, float farDistance, const Vector3& camPos,const  Matrix4& invVP)
{
	//Vector3 posInWorld = H::Math::TransformCoord(Vector3{ static_cast<float>( mouseScreenX),static_cast<float>(mouseScreenY),farDistance }, v2w);
	Vector3 posInWorld = ConvertScreenToNDC(mouseScreenX, mouseScreenY,farDistance, invVP);
	
	SimpleDraw::AddLine(Vector3::Zero(), posInWorld, Colors::Orange);
	//SimpleDraw::AddBox({ posInWorld,6.0f }, Colors::OrangeRed);
	
	Ray r(camPos, H::Math::Normalize(posInWorld - camPos));
	return r;
}

Vector3 TRaycast::ConvertScreenToNDC(int mouseScreenX, int mouseScreenY, float farDistance, const Matrix4& invVP)
{
	auto gSys = GraphicSystem::Get();
 
	Vector3 ndcV(static_cast<float>(mouseScreenX) * step_w - 1.0f, static_cast<float>(mouseScreenY) * step_h + 1.0f, 0.0f);
	_ndcV = ndcV;
	posWorld = H::Math::TransformCoord(ndcV, invVP);
	return  posWorld;
}

void TRaycast::DebugUI()
{
	ImGui::Begin("ray");
	ImGui::DragFloat3("_ndcV", _ndcV.v.data());
	ImGui::DragFloat3("_tfV", _tfV.v.data());
	ImGui::DragFloat3("normal_tfV", normal_tfV.v.data());
	ImGui::DragFloat3("pinworld", posWorld.v.data());

	ImGui::End();

}
