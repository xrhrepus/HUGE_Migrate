#ifndef HUGE_GRAPHICS_D3DUTIL_H
#define HUGE_GRAPHICS_D3DUTIL_H
namespace H::Graphics
{
	//
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();


	//
	class Texture;
	ID3D11ShaderResourceView* GetShaderResourceView(const Texture& texture);

}


#endif // !HUGE_GRAPHICS_D3DUTIL_H
