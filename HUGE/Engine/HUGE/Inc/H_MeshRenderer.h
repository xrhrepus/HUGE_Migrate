#ifndef HUGE_HMESHRENDERERCOMPONENT_H
#define HUGE_HMESHRENDERERCOMPONENT_H
#include "Component.h"
#include "MeshService.h"

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
		void SetContext(const ShaderEffect_Standard::SE_Context_Standard& context);


	private:
		const ShaderEffect_Standard* mStandardShaderEffect;
		ShaderEffect_Standard::SE_Context_Standard mStandardContext;
		const H::MeshService::MeshEntry* mMesh;
		MeshBuffer mMeshBuffer;

	};
}

#endif // !HUGE_HMESHRENDERERCOMPONENT_H


 