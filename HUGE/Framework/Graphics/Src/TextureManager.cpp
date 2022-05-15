#include "Precompiled.h"
#include "TextureManager.h"

using namespace H;
using namespace H::Graphics;

namespace 
{
	std::unique_ptr<TextureManager> sTextureManager;
}

void TextureManager::StaticInitialize(std::filesystem::path rootpath)
{
	ASSERT(sTextureManager == nullptr, "[Graphic::TextureManager] system already initialized!");
	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->SetRootPath(std::move(rootpath));
	sTextureManager->Initialize();
}
 void TextureManager::StaticTerminate()
 {
	 if (sTextureManager != nullptr)
	 {
		 sTextureManager.reset();
	 }

 }
 TextureManager* TextureManager::Get()
 {
	 ASSERT(sTextureManager != nullptr, "[Graphic::TextureManager] instance not created");
	 return sTextureManager.get();
 }

 TextureManager::~TextureManager()
 {
	 for (auto&[key,value] : mInventory)
	 {
		 value->Terminate();
	 }
 }

 void H::Graphics::TextureManager::Initialize()
 {
	 auto hash = std::filesystem::hash_value(mMissingTexture);
 	 auto[iter, success] = mInventory.insert(std::make_pair(hash, nullptr));
	 if (success)
	 {
		 auto& texPtr = iter->second;
		 texPtr = std::make_unique<Texture>();
		 texPtr->Initialize(mMissingTexture);
	 }
	 mMissingTextureId = hash;
 }

 void TextureManager::SetRootPath(std::filesystem::path rootpath)
 {
	 ASSERT(std::filesystem::is_directory(rootpath), "[Graphic::TextureManager] %ls is not a directory", rootpath.c_str());

	 mRootPath = std::move(rootpath);
	 //mRootPath /= "folder";
	 
	 //if (mRootPath.back() != L'/' )
	 //{
		// mRootPath += L'/';
	 //}

 }

 TextureId TextureManager::LoadTexture(std::filesystem::path filename, bool useroot)
 {
	 if (filename.empty())
	 {
		 return mMissingTextureId;
	 }
	 auto hash = std::filesystem::hash_value(filename);
 	 //if (mInventory.find(hash) == mInventory.end())
	 //{
		////auto filePath = mRootPath / filename;
		//auto texture = std::make_unique<Texture>();
		//auto texFile = useroot ? mRootPath / filename : filename;
		//texture->Initialize(texFile);
		//mInventory.emplace(hash, std::move(texture));
		//
	 //}
	 //
	 
	 auto [iter,success] =  mInventory.insert(std::make_pair(hash, nullptr));
	 if (success)
	 {
		 auto& texPtr = iter->second;
		 texPtr = std::make_unique<Texture>();
		 texPtr->Initialize(useroot ? mRootPath / filename : filename);
	 }

	 return hash;

 }
 const Texture* TextureManager::GetTexture(TextureId textureid) const
 {
	 //NOTE190801  c++17
	 if (auto iter = mInventory.find(textureid);//create iter within if scope
		 iter!=mInventory.end())//if condition
	 {
		 return iter->second.get();
	 }

	 return nullptr;
 }
