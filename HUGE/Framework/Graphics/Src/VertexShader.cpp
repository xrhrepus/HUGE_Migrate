#include "Precompiled.h"
#include "VertexShader.h"

#include "D3DUtil.h"

using namespace H;
using namespace H::Graphics;

namespace
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexLayout(uint32_t vertexFormat)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		if (vertexFormat & VE_Position)
			desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Color)
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Normal)
			desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_Tangent)
			desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_TexCoord)
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_BlendIndex)
			desc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		if (vertexFormat & VE_BlendWeight)
			desc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		return desc;
	}
}
DXGI_FORMAT DetermineDXGIFormat(D3D11_SIGNATURE_PARAMETER_DESC paramDesc)
{
	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UNKNOWN)
	{
		return DXGI_FORMAT_UNKNOWN;
	}
	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
	{
		return DXGI_FORMAT_R32G32B32A32_UINT;
	}
	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
	{
		return DXGI_FORMAT_R32G32B32A32_SINT;
	}
	else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
	{
		if (strcmp(paramDesc.SemanticName, "POSITION") == 0 || strcmp(paramDesc.SemanticName, "NORMAL") == 0 || strcmp(paramDesc.SemanticName, "TANGENT") == 0)
		{
			return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (strcmp(paramDesc.SemanticName , "COLOR")==0 || strcmp( paramDesc.SemanticName , "BLENDWEIGHT")==0)
		{
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (strcmp(paramDesc.SemanticName , "TEXCOORD")==0)
		{
			return DXGI_FORMAT_R32G32_FLOAT;
		}

		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
}

void VertexShader::Initialize(uint32_t vertexformat, const wchar_t* filename)
{
	auto device = GetDevice();


	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile
	( filename,
		nullptr, nullptr,
		"VS", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error : %s", (const char*)errorBlob->GetBufferPointer());


	//create vertex shader and input layout descriptor
	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

 	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;

	if (vertexformat == 0)
	{
		ID3D11ShaderReflection* reflector = nullptr;
		hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflector);
		ASSERT(SUCCEEDED(hr), "Failed to reflect input param.");

		D3D11_SHADER_DESC desc{};
		reflector->GetDesc(&desc);
		vertexLayout.reserve(desc.InputParameters);
		for (uint32_t i = 0; i < desc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc{};
			reflector->GetInputParameterDesc(i, &paramDesc);

			auto& elementDesc = vertexLayout.emplace_back(D3D11_INPUT_ELEMENT_DESC{});
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.Format = DetermineDXGIFormat(paramDesc);
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

		}
		hr = device->CreateInputLayout(
			vertexLayout.data(), vertexLayout.size(),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			&mInputLayout);

		SafeRelease(reflector);

	}
	else
	{
		vertexLayout = GetVertexLayout(vertexformat);
		hr = device->CreateInputLayout(
			vertexLayout.data(), vertexLayout.size(),
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			&mInputLayout);
	}

	ASSERT(SUCCEEDED(hr), "Failed to create vertex input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

}
void H::Graphics::VertexShader::Initialize(const wchar_t * filename)
{
	auto device = GetDevice();


	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile
	(filename,
		nullptr, nullptr,
		"VS", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader. Error : %s", (const char*)errorBlob->GetBufferPointer());


	//create vertex shader and input layout descriptor
	hr = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;

 	 
		ID3D11ShaderReflection* reflector = nullptr;
		hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflector);
		ASSERT(SUCCEEDED(hr), "Failed to reflect input param.");

		D3D11_SHADER_DESC desc{};
		reflector->GetDesc(&desc);
		vertexLayout.reserve(desc.InputParameters);
		for (uint32_t i = 0; i < desc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc{};
			reflector->GetInputParameterDesc(i, &paramDesc);

			auto& elementDesc = vertexLayout.emplace_back(D3D11_INPUT_ELEMENT_DESC{});
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.Format = DetermineDXGIFormat(paramDesc);
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

		}

	 
 
	hr = device->CreateInputLayout(
		vertexLayout.data(), vertexLayout.size(),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		&mInputLayout);

	ASSERT(SUCCEEDED(hr), "Failed to create vertex input layout.");

	SafeRelease(reflector);
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}
void VertexShader::Terminate()
{
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);

}
void VertexShader::Bind() const
{
	auto context = GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);

}
void  VertexShader::UnBind() const
{
	GetContext()->VSSetShader(nullptr, nullptr, 0);
}
