#ifndef HUGE_GRAPHICS_MESHIO_H
#define HUGE_GRAPHICS_MESHIO_H

namespace H::Graphics
{
	template<class MeshType>
	struct MeshBase;
	struct Vertex;
  	using Mesh = MeshBase<Vertex>;

	struct BoneVertex;
	using BoneMesh = MeshBase<BoneVertex>;

	class Model;
	struct Skeleton;

	class BoneAnimation;
	using BoneAnimations = std::vector<std::unique_ptr<BoneAnimation>>;

	class AnimationClip;
	using AnimationClips = std::vector<std::unique_ptr< AnimationClip>>;


	namespace MeshIO
	{
 		bool Save(const H::Graphics::BoneMesh& mesh, std::filesystem::path outfile);

		bool Load(H::Graphics::BoneMesh& mesh, std::filesystem::path infile);
		bool Load(std::vector<H::Graphics::BoneMesh>& meshs, std::filesystem::path infile);
 		bool SaveModel(const H::Graphics::Model& model, std::filesystem::path outfile);
		bool LoadModel(H::Graphics::Model& model, std::filesystem::path fromfile);

		bool SaveSkeleton(const H::Graphics::Skeleton& skeleton, std::filesystem::path outfile);
		bool LoadSkeleton(H::Graphics::Skeleton& skeleton, std::filesystem::path fromfile);

		bool SaveAnimationClips(const H::Graphics::AnimationClips& animClips, std::filesystem::path outfile);
		bool LoadAnimationClips(H::Graphics::AnimationClips& animClips, std::filesystem::path fromfile);

		bool LoadAnimationClip(std::unique_ptr < H::Graphics::AnimationClip>& animClip, std::filesystem::path fromfile);

	}

	

}//H::Graphics


 

#endif // !HUGE_GRAPHICS_MESHIO_H

