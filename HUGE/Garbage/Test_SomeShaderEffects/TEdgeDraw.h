#pragma once
#include <HUGE/Inc/HUGE.h>


class TEdgeDraw
{
public:
	struct EdgeDrawData
	{
		Vector4 edgeColor = Colors::Cyan;
		float threshold = 0.06f;
		Vector2 uv_step = { 1.0f / 1024.0f };
		float edgeColorWeight = 1.0f;
	};

	using EdgeDrawParamBuffer = TypedConstantBuffer<EdgeDrawData>;

	void Initialize();
	void Terminate();

	//create DN
	void SetDNBuffer(const TransformData& tfdata);
	void BeginDN();
	void EndDN();

	//origin scene
	void Begin();
	void End();
	
	//postprocess, draw edge
	void Render();

	void DebugUI() ;
	ID3D11ShaderResourceView* GetDN_SEresView();
	ID3D11ShaderResourceView* GetOR_SEresView();
	ID3D11ShaderResourceView* GetFinal_SEresView();


private:
  
	const std::filesystem::path EDGEDRAW_CREATEDN_FILE_PATH = L"../../Assets/Shaders/DrawEdge_createDepthnormal.fx";
	const std::filesystem::path EDGEDRAW_POSTPROCESS_FILE_PATH = L"../../Assets/Shaders/DrawEdge_postprocess.fx";
 	
	//param
	TransformBuffer mTFBuf;
	TransformData mTFData;

	bool EdgeDrawOn = false;
	EdgeDrawParamBuffer mEdgeDrawBuffer;
	EdgeDrawData mEdgeDrawData;

	//create DN
	RenderTarget mCreateDN_RT;
	VertexShader mCreateDN_VS;
	PixelShader  mCreateDN_PS;

	// origin scene
 	RenderTarget mOriginRT;
 
	//final draw
	VertexShader mVS;
	PixelShader  mPS;
	RenderTarget mRT;

	MeshPX mScreenMesh;
	MeshBuffer mScreenMeshBuffer;

};

 
