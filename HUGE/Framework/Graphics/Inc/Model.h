#ifndef HUGE_GRAPHICS_MODEL_H
#define HUGE_GRAPHICS_MODEL_H

#include "Mesh.h"
#include "MeshBuffer.h"

namespace H::Graphics
{
	class Animation;

	class Model
	{
	public:
		void Initialize(std::filesystem::path infile);
		void Terminate();

		void Render();

		struct MeshData
		{
			BoneMesh mesh;
			uint32_t materialIndex = 0;
			MeshBuffer meshBuffer;
		};

		struct MaterialData
		{
			std::string diffuseMapName;
			H::Graphics::TextureId diffuseMapId = 0;

			std::string specularMapName;
			H::Graphics::TextureId specularMapId = 0;

			std::string normalMapName;
			H::Graphics::TextureId normalMapId = 0;

		};

		struct AnimationData
		{
			std::string animationName;
			Animation animation;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		std::vector<AnimationData> animationData;

 
	};
}

#endif // !HUGE_GRAPHICS_MODEL_H

