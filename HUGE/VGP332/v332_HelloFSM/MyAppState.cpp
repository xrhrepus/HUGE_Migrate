
#include "MyAppState.h"

 
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
//void ShowDebugUi(float deltatime);

void WorkingSheep::Initialize()
{
	//init system
 
	bushes.emplace_back(Bush({ 200.0f,200.0f }));
	bushes.emplace_back(Bush({ 500.0f,209.0f }));
	bushes.emplace_back(Bush({ 700.0f,309.0f }));
	bushes.emplace_back(Bush({ 900.0f,209.0f }));



	//Bush b1({ 20.0f,20.0f });
	for (auto& b : bushes)
	{
		b.SetTextureId(TextureManager::Get()->LoadTexture(L"bush64.png"));
		sheep.AddTargetBush(&b);
	}
	//sheep.AddTargetBush((b1));
	sheep.SetTextureId(TextureManager::Get()->LoadTexture(L"sheep64.png"));
	house = TextureManager::Get()->LoadTexture(L"house128.png");
}

void WorkingSheep::Terminate()
{
 }

void WorkingSheep::Render()
{
	for (auto& b : bushes)
	{
		b.Render();
	}
	sheep.Render();
	SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(house), sheep.GetHomePosition());

}

void WorkingSheep::Update(float deltatime)
{
	sheep.Update(deltatime);

	for (auto& b : bushes)
	{
		b.Update(deltatime);
	}

}

void WorkingSheep::DebugUI()
{
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("WorkingSheep");

	ImGui::Text("current state: %s ", sheep.GetCurrentState().c_str());

	ImGui::DragFloat("move speed ", &(sheep.GetMoveSpeed()));
	ImGui::Text("FruitAtHome: %d", sheep.GetFruitAtHome());
	ImGui::Text("FruitOwn: %d", sheep.GetFruitOwned());

	ImGui::Text("FruitInBush1: %d", bushes[0].GetCurrentFruitNum());
	ImGui::Text("  -Bush1: %f,%f", bushes[0].GetPosition().x, bushes[0].GetPosition().y);
	ImGui::Text("  -Bush1 priority:  %d", sheep.GetBushPriority()[0]);

	ImGui::Text("FruitInBush2: %d", bushes[1].GetCurrentFruitNum());
	ImGui::Text("  -Bush2: %f,%f", bushes[1].GetPosition().x, bushes[1].GetPosition().y);
	ImGui::Text("  -Bush2 priority:  %d", sheep.GetBushPriority()[1]);

	ImGui::Text("FruitInBush3: %d", bushes[2].GetCurrentFruitNum());
	ImGui::Text("  -Bush3: %f,%f", bushes[2].GetPosition().x, bushes[2].GetPosition().y);
	ImGui::Text("  -Bush3 priority:  %d", sheep.GetBushPriority()[2]);

	ImGui::Text("FruitInBush4: %d", bushes[3].GetCurrentFruitNum());
	ImGui::Text("  -Bush4: %f,%f", bushes[3].GetPosition().x, bushes[3].GetPosition().y);
	ImGui::Text("  -Bush4 priority:  %d", sheep.GetBushPriority()[3]);

 

 	ImGui::End();

}
