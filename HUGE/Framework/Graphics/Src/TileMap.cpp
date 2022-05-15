#include "Precompiled.h"
#include "TileMap.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include <fstream>
#include <sstream>

using namespace H;
using namespace H::Graphics;


TileMap::~TileMap()
{

}

void TileMap::LoadTileMap(std::wstring tileMapFileName, std::wstring tileSetFileName)
{
	ASSERT(mIsLoaded == false, "[TileMap] call 'UnloadTileMap' before loading new file ");

	//
	std::ifstream fileToRead;
	//std::ifstream tileSetFile;
	std::string line;
	std::stringstream token;
	//
	fileToRead.open(tileMapFileName);
	if (fileToRead.is_open())
	{
		//read map info
		std::string temp;

		//size of tile
		std::getline(fileToRead, line);
		token = std::stringstream(line);
		token >> temp;
		token >> mSingleTileSize;
		//row
		std::getline(fileToRead, line);
		token = std::stringstream(line);
		token >> temp;
		token >> mRow;
		//col
		std::getline(fileToRead, line);
		token = std::stringstream(line);
		token >> temp;
		token >> mColumn;

		//read tiles
		int entireMapSize = mRow * mColumn;
		mTileMap.resize(entireMapSize);

		char charTemp;

		for (int i = 0; i < mRow; ++i)
		{
			std::getline(fileToRead, line);
			token = std::stringstream(line);

			for (int j = 0; j < mColumn; ++j)
			{
				token >> charTemp;
				mTileMap[i*mColumn + j ] = ( charTemp - '0');
				//mMap[(j + (mColumn*i))] = (charTemp - '0');
			}
		}
		//close file
		fileToRead.close();

		// tile set file
		fileToRead.open(tileSetFileName);

		if (fileToRead.is_open())
		{
			std::string tempStr;// 1* convert string to const char*
			const char* tempCh = " ";
			int textureCount = 0;

			// read texture count
			std::getline(fileToRead, line);
			token = std::stringstream(line);
			token >> tempStr;
			token >> textureCount;


			//read tile name
			for (int i = 0; i < textureCount; ++i)
			{
				std::getline(fileToRead, line);
				token = std::stringstream(line);
				token >> tempStr;// 2* convert string to const char*

				//190802 - - convert string to wstring 
				std::wstring tName(tempStr.length(),L' ');
				std::move(tempStr.begin(), tempStr.end(), tName.begin());

				//load texture
				mTileSet.push_back(TextureManager::Get()->LoadTexture(tName));
			}
			//close file
			fileToRead.close();
		}

		mIsLoaded = true;
	}




}
void TileMap::UnLoadTileMap()
{
	ASSERT(mIsLoaded == true, "[TileMap] not loaded ");

	mTileMap.clear();
	mTileSet.clear();
	mIsLoaded = false;
}
void TileMap::Render(const Camera& cam)
{
	Math::Vector2 tempVec = { 0.0f,0.0f };
	TextureId tempID;


	for (int i = 0; i < mRow; i++)
	{
		tempVec.y = static_cast<float>(i * mSingleTileSize);

		for (int j = 0; j < mColumn; j++)
		{
			tempVec.x = static_cast<float>(j * mSingleTileSize);
			tempID = mTileSet[ mTileMap[i * mColumn + j]];
			
			SpriteRenderer::Get()->Draw(*TextureManager::Get()->GetTexture(tempID), tempVec);

		}
	}
	//SpriteRenderer::Get()->Draw();
}
void H::Graphics::TileMap::SaveTileMapToFile(std::wstring tileMapFileName)
{


}
void TileMap::GetTileMapInfo(std::vector<int>& tilemap, int& row, int& column) const
{
	tilemap = mTileMap;
	row = mRow;
	column = mColumn;
}


const std::vector<int>& TileMap::GetTileMap() const
{
	return mTileMap;
}
std::vector<int>& TileMap::GetTileMap()
{
	return mTileMap;
}

const std::vector<TextureId>& H::Graphics::TileMap::GetTileSet() const
{
	return mTileSet;
}

int TileMap::GetMapRow()const
{
	return mRow;
}
int TileMap::GetMapColumn()const
{
	return mColumn;
}

int H::Graphics::TileMap::GetTileSize() const
{
	return mSingleTileSize;
}
