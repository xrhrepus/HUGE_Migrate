#pragma once
#include <HUGE/Inc/HUGE.h>

// Shader Effect provides buffer binding
// Renderer uses MeshBuffer to do final draw call

// Renderer = mesh + Material
// Material = shader + shader customized data

class T_Light;
class T_Transform;
class T_Scene;


// provides a abstract template for Renderer
class T_Material
{
public:
	virtual void Init() = 0;
	void Bind() const;
	void UnBind() const;
	// set commonly used global resource
	virtual void SetTransform(const TransformData& data);
	virtual void SetCamera(const Camera& cam);
	virtual void SetLight(const DirectionalLight& light);
	virtual void DebugUI() = 0;
	virtual void SetScene(const T_Scene& scene);
protected:
	// ref to instance in ShaderEffectManager
	const ShaderEffect* mShaderEffect;
	// ShaderEffect creates context. data stored in Material
	std::unique_ptr<ShaderEffectContext> mShaderEffectContext;
protected:
	const T_Scene* mScene;
};

// 1 shader 1 material.
class Standard_Material : public T_Material
{
public:
	void Init() override;
	void SetTransform(const TransformData& data);
	void SetCamera(const Camera& cam);
	void SetLight(const DirectionalLight& light);

	void SetDiffuseTexture(TextureId tid);
	void DebugUI() override;
};

class T_MeshRenderer
{
public:
	void SetScene(const T_Scene& scene);
public:
	void SetCamera(const Camera& cam);
	void SetMesh(const Mesh& mesh);
	void SetMaterial(T_Material& mat);
	void SetLight(T_Light& light);
	void SetTransform(T_Transform& transform);

	void PreRender();
	void Render() const;
	void PostRender();

	void DebugUI();

private:
	const T_Scene* mScene;
private:
	MeshBuffer mMeshBuffer;
	const Mesh* mMesh;
	const Camera* mCurrentCam;
	T_Transform* mTransform;
	T_Material* mMat; // ?
	T_Light* mLight;
};