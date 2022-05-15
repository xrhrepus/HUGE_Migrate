#ifndef HUGE_GRAPHICS_MESHBUILDER_H
#define HUGE_GRAPHICS_MESHBUILDER_H
#include "Mesh.h"

namespace H::Graphics {


	struct Bone;

	class MeshBuilder
	{
	public:
		//MeshBuilder();
		//~MeshBuilder();
		static MeshPC CreateCubePC();
		static MeshPC CreateQuadPC(int size, const Vector3& centre,const Color& color);
		static MeshPC CreateCirclePC(float radius, int side, const Vector3& centre, const Color& color);

		static MeshPC CreatePlane(int size,int row,int column,const Vector3& centre, const Color& color) ;
		static MeshPC CreateCylinder(int height, float radius,int side, const Vector3& centre, const Color& color);
		static MeshPC CreateShperePC(float radius, int side, const Vector3& centre, const Color& color);
		static MeshPC CreateCone(int height, float radius, int side, const Vector3& centre, const Color& color);

		static MeshPX CreateQuadPX();
		static MeshPX CreateNDCQuadPX();

		static MeshPX CreateShperePX(float radius, int side, const Vector3& centre);
		static MeshPX CreateSkyBoxPX(float radius, int side, const Vector3& centre);

		static MeshPN CreateShperePN(float radius, int side, const Vector3& centre);

		static MeshPN CreateCubePN(float xExtend, float yExtend, float zExtend, const Vector3& centre);

		//standard
		static Mesh CreateShpere(float radius, int side, const Vector3& centre);
		static Mesh CreateQuad(float xExtend, float yExtend, const Vector3& centre);
		static Mesh CreateTerrain(uint32_t numRow, uint32_t numCol, float sizeOfOneBlock = 1.0f, const Vector3& startPos = Vector3::Zero());

		//extend : 1/2 of length
		static Mesh CreateCube(float xExtend, float yExtend, float zExtend, const Vector3& centre);

		static Math::AABB ComputeBound(const Mesh& mesh);


		//bonevertex
		static BoneMesh CreateTentacle(size_t numbones, float height, float radius, int slice, const Vector3& centre);
		static BoneMesh CreateFourBoneTentacle(float height, float radius, int slice, const Vector3& centre);

		private:
			//static const float mUnit{ 1.0f };

		};




} // namespace HUGE::graphics




#endif // !HUGE_GRAPHIC_MESHBUILDER

