#pragma once
#include <HUGE/Inc/HUGE.h>

// Shader Effect provides buffer binding
// Renderer uses MeshBuffer to do final draw call

// Renderer = mesh + Material
// Material = shader + shader customized data

class T_Material
{
public:
	T_Material();
	void BindShader() const;
	void UnBindShader() const;

	void DebugUI();
private:
	// ref to instance in ShaderEffectManager
	const ShaderEffect* mShaderEffect;
	// ShaderEffect creates context. data stored in Material
	std::unique_ptr<ShaderEffectContext> mShaderEffectContext;
};

class T_MeshRenderer
{
public:
	T_MeshRenderer(const Mesh& mesh);
	void PreRender() const;
	void Render() const;
	void PostRender() const;

	void DebugUI();

private:
	const Mesh& mMesh;
	MeshBuffer mMeshBuffer;
	T_Material mMat;
};