#pragma once
#include "ShaderEffect.h"
#include "ShaderEffectBufferType.h"

namespace H::Graphics
{
	class RenderMaterial
	{
	public:
		virtual void Init();
		void Bind() const;
		void UnBind() const;
		// set commonly used global resource
		virtual void SetTransform(const TransformData& data);
		virtual void SetCamera(const Camera& cam);
		virtual void SetLight(const DirectionalLight& light);
		virtual void DebugUI();
		void SetShaderContextDebugUIFunc(std::function<void()> func);
	private:
		// using this name to get effect from ShaderEffectManager
		std::string mShaderEffectName = "Standard";
		// ref to instance in ShaderEffectManager
		const ShaderEffect* mShaderEffect;
		// ShaderEffect creates context. data stored in Material
		std::unique_ptr<ShaderEffectContext> mShaderEffectContext;
		// debugui callback created by context
		std::function<void()> mDebugUICallBack;
	};

}
