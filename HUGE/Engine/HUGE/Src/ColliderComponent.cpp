#include "Precompiled.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "GameObject.h"
using namespace H;

META_DERIVED_BEGIN(ColliderComponent,Component)
META_FIELD_BEGIN
META_FIELD(center,"Center")
META_FIELD(extend, "Extend")
META_FIELD_END
META_CLASS_END

void H::ColliderComponent::Initialize()
{
}
void H::ColliderComponent::Update(float dt)
{
	center = GetOwner().GetComponent<TransformComponent>()->position;
}

void H::ColliderComponent::Render()
{
	H::Graphics::SimpleDraw::AddBox(AABB{ center,extend }, H::Graphics::Colors::Red);
}
void H::ColliderComponent::DebugUI()
{
	if (ImGui::TreeNode("ColliderComponent"))
	{
		ImGui::TreePop();

	}
}
