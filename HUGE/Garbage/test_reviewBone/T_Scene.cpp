#include "T_Scene.h"

void T_Scene::Init()
{
	LoadPrimitiveMesh();
	mTextures.AddItem(std::make_pair("cat", std::make_unique<TextureId>(TextureManager::Get()->LoadTexture("../../Assets/Models/TestModel/cat_tex.png"))));
}

void T_Scene::LoadMesh(const std::string name, const std::filesystem::path & file)
{
	using MeshStoragePairKey = SceneMeshMap::StoragePairKey;
	using MeshStoragePairValue = SceneMeshMap::StoragePairVal;

	auto m = std::make_unique<Mesh>();
	ObjLoader::Load(file, 100.0f, *m);
	mMeshes.AddItem(std::make_pair(name, std::move(m)));
}

void T_Scene::LoadPrimitiveMesh()
{
	mMeshes.AddItem(std::make_pair("cube", std::make_unique<Mesh>(MeshBuilder::CreateCube(10.0f, 10.0f, 10.0f, Vector3::Zero()))));
	mMeshes.AddItem(std::make_pair("sphere", std::make_unique<Mesh>(MeshBuilder::CreateShpere(10.0f, 10, Vector3::Zero()))));

	//
	LoadMesh("cat", "../../Assets/Models/TestModel/cat.obj");
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
