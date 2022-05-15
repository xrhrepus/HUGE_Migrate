#pragma once
#include <HUGE/Inc/HUGE.h>
#include "TDrawNormalDepth.h"
namespace H
{
 
	class TMeshRenderer
	{
		struct EdgeDrawData
		{
			Vector4 edgeColor = Colors::Cyan;
			float threshold = 0.06f;
			Vector2 uv_step = { 1.0f / 1280.0f,1.0f / 720.0f };
			float edgeColorWeight = 1.0f;
		};

	public:
 		void Initialize() ;
		void Terminate() ;
		void Update(float deltatime) ;
		void Render() ;
		void DebugUI() ;

 		void SetContext(const ShaderEffect_Standard::SE_Context_Standard& context);
		void SetContext(const ShaderEffect_Toon::SE_Context_Toon& context);

		bool useStandard = false;
		ShaderEffect_Standard::SE_Context_Standard mStandardContext;
		ShaderEffect_Toon::SE_Context_Toon mToonContext;

 	private:
		//ShaderEffect_Standard* mStandardShaderEffect;
		//ShaderEffect_Toon* mToonShaderEffect;
		ShaderEffectContext* mShaderEffectContext;
		ShaderEffect* mShaderEffect;
		Mesh mMesh;

		TDrawNormalDepth mNDDraw;
		EdgeDrawData mED;
		TypedConstantBuffer<EdgeDrawData> mEDBuf;
	};
}

 

 