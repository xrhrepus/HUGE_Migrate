
#ifndef HUGE_TERRAIN_H
#define HUGE_TERRAIN_H

#include "Quadtree.h"
namespace H
{
	class Terrain
	{
	public:
		Terrain() = default;

		void Initialize(uint32_t numRows, uint32_t numCols, float scale);
		void Terminate();

		bool Intersect(H::Math::Ray ray, float& distance) const;

		void Render(const H::Graphics::Camera& camera);

		void DrawEditorUI();

	private:
		void GenerateVertices();
		void GenerateIndices();

		struct ConstantData
		{
			H::Math::Matrix4 world;
			H::Math::Matrix4 wvp;
			H::Math::Vector4 viewPosition;
			H::Graphics::DirectionalLight directionalLight;
		};

		H::Graphics::TypedConstantBuffer<ConstantData> mConstantBuffer;
		H::Graphics::VertexShader mTerrainVertexShader;
		H::Graphics::PixelShader mTerrainPixelShader;

		H::Graphics::Sampler* mSampler;

		H::Graphics::Texture mTerrainTexture;

		H::Graphics::Mesh mMesh;
		H::Graphics::MeshBuffer mMeshBuffer;
		
		H::Quadtree mQuadtree;

		uint32_t mNumRows{ 0 };
		uint32_t mNumCols{ 0 };
		uint32_t mNumCellsInCol{ 0 };
		uint32_t mNumCellsInRow{ 0 };
		uint32_t mNumCells{ 0 };

		float mScale{ 1.0f };
		float mFrequency0{ 5.0f };
		float mFrequency1{ 40.0f };
		float mWeight0{ 25.0f };
		float mWeight1{ 1.0f };

		bool showQuadtree = false;

	};

}


#endif // !HUGE_TERRAIN_H

 