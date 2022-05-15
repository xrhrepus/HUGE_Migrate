#include "Precompiled.h"
#include "DepthStencilState.h"
#include "D3DUtil.h"
#include "GraphicsSystem.h"
using namespace H::Graphics;
namespace
{
	D3D11_DEPTH_WRITE_MASK GetDepthWriteMask(DepthStencilState::DepthWriteMask dwm)
	{
		switch (dwm)
		{
		case H::Graphics::DepthStencilState::DepthWriteMask::Mask_Zero: return D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		case H::Graphics::DepthStencilState::DepthWriteMask::Mask_All: return D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
			break;
 		}
		return D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	}

	D3D11_COMPARISON_FUNC GetComparison(DepthStencilState::Comparison cmp)
	{
		switch (cmp)
		{
		case H::Graphics::DepthStencilState::Comparison::Never: return D3D11_COMPARISON_NEVER;
			break;
		case H::Graphics::DepthStencilState::Comparison::Less:return D3D11_COMPARISON_LESS;
			break;
		case H::Graphics::DepthStencilState::Comparison::Equal:return D3D11_COMPARISON_EQUAL;
			break;
		case H::Graphics::DepthStencilState::Comparison::LessEqual:return D3D11_COMPARISON_LESS_EQUAL;
			break;
		case H::Graphics::DepthStencilState::Comparison::Greater:return D3D11_COMPARISON_GREATER;
			break;
		case H::Graphics::DepthStencilState::Comparison::NotEqual:return D3D11_COMPARISON_NOT_EQUAL;
			break;
		case H::Graphics::DepthStencilState::Comparison::GreaterEqual:return D3D11_COMPARISON_GREATER_EQUAL;
			break;
		case H::Graphics::DepthStencilState::Comparison::Always:return D3D11_COMPARISON_ALWAYS;
			break;
 		}
		return D3D11_COMPARISON_NEVER;
	}
	D3D11_STENCIL_OP GetStencilOperation(DepthStencilState::StencilOperation sop)
	{
		switch (sop)
		{
		case H::Graphics::DepthStencilState::StencilOperation::Keep: return D3D11_STENCIL_OP_KEEP;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Zero: return D3D11_STENCIL_OP_ZERO;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Replace: return D3D11_STENCIL_OP_REPLACE;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Increment_Saturate: return D3D11_STENCIL_OP_INCR_SAT;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Decrement_Saturate: return D3D11_STENCIL_OP_DECR_SAT;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Invert: return D3D11_STENCIL_OP_INVERT;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Increment: return D3D11_STENCIL_OP_INCR;
			break;
		case H::Graphics::DepthStencilState::StencilOperation::Decrement: return D3D11_STENCIL_OP_DECR;
 			break;
		}
		return D3D11_STENCIL_OP_KEEP;
	}
}
H::Graphics::DepthStencilState::~DepthStencilState()
{
	ASSERT(mDepthStencilState == nullptr, "[DepthStencilState] DepthStencilState not released");

}

void H::Graphics::DepthStencilState::Initialize()
{
	//default, Z test only
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;// 0 close to cam, 1 away from cam


	desc.StencilEnable = false;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_READ_MASK;
 
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS; // is stencikRef val less than current stencil val on this pixel?
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
 	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

 	desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	GetDevice()->CreateDepthStencilState(&desc, &mDepthStencilState);

 
}

void H::Graphics::DepthStencilState::Initialize(bool depthEnable, DepthWriteMask depthWriteMask, Comparison depthFunc, 
	bool stencilEnable, Comparison frontStencilFunc, StencilOperation frontStencilPass, StencilOperation frontStencilFail, StencilOperation frontDepthFail, Comparison backStencilFunc, StencilOperation backStencilPass, StencilOperation backStencilFail, StencilOperation backDepthFail)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = depthEnable;
	desc.DepthWriteMask = GetDepthWriteMask(depthWriteMask);
	desc.DepthFunc = GetComparison(depthFunc);// 0 close to cam, 1 away from cam
 
	desc.StencilEnable = stencilEnable;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilFunc = GetComparison(frontStencilFunc); // is stencikRef val less than current stencil val on this pixel?
	desc.FrontFace.StencilPassOp = GetStencilOperation(frontStencilPass);
	desc.FrontFace.StencilFailOp = GetStencilOperation(frontStencilFail);
	desc.FrontFace.StencilDepthFailOp = GetStencilOperation(frontDepthFail);

	desc.BackFace.StencilFunc = GetComparison(backStencilFunc);  
	desc.BackFace.StencilPassOp = GetStencilOperation(backStencilPass);
	desc.BackFace.StencilFailOp = GetStencilOperation(backStencilFail);
	desc.BackFace.StencilDepthFailOp = GetStencilOperation(backDepthFail);

	GetDevice()->CreateDepthStencilState(&desc, &mDepthStencilState);


}

void H::Graphics::DepthStencilState::Initialize(const DepthState & depthState, const StencilState & stencilState)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = depthState.enable;
	desc.DepthWriteMask = GetDepthWriteMask(depthState.writeMask);
	desc.DepthFunc = GetComparison(depthState.compareFunction);// 0 close to cam, 1 away from cam

	desc.StencilEnable = stencilState.enable;
	desc.StencilReadMask = stencilState.readMask;
	desc.StencilWriteMask = stencilState.writeMask;

	desc.FrontFace.StencilFunc = GetComparison(stencilState.frontCompareFunction); // is stencikRef val less than current stencil val on this pixel?
	desc.FrontFace.StencilPassOp = GetStencilOperation(stencilState.frontPassOp);
	desc.FrontFace.StencilFailOp = GetStencilOperation(stencilState.frontFailOp);
	desc.FrontFace.StencilDepthFailOp = GetStencilOperation(stencilState.frontDepthFailOp);

	desc.BackFace.StencilFunc = GetComparison(stencilState.backCompareFunction);
	desc.BackFace.StencilPassOp = GetStencilOperation(stencilState.backPassOp);
	desc.BackFace.StencilFailOp = GetStencilOperation(stencilState.backFailOp);
	desc.BackFace.StencilDepthFailOp = GetStencilOperation(stencilState.backDepthFailOp);

	GetDevice()->CreateDepthStencilState(&desc, &mDepthStencilState);
}

void H::Graphics::DepthStencilState::Terminate()
{
	SafeRelease(mDepthStencilState);
}

void H::Graphics::DepthStencilState::Set(UINT stencilRef )
{
	GetContext()->OMSetDepthStencilState(mDepthStencilState, stencilRef);
}

void H::Graphics::DepthStencilState::Clear()
{
	GetContext()->OMSetDepthStencilState(nullptr, 0);
}
