#include "Precompiled.h"
#include "World.h"
#include "GameObject.h"

using namespace H;

void H::World::LoadLevel(const std::filesystem::path templatefile)
{
	using namespace rapidjson;

	FILE* file = nullptr;
	fopen_s(&file, templatefile.u8string().c_str(), "r");

	char readBuffer[65536];

	FileReadStream is(file, readBuffer, sizeof(readBuffer));

	Document doc;
	doc.ParseStream(is);

	if (doc.HasMember("Services") && doc["Services"].IsObject())
	{
		auto scvObj = doc["Services"].GetObjectW();
		for (auto& svo : scvObj)
		{
			auto metaclass = Core::Meta::FindMetaClass(svo.name.GetString());
			auto newscv = AddService(metaclass);
			ASSERT(newscv, "[World] failed to create service %s . ",svo.name.GetString());
			metaclass->Deserialize(newscv, svo.value);
		}
	}

	//H::GameObject* gameObject = mGameObjectAllocator->New();

	//if (doc.HasMember("GameObjects") && doc["GameObjects"].IsArray())
	//{
	//	auto jsonObj = doc["GameObject"].GetObjectW();
	//	if (jsonObj.HasMember("Components") && jsonObj["Components"].IsObject())
	//	{
	//		auto comps = jsonObj["Components"].GetObjectW();
	//		for (auto& c : comps)
	//		{
	//			auto meta = Core::Meta::FindMetaClass(c.name.GetString());
	//			auto newcomp = gameObject->AddComponent(meta);
	//			ASSERT(newcomp, "[GameObjectFactory] failed to create GO");
	//			meta->Deserialize(newcomp, c.value);
	//		}
	//	}
	//	//register with handlepool
	//	auto handle = mGameObjectHandlePool->Register(gameObject);

	//	//init the game object 

	//	gameObject->mWorld = this;
	//	gameObject->mHandle = handle;
	//	gameObject->mName = std::move(name);
	//	gameObject->Initialize();

	//	//add game object to update list
	//	mUpdateList.push_back(gameObject);

	//}
}

void H::World::Initialize(size_t capacity)
{
	ASSERT(!mInitialized, "[World] World already Initialized.");

	for (auto& s : mServices)
	{
		s->Initialize();
	}

	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);
	mGameObjectFactory = std::make_unique<GameObjectFactory>(*mGameObjectAllocator);
	mInitialized = true;
}

void H::World::Terminate()
{
	if (!mInitialized)
	{
		return;
	}

	ASSERT(!mUpdating, "[World] can not terminate while updating world");

	mUpdating = true;
	std::for_each(mUpdateList.begin(), mUpdateList.end(), [this](auto go) {Destroy(go->GetHandle()); });
	mUpdating = false;
	mUpdateList.clear();

	//
	ProcessDestroyList();

	mGameObjectAllocator.reset();
	mGameObjectHandlePool.reset();
	mGameObjectFactory.reset();

	for (auto& s : mServices)
	{
		s->Terminate();
	}


	mInitialized = false;
}

Service * H::World::AddService(const Core::Meta::MetaClass * metaClass)
{
	ASSERT(!mInitialized, "[World] World not Initialized before add service");
	Service* newScv = static_cast<Service*>(metaClass->Create());
	auto& newService = mServices.emplace_back(std::unique_ptr<Service>(newScv));
	newService->mWorld = this;
	return newService.get();
}

GameObjectHandle H::World::Create(const std::filesystem::path & templateFileName, std::string name)
{
	auto gameObject = mGameObjectFactory->Create(templateFileName.u8string().c_str());
	ASSERT(gameObject != nullptr, "[World] Failed to create gameobject from template file %s.", templateFileName.u8string().c_str());

	//register with handlepool
	auto handle = mGameObjectHandlePool->Register(gameObject);

	//init the game object 

	gameObject->mWorld = this;
	gameObject->mHandle = handle;
	gameObject->mName = std::move(name);
	gameObject->Initialize();

	//add game object to update list
	mUpdateList.push_back(gameObject);

	return handle;
}

GameObjectHandle H::World::Find(const std::string & name)
{
	using namespace std;

	auto iter = find_if(begin(mUpdateList), end(mUpdateList), [&name](auto gameObject) {return gameObject->GetName() == name; });
	return (iter != end(mUpdateList)) ? (*iter)->GetHandle() : GameObjectHandle();
	 
}

  
void H::World::Destroy(GameObjectHandle handle)
{
	if (!handle.IsValid())
	{
		return;
	}

	//cache the pointer and unregister with the handle pool
	GameObject* gameObject = handle.Get();
	mGameObjectHandlePool->UnRegister(handle);

	//check if can destroy
	if (!mUpdating)
	{
		DestroyInternal(gameObject);
	}
	else
	{
		mDestoryList.push_back(gameObject);
	}
}

void H::World::Update(float deltatime)
{
	ASSERT(!mUpdating, "[World] Already updating world");

	for (auto& s : mServices)
	{
		s->Update(deltatime);
	}


	//lock updatelist
	mUpdating = true;

	//recompute size so new obj can be added to list
	for (size_t i = 0; i < mUpdateList.size(); i++)
	{
		GameObject* go = mUpdateList[i];
		if (go->GetHandle().IsValid())
		{
			go->Update(deltatime);
		}
	}
	//
	mUpdating = false;

	//
	ProcessDestroyList();
 }

void H::World::Render() 
{
	for (auto& s : mServices)
	{
		s->Render();
	}


	for (auto go : mUpdateList)
	{
		go->Render();
	}
}

void H::World::DebugUI()
{
	for (auto& s : mServices)
	{
		s->DebugUI();
	}


	for (auto go : mUpdateList)
	{
		go->DebugUI();
	}
}

void H::World::DestroyInternal(GameObject * gameObject)
{
	ASSERT(!mUpdating, "[World] connot destroy game object during update");
	if (gameObject == nullptr)
	{
		return;
	}

	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObject);
	if (iter != mUpdateList.end())
	{
		mUpdateList.erase(iter);
	}

	gameObject->Terminate();

	mGameObjectFactory->Destroy(gameObject);

}

void H::World::ProcessDestroyList()
{
	for (auto go : mDestoryList)
	{
		DestroyInternal(go);
	}
	mDestoryList.clear();
}
