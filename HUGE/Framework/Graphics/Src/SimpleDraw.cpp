#include "Precompiled.h"
#include "SimpleDraw.h"

#include "GraphicsSystem.h"
#include "Camera.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexType.h"
#include "MeshBuffer.h"
#include "RasterizerStateManager.h"

using namespace H;
using namespace H::Math;
using namespace H::Graphics;

namespace
{
	//NOTE private implementation
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertCount);
		void Terminate();
		void Render(const Camera& cam);
		void Render2D();
		void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Graphics::Color& color);
		void AddTriangle(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Graphics::Color& color);

		void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Graphics::Color& color);

	private:
		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		MeshBuffer mMeshBuffer;
		std::vector<VertexPC> mVertices;
		std::vector<VertexPC> mSolidVertices;

		std::vector<VertexPC> m2DVertices;

		bool mInitialized{ false };
		//uint32_t mMaxVertexCount{ 0 };
		//uint32_t mVertexCount{ 0 };

	};

	void SimpleDrawImpl::Initialize(uint32_t maxVertCount)
	{
		ASSERT(!mInitialized, "[SimpleDraw] Already initialized");

		mVertexShader.Initialize(VertexPC::Format, L"../../Assets/Shaders/SimpleDraw.fx");
		mPixelShader.Initialize(L"../../Assets/Shaders/SimpleDraw.fx", "PS");
		mConstantBuffer.Initialize(sizeof(Matrix4));
		mMeshBuffer.Initialize(nullptr, maxVertCount, sizeof(VertexPC), true);//NOTE dynamic
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);//
		mVertices.reserve(maxVertCount);
		m2DVertices.reserve(maxVertCount);
		mSolidVertices.reserve(maxVertCount);
		mInitialized = true;
	}
	void SimpleDrawImpl::Terminate()
	{
		ASSERT(mInitialized, "[SimpleDraw] Already Terminated");
		
		mVertexShader.Terminate();
		mPixelShader.Terminate();
		mConstantBuffer.Terminate();
		mMeshBuffer.Terminate();
		mInitialized = false;
		
	}
	void SimpleDrawImpl::Render(const Camera& cam)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized");

		GraphicSystem* system = GraphicSystem::Get();

		//draw 3d
		const Matrix4 matView = cam.GetViewMatrix();
		const Matrix4 matProj = cam.GetPerspectiveMatrix();

		auto transform = (matView * matProj).Transpose();
		mConstantBuffer.Set(&transform);
		mConstantBuffer.BindVS();

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(mVertices.data(), static_cast<uint32_t>(mVertices.size()));
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);//
		mMeshBuffer.Render();

		//draw 3d solid
		mMeshBuffer.Update(mSolidVertices.data(), static_cast<uint32_t>(mSolidVertices.size()));
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
		mMeshBuffer.Render();

		mVertices.clear();//?
		mSolidVertices.clear();
	}

	void SimpleDrawImpl::Render2D()
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized");

		GraphicSystem* system = GraphicSystem::Get();

		//draw 2d
		const uint32_t w = system->GetBackBufferWidth();
		const uint32_t h = system->GetBackBufferHeight();
		Math::Matrix4 matInvScreen // ??
		(
			2.0f / w,	0.0f,		0.0f, 0.0f,
			0.0f,		-2.0f / h,	0.0f, 0.0f,
			0.0f,		0.0f,		1.0f, 0.0f,
			-1.0f,		1.0f,		0.0f, 1.0f
		);
		Matrix4 transform = (matInvScreen).Transpose();
		mConstantBuffer.Set(&transform);
		mConstantBuffer.BindVS();

		mVertexShader.Bind();
		mPixelShader.Bind();

		mMeshBuffer.Update(m2DVertices.data(), static_cast<uint32_t>(m2DVertices.size()));
		mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);//
		mMeshBuffer.Render();

		m2DVertices.clear();
	}

	void  SimpleDrawImpl::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Graphics::Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized");
		if (mVertices.size() + 2 <= mVertices.capacity())
		{
			mVertices.emplace_back(v0, color);
			mVertices.emplace_back(v1, color);
		}

	}
	void  SimpleDrawImpl::AddTriangle(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Graphics::Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized");
		if (mSolidVertices.size() + 3 <= mSolidVertices.capacity())
		{
			mSolidVertices.emplace_back(v0, color);
			mSolidVertices.emplace_back(v1, color);
			mSolidVertices.emplace_back(v2, color);
		}

	}

	void  SimpleDrawImpl::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Graphics::Color& color)
	{
		ASSERT(mInitialized, "[SimpleDraw] Not initialized");
		if (m2DVertices.size() + 2 <= m2DVertices.capacity())
		{
			m2DVertices.emplace_back(Vector3(v0.x, v0.y, 0.0f), color);
			m2DVertices.emplace_back(Vector3(v1.x, v1.y, 0.0f), color);
		}

	}

	std::unique_ptr<SimpleDrawImpl> sInstance;

}


