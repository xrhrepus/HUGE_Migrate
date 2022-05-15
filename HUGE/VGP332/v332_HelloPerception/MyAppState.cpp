
#include "MyAppState.h"
 
 
using namespace H::Core;
using namespace H::Graphics;
using namespace H::Math;
using namespace H::Input;
 
void SteeringSheep::Initialize()
{
	srand(std::time(nullptr));
	//ai world
	world.Initialize(world_row, world_col, world_cellSize);

	//texture
	alphaSheepTexture = TextureManager::Get()->LoadTexture(L"alphaSheep64.png");
	dest = TextureManager::Get()->LoadTexture(L"fruit42.png");
	house = TextureManager::Get()->LoadTexture(L"house128.png");
	rulerMap = TextureManager::Get()->LoadTexture(L"rulermap.jpg");

 
	//control by user
	alphaSheep = std::make_unique<Sheep>(world);

	alphaSheep->Init(&circles);
	alphaSheep->SetTextureId(alphaSheepTexture);
	alphaSheep->position = { 500.0f,500.0f };
	alphaSheep->destination = { 200.0f,200.0f };
 
	
	//group of sheep
	sheeps.resize(num_of_sheep);
	for (int i = 0; i < num_of_sheep; i++)
	{
 		sheeps[i] = std::make_unique<Sheep>(world);
		sheeps[i]->Init(&circles);
 
	}

	//circles 
	const int num_of_circle = 10;
	circles.resize(num_of_circle);
 	for (int i = 0; i < num_of_circle; i++)
	{
		circles[i] = Circle({ {static_cast<float>(rand() % 1000 + 100),static_cast<float>(rand() % 500 + 100) },20.0f });
	}

	//camera
	camera.SetPosition({ 0.0f,0.0f, -10.0f });
	camera.SetDirection({ 0.0f,0.0f, 1.0f });
 }

void SteeringSheep::Terminate()
{
 }

void SteeringSheep::Render()
{

	SimpleDraw::Render(camera);
	//SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(rulerMap), {0.0f,0.0f});

	//SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(house), { c1.mCentre.x - 64.0f, c1.mCentre.y - 64.0f });
	
	SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(dest), { alphaSheep->destination.x -21.0f , alphaSheep->destination.y - 21.0f });

 	alphaSheep->Render();

	for (const auto& s : sheeps)
	{
		s->Render();
	}
	
}

void SteeringSheep::Update(float deltatime)
{
 
	world.Update(deltatime);


	SimpleDraw::AddScreenLine(alphaSheep->position, alphaSheep->position + (alphaSheep->heading * alphaSheep->avoidDist), Colors::Cyan);
	SimpleDraw::AddScreenCircle({ alphaSheep->position + (alphaSheep->heading * alphaSheep->avoidDist),5.0f }, Colors::PeachPuff);

	for (const auto& c : circles)
	{
		SimpleDraw::AddScreenCircle(c, Colors::AliceBlue);
	}

	if (InputSystem::Get()->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		alphaSheep->destination.x = static_cast<float>(InputSystem::Get()->GetMouseScreenX());
		alphaSheep->destination.y = static_cast<float>(InputSystem::Get()->GetMouseScreenY());
	}

 
	alphaSheep->Update(deltatime);

	for (auto& s : sheeps)
	{
 		s->Update(deltatime);
	}

}

void SteeringSheep::DebugUI()
{
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("SteeringSheep");
	ImGui::Text("[-----------]");
	if (ImGui::CollapsingHeader("Alpha sheep control"))
	{
		if (ImGui::Checkbox("  Seek##bool", &aSeek))
		{
			alphaSheep->mSteeringModule->GetBehavior<Seek>("Seek")->SetActive(aSeek);
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("  Arrive##bool", &aArrive))
		{
			alphaSheep->mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(aArrive);
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("  wander##bool", &aWander))
		{
			alphaSheep->mSteeringModule->GetBehavior<Wander>("Wander")->SetActive(aWander);
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("  obsAvoid##bool", &aObsAvoid))
		{
			alphaSheep->mSteeringModule->GetBehavior<ObsAvoid>("ObsAvoid")->SetActive(aObsAvoid);
		}
 
		if (ImGui::Checkbox("  Separation##bool", &aSeperation))
		{
			alphaSheep->mSteeringModule->GetBehavior<Separation>("Separation")->SetActive(aSeperation);
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("  Alignment##bool", &aAlignment))
		{
			alphaSheep->mSteeringModule->GetBehavior<Alignment>("Alignment")->SetActive(aAlignment);
		}
		ImGui::SameLine();

		if (ImGui::Checkbox("  Cohesion##bool", &aCohesion))
		{
			alphaSheep->mSteeringModule->GetBehavior<Cohesion>("Cohesion")->SetActive(aCohesion);
		}


	}
 
	ImGui::Text("destination :[%f,%f ]", alphaSheep->destination.x, alphaSheep->destination.y);
 
	ImGui::TextDisabled("[ --------------------- ] ");
	if (ImGui::Button("getneighbour"))
	{
		Agents test = world.GetNeighborhood({ alphaSheep->position,100.0f });
		neighborsize = test.size();
	}
	ImGui::Text("neighbor: %d", neighborsize);

	
	if (ImGui::CollapsingHeader("Group control"))
	{
		if (ImGui::Checkbox("Group wander##bool", &groupWander))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Wander>("Wander")->SetActive(groupWander);
			}

		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Group Arrive##bool", &groupArrive))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Arrive>("Arrive")->SetActive(groupArrive);
			}

		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Group obsAvoid##bool", &groupObsAvoid))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Wander>("ObsAvoid")->SetActive(groupObsAvoid);
			}

		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Group Seek##bool", &groupSeek))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Wander>("Seek")->SetActive(groupSeek);
			}

		}

		if (ImGui::Checkbox("Group Separation##bool", &groupSeperation))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Separation>("Separation")->SetActive(groupSeperation);
			}

		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Group Alignment##bool", &groupAlignment))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Alignment>("Alignment")->SetActive(groupAlignment);
			}

		}
		ImGui::SameLine();

		if (ImGui::Checkbox("Group Cohesion##bool", &groupCohesion))
		{
			for (auto& s : sheeps)
			{
				s->mSteeringModule->GetBehavior<Cohesion>("Cohesion")->SetActive(groupCohesion);
			}

		}

	}
	

  


 	ImGui::End();

}
