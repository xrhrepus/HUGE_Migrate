#pragma once

#include <HUGE\Inc\HUGE.h>
using namespace H;

class EditState : public H::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Render() override;
	void Update(float deltatime) override;
	void DebugUI() override;
	
public:
	void ShowMenuBar();
	void ShowWindowWithDockSpace();
	void ShowSceneView();
	void ShowHierarchyView();
	void ShowInspectorView();

	void RenderScene();
private:
	RenderTarget mSceneRenderTarget;

	Camera* mCamera = nullptr;
	DirectionalLight mLight;


	GameObject mTester;
	
	//only editor + world
	World mWorld;
 };

 