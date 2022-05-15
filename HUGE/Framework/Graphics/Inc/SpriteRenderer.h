#ifndef HUGE_GRAPHIC_SPRITERENDERER_H
#define HUGE_GRAPHIC_SPRITERENDERER_H

namespace DirectX { class SpriteBatch; }

namespace H::Graphics
{
	class Texture;

	class SpriteRenderer
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SpriteRenderer* Get();

		SpriteRenderer() = default;
		~SpriteRenderer();

		void Initialize();
		void Terminate();

		void BeginRender();
		void EndRender();

		void Draw(const Texture& texture, const H::Math::Vector2& pos);
		

		

	private:
		std::unique_ptr< DirectX::SpriteBatch> mSpriteBatch;


	};

}//H::Graphics






#endif // !HUGE_GRAPHIC_SPRITERENDERER_H

