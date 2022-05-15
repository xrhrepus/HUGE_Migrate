//===============================
//HUGE - H's Ultimate Game Engine
//
//===============================

#ifndef HUGE_GAMEOBJECT_H
#define HUGE_GAMEOBJECT_H

namespace H
{
	class Component;
	class World;
	class GameObject;

	using GameObjectAllocator = H::Core::TypedAllocator<H::GameObject>;
	using GameObjectHandle = H::Core::Handle<H::GameObject>;
	using GameObjectHandlePool = H::Core::HandlePool<H::GameObject>;


	class GameObject final
	{
	public:
		META_CLASS_DECLARE

		GameObject() = default;
 
		void Initialize();
		void Terminate();

		void Update(float deltatime);
		void Render();
		void DebugUI();



		//Component

		Component* AddComponent(const H::Core::Meta::MetaClass* metaclass);

		template<class T, class = std::void_t<std::is_base_of<Component,T>>>
		T* AddComponent()
		{
			std::unique_ptr<Component>& newComponent = mComponents.emplace_back(std::make_unique<T>());
			newComponent->mOwner = this;
			return static_cast<T*>(newComponent.get());
		}
		template<class ComponentType>
		const ComponentType* GetComponent() const
		{
			for (auto& component : mComponents)
			{
				if (component->GetMetaClass() == ComponentType::StaticGetMetaClass())
				{
					return static_cast<ComponentType*>(component.get());
				}
			}
			return nullptr;
		}

		template<class ComponentType>
		ComponentType* GetComponent()
		{
			const auto* constMe = this;
			return const_cast<ComponentType*>(constMe->GetComponent<ComponentType>());
		}

		//World
		const World& GetWorld() const { return *mWorld; }
		World& GetWorld() { return *mWorld; }

		//Name
		void SetName(const char* name) { mName = name; }
		const char* GetName() const { return mName.c_str(); }

		GameObjectHandle GetHandle() const { return mHandle; }

	private:
		friend class Editor;
		friend class World;

		using Components = std::vector<std::unique_ptr< Component>>;

		World* mWorld = nullptr;
		std::string mName = "NoName";
		GameObjectHandle mHandle;
		Components mComponents;
	};

}//namespace H

#endif // !HUGE_HUGE_GAMEOBJECT_H

