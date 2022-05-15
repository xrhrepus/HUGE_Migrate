#include "TPlane.h"

void TPlane::Init()
{
	mPlane.n = H::Math::Vector3::yAxis();
	mPlane.d = 0.0f;
}

void TPlane::Update(float dt)
{
}

void TPlane::Render()
{
	H::Graphics::SimpleDraw::AddLine(Vector3::Zero(), mLastIntersec,Colors::Yellow);
	H::Graphics::SimpleDraw::AddBox({ mLastIntersec,8.0f }, Colors::YellowGreen);
}

void TPlane::CheckIntersec(H::Math::Ray ray)
{
	mIntersec = H::Math::Intersect(ray, mPlane, mIntersecDistance);
	mLastIntersec = ray.org + ray.dir * mIntersecDistance;
}

void TPlane::DebugUI()
{
	ImGui::Begin("TPlane");

	ImGui::Text("intersec: %s", mIntersec ? "true" : "false");
	ImGui::Text("last position: [ %f,%f,%f ]", mLastIntersec.x, mLastIntersec.y, mLastIntersec.z);
	ImGui::Text("distance : %f", mIntersecDistance);
	ImGui::End();

}
