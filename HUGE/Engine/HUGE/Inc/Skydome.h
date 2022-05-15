#ifndef HUGE_SKYDOME_H
#define HUGE_SKYDOME_H
#include <Graphics/Inc/Graphics.h>

namespace H
{
	class H::Graphics::Camera;

	class Skydome
	{
	public:
		Skydome() = default;
		void Initialize(float radius = 450.0f, int slice = 6);
		void Terminate();
		void Render(const H::Graphics::Camera& camera);
		void DrawEditorUI();

	private:
		const wchar_t* TEXTURING_FILE_PATH = L"../../Assets/Shaders/Texturing.fx";

 		struct TSkyDome_Data
		{
			H::Math::Matrix4 padding;
			H::Math::Matrix4 wvp;
		};

		H::Graphics::VertexShader mVS;
		H::Graphics::PixelShader mPS;
		H::Graphics::Sampler* mSampler;
		TSkyDome_Data mData;
		H::Graphics::TypedConstantBuffer<TSkyDome_Data> mCB;

		H::Graphics::TextureId mDiffuseId;

		H::Graphics::MeshPX mSkyDomeMesh;
		H::Graphics::MeshBuffer mMB;
		
 		//float mScale = 1.0f;
		H::Math::Matrix4 mTF = H::Math::Matrix4::Identity() * H::Math::Matrix4::rotationX(90.0f * H::Math::Constants::DegToRad);

 		bool showQuadtree = false;

	};

}


#endif // !HUGE_TERRAIN_H

 