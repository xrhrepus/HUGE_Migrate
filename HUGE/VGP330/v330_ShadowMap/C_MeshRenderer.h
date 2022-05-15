#pragma once
#include <HUGE/Inc/HUGE.h>
using namespace H;

class C_MeshRenderer : public H::Component
{
public:
	META_CLASS_DECLARE

 	void Initialize() override;
	void Terminate() override;
	void Update(float deltatime) override;
	void Render() override;
	void DebugUI() override;

public:
	
public:
	TransformData transformData;
	OptionsData optionsData;
	Mesh mesh;

	Material material;

	TextureId diffuse;
	TextureId specular;
	TextureId displacement;
	TextureId normal;

private:
	ShaderEffect_Standard* mShaderEffect;
	MeshBuffer mMeshBuffer;

};

 