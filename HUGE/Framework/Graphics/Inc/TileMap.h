#ifndef HUGE_GRAPHICS_TILEMAP_H
#define HUGE_GRAPHICS_TILEMAP_H


namespace H::Graphics
{
	using TextureId = size_t;
	class Camera;

	class TileMap
	{
	public:
		TileMap() = default;
		~TileMap();

		void LoadTileMap(std::wstring tileMapFileName, std::wstring tileSetFileName);
		void UnLoadTileMap();
		void Render(const Camera& cam);

		void SaveTileMapToFile(std::wstring tileMapFileName);


		//get info
		void GetTileMapInfo(std::vector<int>& tilemap, int& row , int& column) const;

		const std::vector<int>& GetTileMap() const;
		std::vector<int>& GetTileMap();
		const std::vector<TextureId>& GetTileSet() const;

		int GetMapRow()const;
		int GetMapColumn()const;
		int GetTileSize()const;

	private:
		std::vector<int> mTileMap;
		std::vector<TextureId> mTileSet;

		bool mIsLoaded = false;

		int mRow;
		int mColumn;
		int mSingleTileSize;

	};

	
}//namespace H::Graphics




#endif // !HUGE_GRAPHICS_TILEMAP_H

