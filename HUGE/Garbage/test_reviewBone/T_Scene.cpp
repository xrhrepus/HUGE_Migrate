#include "T_Scene.h"

void T_Scene::Init()
{
	LoadDefaultMesh();
	LoadDefaultTexture();
}

void T_Scene::LoadMesh(const std::string name, const std::filesystem::path & file)
{
	using MeshStoragePairKey = SceneMeshMap::StoragePairKey;
	using MeshStoragePairValue = SceneMeshMap::StoragePairVal;

	auto m = std::make_unique<Mesh>();
	ObjLoader::Load(file, 100.0f, *m);
	mMeshes.AddItem(std::make_pair(name, std::move(m)));
}

void T_Scene::LoadDefaultMesh()
{
	mMeshes.AddItem(std::make_pair("cube", std::make_unique<Mesh>(MeshBuilder::CreateCube(10.0f, 10.0f, 10.0f, Vector3::Zero()))));
	mMeshes.AddItem(std::make_pair("sphere", std::make_unique<Mesh>(MeshBuilder::CreateShpere(10.0f, 10, Vector3::Zero()))));

	//
	LoadMesh("cat", "../../Assets/Models/TestModel/cat.obj");
	LoadMesh("tank", "../../Assets/Models/Tank/tank.obj");
}

void T_Scene::LoadTexture(const std::string name, const std::filesystem::path& file)
{
	mTextures.AddItem(std::make_pair(name, std::make_unique<TextureId>(TextureManager::Get()->LoadTexture(file))));
}

void T_Scene::LoadDefaultTexture()
{
	LoadTexture("cat", "../../Assets/Models/TestModel/cat_tex.png");

	LoadTexture("tankdiffuse", "../../Assets/Models/Tank/tank_diffuse.jpg");
	LoadTexture("tankspecular", "../../Assets/Models/Tank/tank_specular.jpg");
	LoadTexture("tanknormal", "../../Assets/Models/Tank/tank_normal.jpg");
	LoadTexture("tankgloss", "../../Assets/Models/Tank/tank_gloss.jpg");
	LoadTexture("tankao", "../../Assets/Models/Tank/tank_ao.jpg");
}

void T_Scene::DebugUI()
{
	using MeshStoragePairKey = SceneMeshMap::StoragePairKey;
	using MeshStoragePairValue = SceneMeshMap::StoragePairVal;

	ImGui::Begin("TScene");
	if (ImGui::CollapsingHeader("Meshes"))
	{
		mMeshes.ForEachItem([](const MeshStoragePairKey& key, const Mesh& value) {
			ImGui::Text(key.c_str());
			ImGui::NewLine();
		});
	}
	ImGui::End();
}

const Mesh* T_Scene::GetMesh(const SceneMeshMap::StoragePairKey& name) const
{
	return mMeshes.GetItem(name);;
}

const TextureId T_Scene::GetTexture(const SceneMeshMap::StoragePairKey & name) const
{
	const auto* t = mTextures.GetItem(name);

	return t ? *t : 0;
}

void T_Scene::ForEachTexture(std::function<void(const std::string&, const TextureId&)> func) const
{
	mTextures.ForEachItem(func);
}
