#pragma once
#include <HUGE/Inc/HUGE.h>
namespace H
{
 
	class TMeshRenderer
	{
	public:
 		void Initialize() ;
		void Terminate() ;
		void Update(float deltatime) ;
		void Render() ;
		void DebugUI() ;

		void SetMesh(const Mesh& mesh);
		void SetContext(const ShaderEffect_Standard::SE_Context_Standard& context);


		ShaderEffect_Standard::SE_Context_Standard mStandardContext;
		MeshBuffer mMeshBuffer;
	private:
		ShaderEffect_Standard* mStandardShaderEffect;
		Mesh mMesh;

	};
}

 

 