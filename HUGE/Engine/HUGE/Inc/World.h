#ifndef HUGE_WORLD_H
#define HUGE_WORLD_H
#include "GameObjectFactory.h"
#include "Service.h"


namespace H
{
	class World
	{
	public:
		void LoadLevel(const std::filesystem::path templatefile);

		void Initialize(size_t capacity);
		void Terminate();

		Service* AddService(const Core::Meta::MetaClass* metaClass);


		template<class T, class = std::void_t<std::is_base_of<Service,T>>>
		T* AddService()
		{
			ASSERT(!mInitialized, "[World] World not Initialized before add service");
			auto& newService = mServices.emplace_back(std::make_unique<T>());
			newService->mWorld = this;
			return static_cast<T*>(newService.get());
		}

		template<class T>
		T* GetService()
		{
			return const_cast<T*> (static_cast<const World*>(this)->GetService<T>());
		}
		template<class T>
		const T* GetService() const
		{
			for (auto& s : mServices)
			{
				if (s->GetMetaClass() == T::StaticGetMetaClass())
				{
					return static_cast<T*>(s.get());
				}
			}
			return nullptr;
		}

		GameObjectHandle Create(const std::filesystem::path& templateFileName, std::string name);
		GameObjectHandle Find(const std::string& name);
		void Destroy(GameObjectHandle handle);

		void Update(float deltatime);
		void Render() ;
		void DebugUI() ;

	private:
		friend class Editor;

		void DestroyInternal(GameObject* gameObject);
		void ProcessDestroyList();

		bool mInitialized = false;
		bool mUpdating = false;
 
		using GameObjectList = std::vector<GameObject*>;
		using Services = std::vector<std::unique_ptr<Service>>;


		Services mServices;

		std::unique_ptr<GameObjectAllocator> mGameObjectAllocator;
		std::unique_ptr<GameObjectHandlePool> mGameObjectHandlePool;
		std::unique_ptr<GameObjectFactory> mGameObjectFactory;

		GameObjectList mUpdateList;
		GameObjectList mDestoryList;
		
	};

 
}

#endif // !HUGE_WORLD_H

