#pragma once
#include <HUGE/Inc/HUGE.h>

using namespace H;

class TestTimerComp : public H::Component
{
public:
	META_CLASS_DECLARE

	//TComponents();
	void Initialize() override;
	void Terminate() override;
	void Update(float deltatime) override;
	void Render() override;
	void DebugUI() override;

public:
	void ResetTimer();

private:

	float mTimeElapsed;

};
 