void SimpleDraw::StaticInitialize(uint32_t maxVertCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertCount);

}
void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Graphics::Color& color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddBox(const Math::AABB& aabb, const Graphics::Color& color)
{
	//Vector3 lt, rt, lb, rb; // x and y direction
	float Min_x = aabb.mCentre.x - 0.5f * aabb.mExtend.x;
	float Min_y = aabb.mCentre.y - 0.5f * aabb.mExtend.y;
	float Min_z = aabb.mCentre.z - 0.5f * aabb.mExtend.z;
	float Max_x = aabb.mCentre.x + 0.5f * aabb.mExtend.x;
	float Max_y = aabb.mCentre.y + 0.5f * aabb.mExtend.y;
	float Max_z = aabb.mCentre.z + 0.5f * aabb.mExtend.z;

	Vector3 lt = { Min_x,Max_y,Max_z };
	Vector3 rt = { Max_x,Max_y,Max_z };
	Vector3 lb = { Min_x,Min_y,Max_z };
	Vector3 rb = { Max_x,Min_y,Max_z };

	Vector3 lt2 = { Min_x,Max_y,Min_z };
	Vector3 rt2 = { Max_x,Max_y,Min_z };
	Vector3 lb2 = { Min_x,Min_y,Min_z };
	Vector3 rb2 = { Max_x,Min_y,Min_z };

	//add line
	//top 4
	sInstance->AddLine(lt, rt, color);
	sInstance->AddLine(lt, lb, color);
	sInstance->AddLine(rt, rb, color);
	sInstance->AddLine(lb, rb, color);
	//bot 4
	sInstance->AddLine(lt2, rt2, color);
	sInstance->AddLine(lt2, lb2, color);
	sInstance->AddLine(rt2, rb2, color);
	sInstance->AddLine(lb2, rb2, color);
	//side
	sInstance->AddLine(lt, lt2, color);
	sInstance->AddLine(rt, rt2, color);
	sInstance->AddLine(lb, lb2, color);
	sInstance->AddLine(rb, rb2, color);
	

}

