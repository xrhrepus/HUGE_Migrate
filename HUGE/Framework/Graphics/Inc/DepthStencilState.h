#ifndef HUGE_GRAPHICS_DEPTHSTENCILSTATE_H
#define HUGE_GRAPHICS_DEPTHSTENCILSTATE_H

namespace H::Graphics
{
	class DepthStencilState
	{
	public:
#pragma region Attributes

		enum DepthWriteMask
		{
			Mask_Zero, Mask_All
		};
		enum Comparison
		{
			Never = 1,Less,Equal,LessEqual,Greater,NotEqual,GreaterEqual,Always
		};
		enum StencilOperation
		{
			Keep = 1,Zero,Replace, Increment_Saturate, Decrement_Saturate,Invert,Increment,Decrement
		};

		struct DepthState
		{
			bool enable = false;
			DepthWriteMask writeMask;
			Comparison compareFunction;
		};
		struct StencilState
		{
			bool enable = false;
			UINT8 readMask;
			UINT8 writeMask;
			Comparison frontCompareFunction;
			StencilOperation frontPassOp;
			StencilOperation frontFailOp;
			StencilOperation frontDepthFailOp; //The stencil operation to perform when stencil testing passes and depth testing fails.
			Comparison backCompareFunction;
			StencilOperation backPassOp;
			StencilOperation backFailOp;
			StencilOperation backDepthFailOp;
		};
 #pragma endregion


		DepthStencilState() = default;
		~DepthStencilState();

		DepthStencilState(const DepthStencilState&) = delete;
		DepthStencilState& operator=(const DepthStencilState&) = delete;

		void Initialize();
		void Initialize(bool depthEnable, DepthWriteMask depthWriteMask, Comparison depthFunc,
			bool stencilEnable, Comparison frontStencilFunc,StencilOperation frontStencilPass, StencilOperation frontStencilFail, StencilOperation frontDepthFail,
			Comparison backStencilFunc, StencilOperation backStencilPass, StencilOperation backStencilFail, StencilOperation backDepthFail);
		void Initialize(const DepthState& depthState, const StencilState& stencilState);

		void Terminate();

		void Set(UINT stencilRef = 0);
		void Clear();

	private:
		ID3D11DepthStencilState* mDepthStencilState = nullptr;
	};

} // namespace H::Graphics

 
#endif // !HUGE_GRAPHICS_DEPTHSTENCILSTATE_H

