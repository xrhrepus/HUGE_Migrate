#include "Precompiled.h"
#include "Terrain.h"
#include "PerlinNoise.h"

using namespace H;


void Terrain::Initialize(uint32_t numRows, uint32_t numCols, float scale)
{
	mNumRows = numRows;
	mNumCols = numCols;
	mNumCellsInCol = mNumRows - 1;
	mNumCellsInRow = mNumCols - 1;
	mNumCells = mNumCellsInCol * mNumCellsInRow;
	mScale = scale;

	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(L"../../Assets/Shaders/Terrain.fx");
	mTerrainPixelShader.Initialize(L"../../Assets/Shaders/Terrain.fx","PS");

	mSampler = (SamplerManager::Get()->GetSampler(SamplerType::AnisotropicWrap));
	mTerrainTexture.Initialize("../../Assets/Images/fruit42.png");

	
	//mMesh = MeshBuilder::CreateTerrain(mNumRows, mNumCols, mScale);
	mMesh.vertices.resize(numRows * numCols);
	mMesh.indices.resize(mNumCells * 6);

	GenerateIndices();
	GenerateVertices();

	mMeshBuffer.Initialize(mMesh, true);
	mQuadtree.Initialize(&mMesh, 5);
}

void Terrain::Terminate()
{
	mMeshBuffer.Terminate();
 	mTerrainTexture.Terminate();
 
	mTerrainPixelShader.Terminate();
	mTerrainVertexShader.Terminate();
	mConstantBuffer.Terminate();
}

bool Terrain::Intersect(H::Math::Ray ray, float & distance) const
{
	return mQuadtree.Intersect(ray, distance);
}

void Terrain::Render(const H::Graphics::Camera & camera)
{
	DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
	BlendStateManager::Get()->GetBlendState(Blend::AlphaBlend)->Set();

	if (showQuadtree)
	{
		Quadtree::Visitor visitor = [](Quadtree::Node* node) {
			node->DrawNode();
			return true;
		};
		mQuadtree.Visit(visitor);
	}

	auto world = Math::Matrix4::Identity();
	auto view = camera.GetViewMatrix();
	auto projection = camera.GetPerspectiveMatrix();

	ConstantData data;
	data.world = Math::Transpose(world);
	data.wvp = Math::Transpose(world * view * projection);
	const auto& camPos = camera.GetPosition();
	data.viewPosition = { camPos.x ,camPos.y,camPos.z, 1.0f };
	data.directionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	data.directionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
	data.directionalLight.diffuse = { 0.6f, 0.6f, 0.6f, 1.0f };
	data.directionalLight.specular = { 0.1f, 0.1f, 0.1f, 1.0f };
	
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mSampler->BindPS(0);
	mTerrainTexture.BindPS(0);

	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();

	mMeshBuffer.Render();
}

void Terrain::DrawEditorUI()
{
	if (ImGui::CollapsingHeader("Terrain Editor"))
	{
		ImGui::Checkbox("showquadtree", &showQuadtree);
		if (ImGui::SliderFloat("Frequency 0", &mFrequency0, 0.0f, 100.0f) ||
			ImGui::SliderFloat("Frequency 1", &mFrequency1, 0.0f, 100.0f) ||
			ImGui::SliderFloat("Weight 0", &mWeight0, 0.0f, 50.0f) ||
			ImGui::SliderFloat("Weight 1", &mWeight1, 0.0f, 50.0f))
		{
			GenerateVertices();
			mMeshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());
		}
	}
}

void Terrain::GenerateVertices()
{
	// Generate mesh for the terrain
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	// | \ | \ | \ |
	// +---+---+---+
	//std::unique_ptr<float[]> heightValues = std::make_unique<float[]>(numRows * numCols);
	//
	//FILE* file = nullptr;
	//fopen_s(&file, "../Assets/Heightmaps/heightmap_200x200.raw", "rb");
	//for (uint32_t i = 0; i < numRows * numCols; ++i)
	//{
	//	heightValues[i] = fgetc(file) / 255.0f * 10.0f;
	//}
	//fclose(file);

	PerlinNoise noise;
	for (uint32_t z = 0; z < mNumRows; ++z)
	{
		for (uint32_t x = 0; x < mNumCols; ++x)
		{
			uint32_t index = x + (z * mNumCols);
			auto& vertex = mMesh.vertices[index];

			float normX = (float)x / mNumCols;
			float normZ = (float)z / mNumRows;
			float noise0 = mWeight0 * noise.Get(mFrequency0 * normX, 0.0f, mFrequency0 * normZ);
			float noise1 = mWeight1 * noise.Get(mFrequency1 * normX, 0.0f, mFrequency1 * normZ);

			vertex.position =
			{
				(float)x * mScale,
				//heightValues[index],
				vertex.position.y = (noise0 + noise1),
				(float)z * mScale
			};
			vertex.normal = { 0.0f, 1.0f, 0.0f };
			vertex.tangent = { 1.0f, 0.0f, 0.0f };
			vertex.texCoord.x = normX;
			vertex.texCoord.y = normZ;
		}
	}

	//Graphics::MeshBuilder::ComputeNormals(mMesh);
}

void Terrain::GenerateIndices()
{
	uint32_t i = 0;
	for (uint32_t z = 0; z < mNumCellsInCol; ++z)
	{
		for (uint32_t x = 0; x < mNumCellsInRow; ++x)
		{
			uint32_t lb = x + (z * mNumCols);
			uint32_t lt = x + ((z + 1) * mNumCols);
			uint32_t rb = (x + 1) + (z * mNumCols);
			uint32_t rt = (x + 1) + ((z + 1) * mNumCols);

			mMesh.indices[i++] = lb;
			mMesh.indices[i++] = lt;
			mMesh.indices[i++] = rb;
			mMesh.indices[i++] = lt;
			mMesh.indices[i++] = rt;
			mMesh.indices[i++] = rb;
		}
	}
}