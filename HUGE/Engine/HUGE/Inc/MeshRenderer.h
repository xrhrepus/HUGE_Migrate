#ifndef HUGE_HMESHRENDERERCOMPONENT_H
#define HUGE_HMESHRENDERERCOMPONENT_H
#include "Component.h"
#include "MeshService.h"
#include "RenderMaterialService.h"
#include <Graphics/Inc/StandardEffect.h>
#include <Graphics/Inc/RenderMaterial.h>
#include "TransformComponent.h"
#include "LightService.h"

namespace H
{
 
	class MeshRenderer : public Component
	{
	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void Terminate() override;
		void Update(float deltatime) override;
		void Render() override;
		void DebugUI() override;

		void SetMesh(const H::MeshService::MeshEntry& mesh);
		void SetRenderMaterial(H::RenderMaterialService::RenderMaterialEntry& material);
		void SetContext(const Graphics::ShaderEffect_Standard::SE_Context_Standard& context);


	private:
		const Camera* mCamera;
		H::LightService* mLightServicePtr;
		const TransformComponent* mTransformComponent;
		const ShaderEffect_Standard* mStandardShaderEffect;
		ShaderEffect_Standard::SE_Context_Standard mStandardContext;
		const H::MeshService::MeshEntry* mMesh;
		MeshBuffer mMeshBuffer;
		H::RenderMaterialService::RenderMaterialEntry* mRenderMaterial;
	};
}

#endif // !HUGE_HMESHRENDERERCOMPONENT_H


 