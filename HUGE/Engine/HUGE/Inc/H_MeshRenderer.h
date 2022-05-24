#ifndef HUGE_COLLIDERCOMPONENT_H
#define HUGE_COLLIDERCOMPONENT_H
#include "Component.h"

namespace H
{
 
	class H_MeshRenderer : public Component
	{
	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void Terminate() override;
		void Update(float deltatime) override;
		void Render() override;
		void DebugUI() override;

		void SetMesh(const Mesh& mesh);
		void SetContext(const ShaderEffect_Standard::SE_Context_Standard& context);


	private:
		const ShaderEffect_Standard* mStandardShaderEffect;
		ShaderEffect_Standard::SE_Context_Standard mStandardContext;
		Mesh mMesh;
		MeshBuffer mMeshBuffer;

	};
}

#endif // !HUGE_COLLIDERCOMPONENT_H


 