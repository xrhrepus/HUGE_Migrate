#ifndef HUGE_GRAPHIC_SIMPLEDRAW_H
#define HUGE_GRAPHIC_SIMPLEDRAW_H
#include "Colors.h"
#include "Camera.h"

//namespace H::Graphics{struct Color; }
//NOTE: c++17, TODO: fix all

namespace H::Graphics::SimpleDraw{

	void StaticInitialize(uint32_t maxVertCount);
	void StaticTerminate();

	//3d
	void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Graphics::Color& color);
	void AddBox(const Math::AABB& aabb, const Graphics::Color& color);
	void AddShpere(const Math::Sphere& sphere, const Graphics::Color& color, const size_t& side = 4);

	//3d solid
  
	void AddSolidBox(const Math::AABB& aabb, const Graphics::Color& color);

	//2d
	void AddScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Graphics::Color& color);
	void AddScreenRect(const Math::Rect& rect, const Graphics::Color& color);
	void AddScreenCircle(const Math::Circle& circle,const Graphics::Color& color,const size_t& side = 4);

	//2d solid

	//2d - ai
	//void AddScreenGraph(int row,int column, const Math::Circle& leftTopCircle, const Graphics::Color& color, const size_t& side = 4);
	void AddScreenCircle_For_AI_Graph(const std::vector<Math::Vector2>& cVerts, const Math::Circle& circle, const Graphics::Color& color, const size_t& side = 4);

	void ComputeCircleVertex(std::vector<Math::Vector2>& cVerts, float radius, const size_t& side = 4);

	void Render(const Camera& cam);
	void Render2D();
}
//namespace 
//huge :: graphics :: simpledraw



#endif // !HUGE_GRAPHIC_SIMPLEDRAW_H
