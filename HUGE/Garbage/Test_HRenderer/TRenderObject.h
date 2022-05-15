#pragma once
#include "C_Transform.h"
#include "TMeshRenderer.h"

class TRenderObject
{
public:
  
	void Initialize(const Mesh& mesh, ShaderEffect_Standard::SE_Context_Standard& ctx,std::string name);
	void Initialize(std::string name);

	void Terminate();
	void Update(float deltatime);
	void Render();
	void DebugUI();

	void SetMeshContext(const Mesh& mesh,ShaderEffect_Standard::SE_Context_Standard& ctx);

	std::string mName;
	C_Transform mTransform;
	TMeshRenderer mMeshRenderer;
private:
 
};
 