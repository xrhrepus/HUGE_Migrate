#include "Precompiled.h"
#include "PixelShader.h"
#include "D3DUtil.h"

//#include <Graphics/Inc/Graphics.h>
//#include <Graphics\Inc\PixelShader.h>

using namespace H::Graphics;

void PixelShader::Initialize()
{
 

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile
	(L"../Assets/Shaders/Texturing.fx",
		nullptr, nullptr,
		"PS", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error : %s", (const char*)errorBlob->GetBufferPointer());

	//create pixel shader and input layout descriptor
	hr = GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");
 
 	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);


}
void PixelShader::Initialize(const wchar_t* filename, const char* entryFunction = "PS")
{
	 


	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile
	(filename,
		nullptr, nullptr,
		entryFunction, "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader. Error : %s", (const char*)errorBlob->GetBufferPointer());

	//create pixel shader and input layout descriptor
	hr = GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");


 	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);


}

void PixelShader::Terminate()
{
	SafeRelease(mPixelShader);

}
void PixelShader::Bind() const
{
 	GetContext()->PSSetShader(mPixelShader, nullptr, 0);

}
void PixelShader::UnBind() const 
{
	GetContext()->PSSetShader(nullptr, nullptr, 0);
}
