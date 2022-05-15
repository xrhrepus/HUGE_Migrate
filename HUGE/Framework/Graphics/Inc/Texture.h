#ifndef HUGE_GRAPHICS_TEXTURE_H
#define HUGE_GRAPHICS_TEXTURE_H


namespace H::Graphics {

	class Texture
	{
	public:
		Texture() = default;
		~Texture();

		void Initialize(const std::filesystem::path filename);
		void Terminate();

		void BindPS(uint32_t slot = 0) const;
		void BindVS(uint32_t slot = 0) const;

		void UnBindPS(uint32_t slot = 0) const;
		void UnBindVS(uint32_t slot = 0) const;

	private:
		friend 	ID3D11ShaderResourceView* GetShaderResourceView(const Texture& texture);


		ID3D11ShaderResourceView* mShaderResourceView{ nullptr };
	};





}// HUGE GRAPHICS

#endif // !HUGE_GRAPHIC_TEXTURE_H