void SimpleDraw::AddShpere(const Math::Sphere& sphere, const Graphics::Color& color, const size_t& side)
{
	const float deg_step = (0.5f * Math::Constants::Pi) / static_cast<float>(side);
	const float z_step = Math::Constants::Pi / static_cast<float>(side);

	const size_t layers = (side * 2 - 1);
	const size_t vtx_in_circle = side * 4 + 1; // num of verts in one circle/layer
	const size_t total_amount_of_vertex = 8 * side*side - 2 * side + 1;


	float curr_radius = sphere.mRadius;
	float curr_deg = 0.0f;
	float curr_z = sphere.mRadius;
	float curr_z_deg = 0.0f;

	Vector3 sphere_centre = sphere.mCentre;
	//vertex
	std::vector<Vector3> vtx;// position only
	vtx.reserve(total_amount_of_vertex);

	//top
	vtx.push_back({ sphere_centre.x ,sphere_centre.y,sphere_centre.z + curr_z });
	
	//loop
	for (size_t i = 0; i < layers; i++) // from  + 1/2 z to - 1/2 z
	{
		curr_z_deg += deg_step;
		curr_z = sphere.mRadius * cosf(curr_z_deg);
		curr_radius = abs(sphere.mRadius * (sinf(curr_z_deg)));
		curr_deg = 0.0f;

		for (size_t j = 0; j < vtx_in_circle; j++)
		{
			vtx.push_back({ sphere_centre.x + curr_radius* cosf(curr_deg), sphere_centre.y + curr_radius * sinf(curr_deg), sphere_centre.z + curr_z });
			curr_deg += deg_step;
		}
	}
	//bot
	vtx.push_back({ sphere_centre.x ,sphere_centre.y, sphere_centre.z-sphere.mRadius });

	//add lines vertical
	size_t start_index = 1;

	//top
	for (size_t i = 0; i < vtx_in_circle; i++)// 0,1 0,2 ... 0,i
	{
		sInstance->AddLine(vtx[0], vtx[i], color);
	}
	//loop
	for (size_t i = 0; i < (side - 1) * 2 ; i++)// for each circle/ring
	{
		for (size_t j = start_index; j < vtx_in_circle + start_index; j++)//each vtx connect to vtx on next layer
		{
			sInstance->AddLine(vtx[j], vtx[j + vtx_in_circle], color);
		}
		start_index += vtx_in_circle;
	}
	//bot
	const size_t b_start_index = vtx.size() - 1 - vtx_in_circle;

	for (size_t i = b_start_index; i < vtx_in_circle + b_start_index; i++)// 0,1 0,2 ... 0,i
	{
		sInstance->AddLine(vtx[vtx.size() - 1], vtx[i], color);
	}
	//add lines horizontal
	start_index = 1;

	for (size_t i = 0; i < (side - 1) * 2 + 1; i++)// for each circle/ring
	{
		size_t si = start_index;
		int j = start_index;
		for (; j < vtx_in_circle + start_index - 1; j++)//exclude last one
		{
			sInstance->AddLine(vtx[j], vtx[j + 1], color);
		}
		//last one
		sInstance->AddLine(vtx[j], vtx[si], color);

		start_index += vtx_in_circle;
	}


}

 
void H::Graphics::SimpleDraw::AddSolidBox(const Math::AABB & aabb, const Graphics::Color & color)
{
	//Vector3 lt, rt, lb, rb; // x and y direction
	float Min_x = aabb.mCentre.x - 0.5f * aabb.mExtend.x;
	float Min_y = aabb.mCentre.y - 0.5f * aabb.mExtend.y;
	float Min_z = aabb.mCentre.z - 0.5f * aabb.mExtend.z;
	float Max_x = aabb.mCentre.x + 0.5f * aabb.mExtend.x;
	float Max_y = aabb.mCentre.y + 0.5f * aabb.mExtend.y;
	float Max_z = aabb.mCentre.z + 0.5f * aabb.mExtend.z;

	Vector3 lt = { Min_x,Max_y,Max_z };
	Vector3 rt = { Max_x,Max_y,Max_z };
	Vector3 lb = { Min_x,Min_y,Max_z };
	Vector3 rb = { Max_x,Min_y,Max_z };

	Vector3 lt2 = { Min_x,Max_y,Min_z };
	Vector3 rt2 = { Max_x,Max_y,Min_z };
	Vector3 lb2 = { Min_x,Min_y,Min_z };
	Vector3 rb2 = { Max_x,Min_y,Min_z };

	//front
	sInstance->AddTriangle(lt, rt2, lt2,color);
	sInstance->AddTriangle(lt, rt, rt2, color);

	sInstance->AddTriangle(rt, rb, rb2,color);
	sInstance->AddTriangle(rt, rb2, rt2,color);

	sInstance->AddTriangle(lb, rt, lt, color);
	sInstance->AddTriangle(lb, rb, rt, color);
	//back

	sInstance->AddTriangle(rb, lb, lb2, color);
	sInstance->AddTriangle(rb, lb2, rb2, color);

	sInstance->AddTriangle(lb, lt, lt2, color);
	sInstance->AddTriangle(lb, lt2, lb2, color);

	

	sInstance->AddTriangle(lb2, lt2, rt2, color);
	sInstance->AddTriangle(lb2, rt2, rb2, color);


}

