#ifndef HUGE_GRAPHICS_VERTEXTYPE_H
#define HUGE_GRAPHICS_VERTEXTYPE_H

#include "Common.h"
#include "Colors.h"

using namespace H::Math;

namespace H::Graphics {

	//vertx element
	constexpr uint32_t VE_Position		= 0x1 << 0;
	constexpr uint32_t VE_Normal		= 0x1 << 1;
	constexpr uint32_t VE_Tangent		= 0x1 << 2;
	constexpr uint32_t VE_Color			= 0x1 << 3;
	constexpr uint32_t VE_TexCoord		= 0x1 << 4;
	constexpr uint32_t VE_BlendIndex	= 0x1 << 5;
	constexpr uint32_t VE_BlendWeight	= 0x1 << 6;

#define VERTEX_FORMAT(fmt)\
	static constexpr uint32_t Format = fmt
	struct VertexPX
	{
		VERTEX_FORMAT(VE_Position | VE_TexCoord);
		Vector3 position;
		//float r, g, b, a;
		//Color color;
		Vector2 texCoord;
	};

	struct VertexPC
	{
		VERTEX_FORMAT( VE_Position | VE_Color);
		constexpr VertexPC() = default;
		constexpr VertexPC(const Math::Vector3& position, const Color& color) noexcept
			:position(position),color(color)
		{}
		Vector3 position;
		//float r, g, b, a;
		Color color;
	};
	struct VertexPN
	{
		VERTEX_FORMAT(VE_Position | VE_Normal);
		Vector3 position;
		Vector3 normal;
	};
	struct VertexPNC
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Color);
		constexpr VertexPNC() = default;
		constexpr VertexPNC(const Math::Vector3& position, const Math::Vector3& normal, const Color& color) noexcept
			:position(position),normal(normal),color(color)
		{}
		Vector3 position;
		Vector3 normal;
		Color color;
	};

	struct VertexPNX
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_TexCoord);
		Vector3 position;
		Vector3 normal;
		Vector2 texCoord;

		//float r, g, b, a;
		//Color color;
	};

	struct Vertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord);
		Vector3 position;
		Vector3 normal;
		Vector3 tangent;
		Vector2 texCoord;
 
	};

	struct BoneVertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord | VE_BlendIndex | VE_BlendWeight);
		Vector3 position;
		Vector3 normal;
		Vector3 tangent;
		Vector2 texCoord;
		int blendIndices[4] = { 0 };
		float blendWeights[4] = { 0.0f };

	};
 
}



#endif // !HUGE_GRAPHIC_VERTEXTYPE_H



