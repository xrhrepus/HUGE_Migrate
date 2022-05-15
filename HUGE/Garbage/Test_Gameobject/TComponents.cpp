#include "TComponents.h"

using namespace H;
META_DERIVED_BEGIN(TestTimerComp,Component)
META_FIELD_BEGIN
META_FIELD(mTimeElapsed, "timeElapsed")
META_FIELD_END
META_CLASS_END


void TestTimerComp::Initialize()
{
	mTimeElapsed = 0.0f;
}

void TestTimerComp::Terminate()
{
}

void TestTimerComp::Update(float deltatime)
{
	mTimeElapsed += deltatime;
}

void TestTimerComp::Render()
{
}

void TestTimerComp::DebugUI()
{
	ImGui::Begin("TestTimerComp");
	ImGui::DragFloat("timeElapsed##TestTimerComp", &mTimeElapsed);
	if (ImGui::Button("reset##TestTimerComp"))
	{
		ResetTimer();
	}
	ImGui::End();

}

void TestTimerComp::ResetTimer()
{
	mTimeElapsed = 0.0f;
}
