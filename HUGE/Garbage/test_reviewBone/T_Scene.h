#pragma once
#include <HUGE/Inc/HUGE.h>

template<class T>
class T_Scene_ResourceStorage
{
public:
	using StoragePairKey = std::string;
	using StoragePairVal = std::unique_ptr<T>;
	using StoragePair = std::pair<StoragePairKey, StoragePairVal>;
	using Storage = std::unordered_map<StoragePairKey, StoragePairVal>;
	bool AddItem(StoragePair&& item)
	{
		ASSERT(item.second != nullptr, "[T_Scene_ResourceStorage] do not add a null item");
		if (item.second == nullptr)
		{
			return false;
		}
		auto[it, succeed] = mStorage.emplace(std::move(item));
		return succeed;
	}
	const T* GetItem(const StoragePairKey& name) const
	{
		auto it = findItem(name);
		if (it != mStorage.cend())
		{
			return it->second.get(); // safe
		}
		return nullptr;
	}
	bool RemoveItem(const StoragePairKey& name)
	{
		auto it = findItem(name); // ?
		if (it != mStorage.cend())
		{
			mStorage.erase(it);
			return true;
		}
		return false;
	}
	void ForEachItem(std::function<void(const StoragePairKey&, const T&)> func)
	{
		for (auto& [key, value] : mStorage)
		{
			func(key, *value); //safe
		}
	}
	
private:
	decltype(auto) findItem(const StoragePairKey& name) const
	{
		return mStorage.find(name);
	}

	Storage mStorage;
};

class T_Scene
{
	using SceneCameraMap = T_Scene_ResourceStorage<Camera>;
	using SceneMeshMap = T_Scene_ResourceStorage<Mesh>;
	using SceneTextureMap = T_Scene_ResourceStorage<TextureId>;

public:
	void Init();
	void DebugUI();

	const Mesh* GetMesh(const SceneMeshMap::StoragePairKey& name) const;
	const TextureId GetTexture(const SceneMeshMap::StoragePairKey& name) const;
private:
	void LoadMesh(const std::string name, const std::filesystem::path& file);
	void LoadPrimitiveMesh();
	// resources
	SceneCameraMap mCameras;
	SceneMeshMap mMeshes;
	SceneTextureMap mTextures;
};
