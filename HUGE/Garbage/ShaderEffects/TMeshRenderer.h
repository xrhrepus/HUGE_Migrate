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

 		void SetContext(const ShaderEffect_Standard::SE_Context_Standard& context);
		void SetContext(const ShaderEffect_Toon::SE_Context_Toon& context);

		bool useStandard = true;
		ShaderEffect_Standard::SE_Context_Standard mStandardContext;
		ShaderEffect_Toon::SE_Context_Toon mToonContext;

 	private:
 		ShaderEffectContext* mShaderEffectContext;
		ShaderEffect* mShaderEffect;
		Mesh mMesh;
 	};
}

 

 