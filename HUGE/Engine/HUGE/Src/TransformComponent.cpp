#include "Precompiled.h"
#include "TransformComponent.h"


using namespace H;

META_DERIVED_BEGIN(TransformComponent,Component)
META_FIELD_BEGIN
META_FIELD(position,"Position")
META_FIELD(rotation, "Rotation")
META_FIELD_END
META_CLASS_END


void H::TransformComponent::Initialize()
{
}

void H::TransformComponent::Render()
{
	H::Graphics::SimpleDraw::AddLine(position, { position.x+10.0f,position.y,position.z }, H::Graphics::Colors::Red);
	H::Graphics::SimpleDraw::AddLine(position, { position.x ,position.y + 10.0f,position.z }, H::Graphics::Colors::Green);
	H::Graphics::SimpleDraw::AddLine(position, { position.x ,position.y,position.z + 10.0f }, H::Graphics::Colors::Blue);

}
void H::TransformComponent::DebugUI()
{
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::DragFloat3("Position", position.v.data(), 0.01f);
		ImGui::DragFloat4("Rotation", &rotation.x, 0.01f);
		ImGui::TreePop();
	}
 
}
