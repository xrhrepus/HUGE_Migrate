#pragma once

#include <HUGE\Inc\HUGE.h>
using namespace H;

class MainState : public H::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void Update(float deltatime) override;
	void DebugUI() override;

public:
	void RenderScene();

 private:
	RenderTarget mSceneRenderTarget;

	Camera* mCamera = nullptr;
	DirectionalLight mLight;


 	World mWorld;
	std::unique_ptr<Editor> mEditor;
};

