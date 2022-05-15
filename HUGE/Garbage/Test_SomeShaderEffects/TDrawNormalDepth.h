#pragma once
#include <HUGE/Inc/HUGE.h>

class TDrawNormalDepth
{

public:
	void Initialize();
	void Terminate();

	//create DN
	void SetDNBuffer(const TransformData& tfdata);
	void BeginDN();
	void EndDN();

	void DebugUI();
	ID3D11ShaderResourceView* GetDN_SEresView();
	RenderTarget mCreateDN_RT;

private:
	const std::filesystem::path EDGEDRAW_CREATEDN_FILE_PATH = L"../../Assets/Shaders/DrawEdge_createDepthnormal.fx";
	//create DN
	TransformBuffer mTFBuf;

	VertexShader mCreateDN_VS;
	PixelShader  mCreateDN_PS;

};
 