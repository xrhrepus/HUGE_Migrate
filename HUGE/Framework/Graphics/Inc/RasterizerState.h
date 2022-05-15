#ifndef HUGE_GRAPHICS_RASTERIZERSTATE_H
#define HUGE_GRAPHICS_RASTERIZERSTATE_H

namespace H::Graphics
{


class RasterizerState
{
public:
	enum class CullMode	{Front,Back,None};

	enum class FillMode	{Solid,WireFrame};

	RasterizerState()= default;
	~RasterizerState();

	RasterizerState(const RasterizerState&) = delete;
	RasterizerState& operator=(const RasterizerState&) = delete;

	void Initialize(CullMode cullMode, FillMode fillMode);
	void Terminate();

	void Set();
	void Clear();


private:
	ID3D11RasterizerState* mRasterizerState{ nullptr };

};

}//namespace H::Graphics

 

#endif // !HUGE_GRAPHICS_RASTERIZERSTATE_H

