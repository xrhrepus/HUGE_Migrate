#ifndef HUGE_GRAPHICS_TEXTUREMANAGER_H
#define HUGE_GRAPHICS_TEXTUREMANAGER_H

#include"Texture.h"

namespace H::Graphics
{
	using TextureId = size_t;

	class TextureManager
	{
	public:
		static void StaticInitialize(std::filesystem::path rootpath);
		static void StaticTerminate();
		static TextureManager* Get();

	public:

		TextureManager() = default;
		~TextureManager();
		void Initialize();
		void SetRootPath(std::filesystem::path rootpath);

		TextureId LoadTexture(std::filesystem::path filename, bool useroot = true);
		const Texture* GetTexture(TextureId textureid) const;

	private:
		std::filesystem::path mRootPath;
		std::unordered_map<TextureId, std::unique_ptr<Texture>> mInventory;
		std::filesystem::path mMissingTexture = L"../../Assets/Images/missingTexture.png";
		TextureId mMissingTextureId;
	};


}//namespace H::Graphics


#endif // !HUGE_GRAPHICS_TEXTUREMANAGER_H

