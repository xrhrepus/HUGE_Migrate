
#include "MyAppState.h"
 
 
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
//void ShowDebugUi(float deltatime);

void NavalGame::Initialize()
{
	debugDatas.resize(1);
	debugDatas[0] = std::make_unique<float>(1.0f);

	srand(std::time(nullptr));
	//ai world
	world.Initialize(world_row, world_col, world_cellSize);
	bs = std::make_unique<BattleShip>(world);
	bs->Init();
	bs->PassDataPtr(debugDatas);
	bs->position = { 500.0f,500.0f };
	//texture
	alphaSheepTexture = TextureManager::Get()->LoadTexture(L"alphaSheep64.png");
	dest = TextureManager::Get()->LoadTexture(L"fruit42.png");
	house = TextureManager::Get()->LoadTexture(L"house128.png");
	rulerMap = TextureManager::Get()->LoadTexture(L"rulermap.jpg");

 
 
	//camera
	camera.SetPosition({ 0.0f,0.0f, -10.0f });
	camera.SetDirection({ 0.0f,0.0f, 1.0f });
 }

void NavalGame::Terminate()
{
 }

void NavalGame::Render()
{
	bs->Render();
	SimpleDraw::Render(camera);

 	
}

void NavalGame::Update(float deltatime)
{
	bs->Update(deltatime);
	world.Update(deltatime);



	if (InputSystem::Get()->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		bs->destination.x = static_cast<float>(InputSystem::Get()->GetMouseScreenX());
		bs->destination.y = static_cast<float>(InputSystem::Get()->GetMouseScreenY());
	}

 
 
}

void NavalGame::DebugUI()
{
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("NavalGame");
	ImGui::Text("[-----------]");
	//if (ImGui::CollapsingHeader("Alpha sheep control"))
	//{
	//	if (ImGui::Checkbox("  Seek##bool", &aSeek))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<Seek>("Seek")->SetActive(aSeek);
	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("  Arrive##bool", &aArrive))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(aArrive);
	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("  wander##bool", &aWander))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<Wander>("Wander")->SetActive(aWander);
	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("  obsAvoid##bool", &aObsAvoid))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid")->SetActive(aObsAvoid);
	//	}
 //
	//	if (ImGui::Checkbox("  Separation##bool", &aSeperation))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<Separation>("Separation")->SetActive(aSeperation);
	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("  Alignment##bool", &aAlignment))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<Alignment>("Alignment")->SetActive(aAlignment);
	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("  Cohesion##bool", &aCohesion))
	//	{
	//		alphaSheep->mSteeringModule->GetBehavior<Cohesion>("Cohesion")->SetActive(aCohesion);
	//	}


	//}
 //
	//ImGui::Text("destination :[%f,%f ]", alphaSheep->destination.x, alphaSheep->destination.y);
 //
	//ImGui::TextDisabled("[ --------------------- ] ");
	//if (ImGui::Button("getneighbour"))
	//{
	//	Agents test = world.GetNeighborhood({ alphaSheep->position,100.0f });
	//	neighborsize = test.size();
	//}
	//ImGui::Text("neighbor: %d", neighborsize);

	//
	//if (ImGui::CollapsingHeader("Group control"))
	//{
	//	if (ImGui::Checkbox("Group wander##bool", &groupWander))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Wander>("Wander")->SetActive(groupWander);
	//		}

	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("Group Arrive##bool", &groupArrive))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(groupArrive);
	//		}

	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("Group obsAvoid##bool", &groupObsAvoid))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Wander>("ObsAvoid")->SetActive(groupObsAvoid);
	//		}

	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("Group Seek##bool", &groupSeek))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Wander>("Seek")->SetActive(groupSeek);
	//		}

	//	}

	//	if (ImGui::Checkbox("Group Separation##bool", &groupSeperation))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Separation>("Separation")->SetActive(groupSeperation);
	//		}

	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("Group Alignment##bool", &groupAlignment))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Alignment>("Alignment")->SetActive(groupAlignment);
	//		}

	//	}
	//	ImGui::SameLine();

	//	if (ImGui::Checkbox("Group Cohesion##bool", &groupCohesion))
	//	{
	//		for (auto& s : sheeps)
	//		{
	//			s->mSteeringModule->GetBehavior<Cohesion>("Cohesion")->SetActive(groupCohesion);
	//		}

	//	}

	//	ImGui::DragFloat("Seek Force", &data1,0.1f,0.0f,20.0f);
	//	ImGui::DragFloat("Seperation Force", &data2, 0.1f, 0.0f, 20.0f);
	//	ImGui::DragFloat("Align Force", &data3, 0.1f, 0.0f, 20.0f);
	//	ImGui::DragFloat("Cohesion Force", &data4, 0.1f, 0.0f, 20.0f);



	//}
	

  


 	ImGui::End();

}
