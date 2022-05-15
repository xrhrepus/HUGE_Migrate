#include "Precompiled.h"
#include "Component.h"
#include "GameObject.h"

using namespace H;

META_CLASS_BEGIN(GameObject)
META_NO_FIELD
META_CLASS_END


void H::GameObject::Initialize()
{
	for (auto& comp : mComponents)
	{
		comp->Initialize();
	}
}

void H::GameObject::Terminate()
{
	for (auto& comp : mComponents)
	{
		comp->Terminate();
	}

}

void H::GameObject::Update(float deltatime)
{
	for (auto& comp : mComponents)
	{
		comp->Update(deltatime);
	}

}

void H::GameObject::Render()
{
	for (auto& comp : mComponents)
	{
		comp->Render();
	}

}

void H::GameObject::DebugUI()
{
 	for (auto& comp : mComponents)
	{
		comp->DebugUI();
	}
 }

Component * H::GameObject::AddComponent(const H::Core::Meta::MetaClass* metaclass)
{
	Component* newComp = static_cast<Component*>(metaclass->Create());
	newComp->mOwner = this;
	mComponents.emplace_back (std::unique_ptr<Component>(newComp));
	return newComp;
}
