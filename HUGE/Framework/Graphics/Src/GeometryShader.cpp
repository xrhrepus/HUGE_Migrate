//2019.10.14
#include "Precompiled.h"
#include "GeometryShader.h"
#include "D3DUtil.h"

using namespace H::Graphics;


void GeometryShader::Initialize(const wchar_t* filename)
{

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile
	(filename,
		nullptr, nullptr,
		"GS", "gs_5_0", 0, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile geometry shader. Error : %s", (const char*)errorBlob->GetBufferPointer());

	//create geometry shader 
	hr = GetDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mGeometryShader);
	ASSERT(SUCCEEDED(hr), "Failed to create geometry shader.");


 	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

}
void GeometryShader::Terminate()
{
	SafeRelease(mGeometryShader);
}
void GeometryShader::Bind()
{
	GetContext()->GSSetShader(mGeometryShader,nullptr,0);
}

void GeometryShader::UnBind()
{
	GetContext()->GSSetShader(nullptr, nullptr, 0);//?
}