void SimpleDraw::AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Graphics::Color& color)
{
	sInstance->AddScreenLine(v0, v1, color);
}
void SimpleDraw::AddScreenRect(const Math::Rect& rect, const Graphics::Color& color)
{
	float min_x = rect.mCentre.x -  rect.mExtend.x;
	float min_y = rect.mCentre.y -  rect.mExtend.y;	

	float max_x = rect.mCentre.x +  rect.mExtend.x;
	float max_y = rect.mCentre.y +  rect.mExtend.y;


	sInstance->AddScreenLine({ min_x, max_y }, { max_x, max_y }, color);
	sInstance->AddScreenLine({ min_x, max_y }, { min_x, min_y }, color);
	sInstance->AddScreenLine({ min_x, min_y }, { max_x, min_y }, color);
	sInstance->AddScreenLine({ max_x, max_y }, { max_x, min_y }, color);


}

void SimpleDraw::AddScreenCircle(const Math::Circle& circle, const Graphics::Color& color, const size_t& side )
{
	//const float deg_step = (0.5f * Math::Constants::Pi) / static_cast<float>(side);

	//float curr_deg = 0.0f;
	//

	//std::vector<Vector2> pts;
	//pts.reserve(4 * side);
	//for (size_t i = 0; i < 4 * side; i++)
	//{
	//	pts.push_back({ circle.mRadius * cosf(curr_deg), circle.mRadius * sinf(curr_deg) });
	//	curr_deg += deg_step;
	//}
	std::vector<Vector2> pts;
	ComputeCircleVertex(pts, circle.radius,side);

	for (size_t i = 0; i < pts.size() - 1; i++)
	{
		sInstance->AddScreenLine(pts[i] + circle.centre, pts[i+1] + circle.centre, color);
	}
	sInstance->AddScreenLine(pts[pts.size()-1] + circle.centre, pts[0] + circle.centre, color);



}

//void SimpleDraw::AddScreenGraph(int row, int column,const Math::Circle & leftTopCircle, const Graphics::Color & color, const size_t & side)
//{
//
//	std::vector<Vector2> pts;
//	ComputeCircleVertex(pts, leftTopCircle.mRadius, side);
//
//	Vector2 currentCenter = leftTopCircle.mCentre;

//}


void SimpleDraw::AddScreenCircle_For_AI_Graph(const std::vector<Math::Vector2>& cVerts, const Math::Circle & circle, const Graphics::Color & color, const size_t & side)
{
	for (size_t i = 0; i < cVerts.size() - 1; i++)
	{
		sInstance->AddScreenLine(cVerts[i] + circle.centre, cVerts[i + 1] + circle.centre, color);
	}
	sInstance->AddScreenLine(cVerts[cVerts.size() - 1] + circle.centre, cVerts[0] + circle.centre, color);


}

void SimpleDraw::ComputeCircleVertex(std::vector<Vector2>& cVerts, float radius, const size_t & side)
{
	const float deg_step = (0.5f * Math::Constants::Pi) / static_cast<float>(side);

	float curr_deg = 0.0f;

	std::vector<Vector2> pts;
	pts.reserve(4 * side);
	for (size_t i = 0; i < 4 * side; i++)
	{
		pts.push_back({ radius * cosf(curr_deg), radius * sinf(curr_deg) });
		curr_deg += deg_step;
	}

	cVerts = std::move(pts);
}

void SimpleDraw::Render(const Camera& cam)
{
	sInstance->Render(cam);
}

void SimpleDraw::Render2D()
{
	sInstance->Render2D();
}
