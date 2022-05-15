#include "Precompiled.h"
#include "Component.h"
#include "GameObjectFactory.h"

using namespace H;
 
H::GameObjectFactory::GameObjectFactory(GameObjectAllocator & allocator)
	:mGameObjectAllocator(allocator)
{
}

H::GameObject* H::GameObjectFactory::Create(const char * templateFileName)
{
	using namespace rapidjson;

	H::GameObject* gameObject = mGameObjectAllocator.New();
	if (gameObject != nullptr)
	{
		FILE* file = nullptr;
		fopen_s(&file, templateFileName, "r");

		char readBuf[66536];
		FileReadStream is(file, readBuf, sizeof(readBuf));

		Document document;
		document.ParseStream(is);

		if (document.HasMember("GameObject") && document["GameObject"].IsObject())
		{
			auto jsonObj = document["GameObject"].GetObjectW();
			if (jsonObj.HasMember("Components") && jsonObj["Components"].IsObject())
			{
				auto comps = jsonObj["Components"].GetObjectW();
				for (auto& c : comps)
				{
					auto meta = Core::Meta::FindMetaClass(c.name.GetString());
					auto newcomp = gameObject->AddComponent(meta);
					ASSERT(newcomp, "[GameObjectFactory] failed to create GO");
					meta->Deserialize(newcomp, c.value);
				}
			}
		}

		fclose(file);
	}
	return gameObject;
}

void H::GameObjectFactory::Destroy(H::GameObject * gameobject)
{
	mGameObjectAllocator.Delete(gameobject);
}
