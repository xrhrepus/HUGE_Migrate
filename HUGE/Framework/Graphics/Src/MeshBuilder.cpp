#include "Precompiled.h"
#include "MeshBuilder.h"
 
using namespace H;
using namespace H::Graphics;

namespace
{
	Math::Vector2 convert_xyz_to_cube_uv(float x, float y, float z /*, int *index, float *u, float *v */)
	{
		float absX = fabs(x);
		float absY = fabs(y);
		float absZ = fabs(z);

		int isXPositive = x > 0 ? 1 : 0;
		int isYPositive = y > 0 ? 1 : 0;
		int isZPositive = z > 0 ? 1 : 0;

		float maxAxis, uc, vc;

		// POSITIVE X
		if (isXPositive && absX >= absY && absX >= absZ) {
			// u (0 to 1) goes from +z to -z
			// v (0 to 1) goes from -y to +y
			maxAxis = absX;
			uc = -z;
			vc = y;
			//*index = 0;
		}
		// NEGATIVE X
		if (!isXPositive && absX >= absY && absX >= absZ) {
			// u (0 to 1) goes from -z to +z
			// v (0 to 1) goes from -y to +y
			maxAxis = absX;
			uc = z;
			vc = y;
			//*index = 1;
		}
		// POSITIVE Y
		if (isYPositive && absY >= absX && absY >= absZ) {
			// u (0 to 1) goes from -x to +x
			// v (0 to 1) goes from +z to -z
			maxAxis = absY;
			uc = x;
			vc = -z;
			//*index = 2;
		}
		// NEGATIVE Y
		if (!isYPositive && absY >= absX && absY >= absZ) {
			// u (0 to 1) goes from -x to +x
			// v (0 to 1) goes from -z to +z
			maxAxis = absY;
			uc = x;
			vc = z;
			//*index = 3;
		}
		// POSITIVE Z
		if (isZPositive && absZ >= absX && absZ >= absY) {
			// u (0 to 1) goes from -x to +x
			// v (0 to 1) goes from -y to +y
			maxAxis = absZ;
			uc = x;
			vc = y;
			//*index = 4;
		}
		// NEGATIVE Z
		if (!isZPositive && absZ >= absX && absZ >= absY) {
			// u (0 to 1) goes from +x to -x
			// v (0 to 1) goes from -y to +y
			maxAxis = absZ;
			uc = -x;
			vc = y;
			//*index = 5;
		}

		// Convert range from -1 to 1 to 0 to 1
		//*u = 0.5f * (uc / maxAxis + 1.0f);
		//*v = 0.5f * (vc / maxAxis + 1.0f);
		return Vector2(0.5f * (uc / maxAxis + 1.0f), 0.5f * (vc / maxAxis + 1.0f));
	}
}

MeshPC MeshBuilder::CreateCubePC()
{
	constexpr VertexPC verts[] = 
	{
		//1
		{ {0.0f,0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f} },
		{ {0.5f,0.0f,0.0f}, {1.0f,0.0f,0.0f,1.0f} },
		{ {0.0f,-0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f} },

		{ {0.0f,-0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
		{ {0.0f, 0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },

		//2
		{ {0.0f,-0.5f,-0.5f},{0.0f,0.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,0.0f}, {0.0f,0.0f,1.0f,1.0f} },
		{ {0.5f,0.0f,0.0f},  {0.0f,0.0f,1.0f,1.0f} },

		{ {0.5f,0.0f,-0.5f}, {1.0f,0.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,-0.5f},{1.0f,0.0f,1.0f,1.0f} },
		{ {0.5f,0.0f,0.0f},  {1.0f,0.0f,1.0f,1.0f} },

		//3
		{ {-0.5f,0.0f,0.0f}, {0.0f,1.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,0.0f}, {0.0f,1.0f,1.0f,1.0f} },
		{ {0.0f,-0.5f,-0.5f},{0.0f,1.0f,1.0f,1.0f} },

		{ {-0.5f,0.0f,0.0f}, {1.0f,1.0f,0.0f,1.0f} },
		{ {0.0f,-0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },

		//4
		{ {-0.5f,0.0f,0.0f},{1.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,-0.5f},{1.0f,0.0f,0.0f,1.0f} },
		{ {0.0f,0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },

		{ {0.0f,0.5f,0.0f},{1.0f,1.0f,0.0f,1.0f} },
		{ {-0.5f,0.0f,-0.5f},{1.0f,0.0f,0.0f,1.0f} },
		{ {0.0f,0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },

		//5
		{ {0.5f,0.0f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
		{ {0.0f,0.5f,0.0f},{0.0f,1.0f,0.0f,1.0f} },
		{ {0.5f,0.0f,-0.5f},{0.0f,1.0f,0.0f,1.0f} },

		{ {0.0f,0.5f,0.0f},{ 1.0f,1.0f,0.0f,1.0f} },
		{ {0.0f,0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },
		{ {0.5f,0.0f,-0.5f},{1.0f,1.0f,0.0f,1.0f} },

		//6
				//6
				{ {0.0f,0.5f, -0.5f},{0.2f,0.0f,0.2f,1.0f} },
				{ {0.5f,0.0f, -0.5f},{0.2f,0.0f,0.2f,1.0f} },
				{ {0.0f,-0.5f,-0.5f},{0.2f,0.0f,0.2f,1.0f} },

				{ {0.0f,-0.5f,-0.5f},{0.2f,0.0f,0.2f,1.0f} },
				{ {-0.5f,0.0f,-0.5f},{0.2f,0.0f,0.2f,1.0f} },
				{ {0.0f, 0.5f,-0.5f},{0.2f,0.0f,0.2f,1.0f} }


	};

	constexpr uint32_t indices[] =
	{
		 1,2,0,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35
	};

	MeshPC mesh;
	
	mesh.vertices.insert(mesh.vertices.end(), std::begin(verts), std::end(verts));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh; 
	//NOTE0604: only one return  == std::move(r)     mesh&& funcname
	//copy elision
	//RETURN VALUE OPTIMIZATION
	//RVO
}

//	^ y
//	| /  z
//	|/
//	--------> x

MeshPC MeshBuilder::CreateQuadPC(int size, const Vector3& centre, const Color& color)
{
	const float unit = size * 1.0f;
	VertexPC verts[] =
	{
		{ { centre.x + unit, centre.y + unit, centre.z},color },
		{ { centre.x + unit, centre.y - unit, centre.z },color },
		{ { centre.x - unit, centre.y + unit, centre.z },color },
		{ { centre.x - unit, centre.y - unit, centre.z },color }
	};
	constexpr uint32_t indices[] =
	{
		0,2,1,2,3,1
	};
	MeshPC mesh;

	mesh.vertices.insert(mesh.vertices.end(), std::begin(verts), std::end(verts));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;
}

MeshPC MeshBuilder::CreatePlane(int size, int row, int column, const Vector3& centre, const Color& color)
{
		MeshPC mesh;
		const float unit = size * 1.0f;

		const float wid = unit * column * 0.5f;
		const float hei = unit * row * 0.5f;
		VertexPC verts[] =
		{
			{ { centre.x + wid, centre.y + hei, centre.z },color },
			{ { centre.x + wid, centre.y - hei, centre.z },color },
			{ { centre.x - wid, centre.y + hei, centre.z },color },
			{ { centre.x - wid, centre.y - hei, centre.z },color }

		};
		constexpr uint32_t indices[] =
		{
			0,2,1,2,3,1
		};
		mesh.vertices.insert(mesh.vertices.end(), std::begin(verts), std::end(verts));
		mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));

		return mesh;


		//const Vector3 top_left{ (row - 1) * size * 0.5 ,(column - 1) * size * 0.5,0.0f };
	//	const float top_left_x{ (row - 1) * size * 0.5f };
	//	const float top_left_y{ (column - 1) * size * 0.5f };

	//	const float unit = size * 1.0f;
	//	for (size_t i = 0; i < row; i++)
	//	{
	//		Vector3 pos{top_left_x,top_left_y + unit * i,0.0f};

	//		for (size_t j = 0; j < column; j++)
	//		{
	//			CreateQuad(size, pos, color);
	//			pos.x += unit;
	//		}
	//	}
	//}


}
MeshPC MeshBuilder::CreateCirclePC( float radius, int side, const Vector3& centre, const Color& color)
{
	if (side <= 0)
	{
		return MeshPC();
	}

	const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;
	float curr_deg = 0.0f;
	const int SIDE_TIMES_FOUR = side * 4;

	std::vector<VertexPC> res;

	res.reserve(SIDE_TIMES_FOUR + 1);

	//c-clockwise
	//g1
	for (int i = 0; i < SIDE_TIMES_FOUR+1; i++)
	{
		res.push_back({ { radius* cosf(curr_deg), radius * sinf(curr_deg), centre.z },color });
		curr_deg += deg_step;
	}
	res.push_back({ { centre.x, centre.y, centre.z },color }); // centre

	std::vector<uint32_t> indices;
	const size_t centre_index = res.size()-1;

	for (int i = 0; i < SIDE_TIMES_FOUR+1; i++)
	{
		indices.push_back(centre_index);
		indices.push_back(i );
		indices.push_back(i + 1);
	}
	//last one
	//indices.push_back(centre_index);
	//indices.push_back(centre_index-1);
	//indices.push_back(0);



	MeshPC mesh;

	mesh.vertices = std::move(res);
	mesh.indices = std::move(indices);

	return mesh;
}
MeshPC MeshBuilder::CreateCylinder(int height, float radius, int side, const Vector3& centre, const Color& color)
{
	MeshPC mesh;

	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;
		float curr_deg = 0.0f;
		const int SIDE_TIMES_FOUR = side * 4;

		std::vector<VertexPC> res;

		res.reserve((SIDE_TIMES_FOUR + 2) * 2 );

		//c-clockwise
		const float half_height = height / 2.0f;

		//top face
		for (int i = 0; i < SIDE_TIMES_FOUR + 1; i++)
		{
			res.push_back({ { radius* cosf(curr_deg), radius * sinf(curr_deg), centre.z + half_height },color });
			curr_deg += deg_step;
		}
		res.push_back({ { centre.x, centre.y, centre.z + half_height },color }); // top centre
		const size_t top_centre_index = res.size() - 1;

		//bot
		for (int i = 0; i < SIDE_TIMES_FOUR + 1; i++)
		{
			VertexPC vtx = res[i];
			vtx.position.z = centre.z - half_height;
			res.push_back(vtx);
			//curr_deg += deg_step;
		}
		res.push_back({ { centre.x, centre.y, centre.z - half_height },color }); // bot centre
		const size_t bot_start = top_centre_index + 1;


		//index
		std::vector<uint32_t> indices;
		const size_t bot_centre_index = top_centre_index + SIDE_TIMES_FOUR;
		//draw sides

		for (int i = 0; i < side * 4; i++)
		{
			indices.push_back(i);
			indices.push_back(bot_start + i + 1);
			indices.push_back(bot_start + i);
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(bot_start + i + 1);
		}
		////last one
		//indices.push_back(SIDE_TIMES_FOUR - 1);
		//indices.push_back(bot_start);
		//indices.push_back(bot_start + SIDE_TIMES_FOUR - 1);

		//indices.push_back(SIDE_TIMES_FOUR - 1);
		//indices.push_back(indices[0]);
		//indices.push_back(bot_start);
		//



		//draw circle
		//top
		for (int i = 0; i < SIDE_TIMES_FOUR+1; i++)
		{
			indices.push_back(top_centre_index);
			indices.push_back(i + 1);
			indices.push_back(i);
		}
		////last one
		//indices.push_back(top_centre_index);
		//indices.push_back(0);
		//indices.push_back(top_centre_index - 1);


		//bot
		for (int i = 0; i < SIDE_TIMES_FOUR+1; i++)
		{
			indices.push_back(bot_centre_index);
			indices.push_back(bot_start + i);
			indices.push_back(bot_start + i + 1);
		}
		//indices.push_back(bot_centre_index);
		//indices.push_back(bot_centre_index - 1);
		//indices.push_back(bot_start);






		mesh.vertices = std::move(res);
		mesh.indices = std::move(indices);
	}
	return mesh;
}

MeshPC MeshBuilder::CreateCone(int height, float radius, int side, const Vector3& centre, const Color& color)
{
	MeshPC mesh;
	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;
		float curr_deg = 0.0f;
		const int SIDE_TIMES_FOUR = side * 4;

		std::vector<VertexPC> res;

		res.reserve(SIDE_TIMES_FOUR + 1);

		//c-clockwise
		const float half_height = height / 2.0f;

		//bot face
		for (int i = 0; i < SIDE_TIMES_FOUR; i++)
		{
			res.push_back({ { radius* cosf(curr_deg), radius * sinf(curr_deg), centre.z - half_height },color });
			curr_deg += deg_step;
		}
		res.push_back({ { centre.x, centre.y, centre.z - half_height },color }); // bot centre
		const size_t top_centre_index = res.size() - 1;

		// tip

		res.push_back({ { centre.x, centre.y, centre.z + half_height },color }); // top centre


		//index
		std::vector<uint32_t> indices;
		const size_t tip_index = res.size() -1;

		for (int i = 0; i < side * 4 - 1; i++)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(tip_index);
		}
		////last one
		indices.push_back(top_centre_index - 1);
		indices.push_back(0);
		indices.push_back(tip_index);

		//bot circle
		for (int i = 0; i < SIDE_TIMES_FOUR; i++)
		{
			indices.push_back(top_centre_index);
			indices.push_back(i + 1);
			indices.push_back(i);
		}
		////last one
		indices.push_back(top_centre_index);
		indices.push_back(0);
		indices.push_back(top_centre_index - 1);




		mesh.vertices = std::move(res);
		mesh.indices = std::move(indices);


	}


	return mesh;

}

MeshPC MeshBuilder::CreateShperePC(float radius, int side, const Vector3& centre, const Color& color)
{
	MeshPC mesh;

	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;
		//const float radius_step = radius / static_cast<float>(side);
		const int layers = (side * 2 - 1);
		const float z_step = 180.0f / (layers + 1) * Math::Constants::DegToRad;

		const int vtx_in_circle = side * 4 + 1; // num of verts in one circle/layer
		const int total_amount_of_vertex = 8 * side*side - 2 * side + 1;

		
		float curr_deg = 0.0f;
		float curr_radius = radius;
		float curr_z = radius;
		float curr_z_deg = 0.0f;

		//start from center
		std::vector<VertexPC> vtx;
		vtx.reserve(total_amount_of_vertex);
		//top
		vtx.push_back({ { centre.x ,centre.y,curr_z },color });

		//
		for (int i = 0; i < layers; i++) // from  + 1/2 z to - 1/2 z
		{
			curr_z_deg += deg_step;
			curr_z = radius * cosf(curr_z_deg);
			curr_radius = abs( radius * (  sinf(curr_z_deg)));
			curr_deg = 0.0f;

			for (int j = 0; j < vtx_in_circle; j++)
			{
				vtx.push_back({ { curr_radius* cosf(curr_deg), curr_radius * sinf(curr_deg), curr_z },color });
				curr_deg += deg_step;
			}
		}
		//bot
		vtx.push_back({ { centre.x ,centre.y, -radius },color });

		//index
		std::vector<uint32_t> indices;
		//top first
		for (int i = 1; i < vtx_in_circle ; i++)
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		//loop
		int start_index = 1;
		for (int i = 0; i < layers - 1; i++)
		{

			for (int j = start_index; j < start_index + vtx_in_circle - 1; j++)
			{
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j + 1);
			}
			start_index += vtx_in_circle;
		}
		//bot
		const int bot_circle_index = vtx.size() - 1 - vtx_in_circle;
		const int bot_index = vtx.size() - 1;

		for (int i = 0; i < vtx_in_circle; i++)
		{
			indices.push_back(i + bot_circle_index);
			indices.push_back(bot_index);
			indices.push_back(i + bot_circle_index + 1);
		}
		mesh.vertices = std::move(vtx);
		mesh.indices = std::move(indices);
	}


	return mesh;

}
 

MeshPX MeshBuilder::CreateQuadPX()
{
	constexpr VertexPX verts[] =
	{
		{{-1.0f,1.0f,0.0f},{0.0f,0.0f} },
		{{1.0f,1.0f,0.0f},{1.0f,0.0f} },
		{{1.0f,-1.0f,0.0f},{1.0f,1.0f} },
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f} }
	};

	constexpr uint32_t indices[] =
	{
		0,1,2,0,2,3
	};
	MeshPX mesh;

	mesh.vertices.insert(mesh.vertices.end(), std::begin(verts), std::end(verts));
	mesh.indices.insert(mesh.indices.end(), std::begin(indices), std::end(indices));
	return mesh;


}

MeshPX MeshBuilder::CreateNDCQuadPX()
{
	MeshPX mesh;
	//in NDC
	//-1 +1(0 0) --------- 0 -------- +1 +1(1 1)


	//-1 -1(0 1) --------- 0 -------- +1 -1(1 1)

	mesh.vertices.emplace_back(VertexPX{ { -1.0f,-1.0f,0.0f }, { 0.0f,1.0f } });
	mesh.vertices.emplace_back(VertexPX{ { -1.0f, 1.0f,0.0f }, { 0.0f,0.0f } });
	mesh.vertices.emplace_back(VertexPX{ {  1.0f, 1.0f,0.0f }, { 1.0f,0.0f } });
	mesh.vertices.emplace_back(VertexPX{ {  1.0f,-1.0f,0.0f }, { 1.0f,1.0f } });

	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(1);
	mesh.indices.emplace_back(2);

	mesh.indices.emplace_back(0);
	mesh.indices.emplace_back(2);
	mesh.indices.emplace_back(3);

	return mesh;
}

//todo
MeshPX MeshBuilder::CreateShperePX(float radius, int side, const Vector3& centre)
{
	MeshPX mesh;

	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;
		
		const int layers = (side * 2 + 1);//include top n bot

		const float z_step = 180.0f / (layers + 1) * Math::Constants::DegToRad;

		const int vtx_in_circle = side * 4 + 1; // num of verts in one circle/layer

		const int total_amount_of_vertex = layers * vtx_in_circle;
		//tex coor
		const float tex_step_y = 1.0f / (static_cast<float>(side)  * 2.0f);
		const float tex_step_x = 1.0f / static_cast<float>(vtx_in_circle - 1);

		float curr_deg = 0.0f;
		float curr_radius = radius;
		float curr_z = radius;
		float curr_z_deg = 0.0f;
		float curr_tex_x = 0.0f;
		float curr_tex_y = 0.0f;


		std::vector<VertexPX> vtx;
		vtx.reserve(total_amount_of_vertex);

		//
		for (int i = 0; i < layers; i++) // from  + 1/2 z to - 1/2 z
		{
			curr_z = radius * cosf(curr_z_deg);
			curr_radius = abs(radius * (sinf(curr_z_deg)));
			curr_deg = 0.0f;
			curr_tex_x = 0.0f;
			for (int j = 0; j < vtx_in_circle; j++)
			{
				vtx.push_back({ { curr_radius* cosf(curr_deg), curr_radius * sinf(curr_deg), curr_z },{curr_tex_x,curr_tex_y} });
				curr_deg += deg_step;
				curr_tex_x += tex_step_x;
			}
			curr_z_deg += deg_step;
			curr_tex_y += tex_step_y;

		}

		//index
		std::vector<uint32_t> indices;
		//loop
		int start_index = 0;
		for (int i = 0; i < layers - 1; i++)
		{

			for (int j = start_index; j < start_index + vtx_in_circle - 1; j++)
			{
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j + 1);
			}
			start_index += vtx_in_circle;
		}

		mesh.vertices = std::move(vtx);
		mesh.indices = std::move(indices);
	}


	return mesh;

}


MeshPX MeshBuilder::CreateSkyBoxPX(float radius, int side, const Vector3& centre)
{
	MeshPX mesh;

	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;

		const int layers = (side * 2 + 1);//include top n bot

		const float z_step = 180.0f / (layers + 1) * Math::Constants::DegToRad;

		const int vtx_in_circle = side * 4 + 1; // num of verts in one circle/layer

		const int total_amount_of_vertex = layers * vtx_in_circle;
		//tex coor
		const float tex_step_y = 1.0f / (static_cast<float>(side)  * 2.0f);
		const float tex_step_x = 1.0f / static_cast<float>(vtx_in_circle - 1);

		float curr_deg = 0.0f;
		float curr_radius = radius;
		float curr_z = radius;
		float curr_z_deg = 0.0f;
		float curr_tex_x = 0.0f;
		float curr_tex_y = 0.0f;


		std::vector<VertexPX> vtx;
		vtx.reserve(total_amount_of_vertex);

		//
		for (int i = 0; i < layers; i++) // from  + 1/2 z to - 1/2 z
		{
			curr_z = radius * cosf(curr_z_deg);
			curr_radius = abs(radius * (sinf(curr_z_deg)));
			curr_deg = 0.0f;
			curr_tex_x = 0.0f;
			for (int j = 0; j < vtx_in_circle; j++)
			{
				vtx.push_back({ { curr_radius* cosf(curr_deg), curr_radius * sinf(curr_deg), curr_z },{curr_tex_x,curr_tex_y} });
				curr_deg += deg_step;
				curr_tex_x += tex_step_x;
			}
			curr_z_deg += deg_step;
			curr_tex_y += tex_step_y;

		}

		//index
		std::vector<uint32_t> indices;
		//loop
		int start_index = 0;
		for (int i = 0; i < layers - 1; i++)
		{

			for (int j = start_index; j < start_index + vtx_in_circle - 1; j++)
			{
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j + vtx_in_circle);
				indices.push_back(j);
				indices.push_back(j + 1);
				indices.push_back(j + vtx_in_circle + 1);
			}
			start_index += vtx_in_circle;
		}

		mesh.vertices = std::move(vtx);
		mesh.indices = std::move(indices);
	}


	return mesh;

}

MeshPN H::Graphics::MeshBuilder::CreateShperePN(float radius, int side, const Vector3 & centre)
{
	MeshPN mesh;

	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;

		const int layers = (side * 2 + 1);//include top n bot

		const float z_step = 180.0f / (layers + 1) * Math::Constants::DegToRad;

		const int vtx_in_circle = side * 4 + 1; // num of verts in one circle/layer

		const int total_amount_of_vertex = layers * vtx_in_circle;
		//tex coor
		const float tex_step_y = 1.0f / (static_cast<float>(side)  * 2.0f);
		const float tex_step_x = 1.0f / static_cast<float>(vtx_in_circle - 1);

		float curr_deg = 0.0f;
		float curr_radius = radius;
		float curr_z = radius;
		float curr_z_deg = 0.0f;
		float curr_tex_x = 0.0f;
		float curr_tex_y = 0.0f;


		std::vector<VertexPN> vtx;
		vtx.reserve(total_amount_of_vertex);

		//
		for (int i = 0; i < layers; i++) // from  + 1/2 z to - 1/2 z
		{
			curr_z = radius * cosf(curr_z_deg);
			curr_radius = abs(radius * (sinf(curr_z_deg)));
			curr_deg = 0.0f;
			curr_tex_x = 0.0f;
			for (int j = 0; j < vtx_in_circle; j++)
			{
				VertexPN pn = { 
					Vector3({ curr_radius* cosf(curr_deg) + centre.x, curr_radius * sinf(curr_deg) + centre.y, curr_z + centre.z}),
					Vector3({ H::Math::Normalize({ curr_radius* cosf(curr_deg) + centre.x, curr_radius * sinf(curr_deg) + centre.y, curr_z + centre.z})})
				};
				vtx.push_back(std::move(pn));
				//vtx.push_back(
				//	
				//	{ H::Math::Normalize( { curr_radius* cosf(curr_deg), curr_radius * sinf(curr_deg), curr_z })}
				//);
				curr_deg += deg_step;
				curr_tex_x += tex_step_x;
			}
			curr_z_deg += deg_step;
			curr_tex_y += tex_step_y;

		}

		//index
		std::vector<uint32_t> indices;
		//loop
		int start_index = 0;
		for (int i = 0; i < layers - 1; i++)
		{

			for (int j = start_index; j < start_index + vtx_in_circle - 1; j++)
			{
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j + 1);
			}
			start_index += vtx_in_circle;
		}

		mesh.vertices = std::move(vtx);
		mesh.indices = std::move(indices);
	}


	return mesh;


	
}
MeshPN H::Graphics::MeshBuilder::CreateCubePN(float xExtend, float yExtend, float zExtend, const Vector3 & centre)
{
	MeshPN cube;
	auto& vtx = cube.vertices;
	vtx.resize(8);

	//Vector3 ntpos = H::Math::Normalize(tpos);
	Vector3 tpos = centre + Vector3{ xExtend,yExtend,-zExtend } ;
	vtx[0] = { tpos, H::Math::Normalize(tpos) };

	tpos = centre + Vector3{ xExtend,yExtend,zExtend };
	vtx[1] = { tpos, H::Math::Normalize(tpos) };

	tpos = centre + Vector3{ -xExtend,yExtend,zExtend };
	vtx[2] = { tpos, H::Math::Normalize(tpos) };

	tpos = centre + Vector3{ -xExtend,yExtend,-zExtend };
	vtx[3] = { tpos, H::Math::Normalize(tpos) };
	//
	tpos = centre + Vector3{ xExtend,-yExtend,-zExtend };
	vtx[4] = { tpos, H::Math::Normalize(tpos) };

	tpos = centre + Vector3{ xExtend,-yExtend,zExtend };
	vtx[5] = { tpos, H::Math::Normalize(tpos) };

	tpos = centre + Vector3{ -xExtend,-yExtend,zExtend };
	vtx[6] = { tpos, H::Math::Normalize(tpos) };

	tpos = centre + Vector3{ -xExtend,-yExtend,-zExtend };
	vtx[7] = { tpos, H::Math::Normalize(tpos) };

	//
	auto& idx = cube.indices;
	constexpr uint32_t indices[] =
	{
		//front
		4,0,1,
		4,1,5,
		//back
		6,2,3,
		6,3,7,
		//top
		0,3,2,
		0,2,1,
		//bot
		5,6,7,
		5,7,4,
		//left
		7,3,0,
		7,0,4,
		//right
		5,1,2,
		5,2,6
	};
	idx.insert(idx.end(), std::begin(indices), std::end(indices));
	return cube;

}
Mesh H::Graphics::MeshBuilder::CreateShpere(float radius, int side, const Vector3& centre)
{
	Mesh mesh;

	if (side > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(side)) * Math::Constants::DegToRad;

		const int layers = (side * 2 + 1);//include top n bot

		const float z_step = 180.0f / (layers + 1) * Math::Constants::DegToRad;

		const int vtx_in_circle = side * 4 + 1; // num of verts in one circle/layer

		const int total_amount_of_vertex = layers * vtx_in_circle;
		//tex coor
		const float tex_step_y = 1.0f / (static_cast<float>(side)  * 2.0f);
		const float tex_step_x = 1.0f / static_cast<float>(vtx_in_circle - 1);

		float curr_deg = 0.0f;
		float curr_radius = radius;
		float curr_z = radius;
		float curr_z_deg = 0.0f;
		float curr_tex_x = 0.0f;
		float curr_tex_y = 0.0f;


		std::vector<Vertex> vtx;
		vtx.reserve(total_amount_of_vertex);

		//
		for (int i = 0; i < layers; i++) // from  + 1/2 z to - 1/2 z
		{
			curr_z = radius * cosf(curr_z_deg);
			curr_radius = abs(radius * (sinf(curr_z_deg)));
			curr_deg = 0.0f;
			curr_tex_x = 0.0f;


			for (int j = 0; j < vtx_in_circle; j++)
			{
				Vector3 vPos = { curr_radius* cosf(curr_deg) + centre.x, curr_radius * sinf(curr_deg) + centre.y, curr_z + centre.z};
				Vector3 tangent = { -vPos.z,0.0f,vPos.x };//?

				vtx.push_back(
					{ vPos,
					H::Math::Normalize(vPos),
					/*{0.0f,0.0f,0.0f}*/tangent,
					{curr_tex_x,curr_tex_y} });
				curr_deg += deg_step;
				curr_tex_x += tex_step_x;
			}
			curr_z_deg += deg_step;
			curr_tex_y += tex_step_y;

		}

		//index
		std::vector<uint32_t> indices;
		//loop
		int start_index = 0;
		for (int i = 0; i < layers - 1; i++)
		{

			for (int j = start_index; j < start_index + vtx_in_circle - 1; j++)
			{
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j);
				indices.push_back(j + vtx_in_circle + 1);
				indices.push_back(j + 1);
			}
			start_index += vtx_in_circle;
		}

		mesh.vertices = std::move(vtx);
		mesh.indices = std::move(indices);
	}


	return mesh;



}
Mesh H::Graphics::MeshBuilder::CreateQuad(float xExtend, float yExtend, const Vector3 & centre)
{
	Mesh quad;
	const Vector3 normal = Vector3::zAxis();
	const Vector3 tangent = Vector3::Zero();

 	Vertex verts[] =
	{
		{ { centre.x + xExtend, centre.y + yExtend, centre.z} ,normal,tangent,{1.0f,0.0f} },
		{ { centre.x + xExtend, centre.y - yExtend, centre.z },normal,tangent,{1.0f,1.0f} },
		{ { centre.x - xExtend, centre.y + yExtend, centre.z },normal,tangent,{0.0f,0.0f} },
		{ { centre.x - xExtend, centre.y - yExtend, centre.z },normal,tangent,{0.0f,1.0f} }
	};
	constexpr uint32_t indices[] =
	{
		0,2,1,2,3,1
	};
	quad.vertices.insert(quad.vertices.end(), std::begin(verts), std::end(verts));
	quad.indices.insert(quad.indices.end(), std::begin(indices), std::end(indices));


	return quad;
}
Mesh H::Graphics::MeshBuilder::CreateTerrain(uint32_t numRow, uint32_t numCol, float sizeOfOneBlock, const Vector3 & startPos)
{
	Mesh terrain;
	//
	auto& vtx = terrain.vertices;
	const uint32_t totalVtxNum = (numRow + 1)* (numCol + 1);
	vtx.reserve(totalVtxNum);
	auto& idx = terrain.indices;
	idx.reserve(6 * numRow*numCol);
	//
	const float startY = startPos.y;
	const Vector3 normal(0.0f, 1.0f, 0.0f);
	const Vector3 tangent(1.0f, 0.0f, 0.0f);
	//
	const float texCoordXstep = 1.0f / (float)(numCol);
	const float texCoordYstep = 1.0f / (float)(numRow);

	for (size_t i = 0; i <= numRow; ++i)
	{
		float currX = startPos.x;
		float currZ = startPos.z + sizeOfOneBlock * i;

		float texCurrX = 0.0f;
		float texCurrY = texCoordYstep * i;
 
		for (size_t j = 0; j <= numCol; ++j)
		{
			Vertex v1;
			v1.position = { currX,startY,currZ };
			v1.normal = normal;
			v1.tangent = tangent;
			v1.texCoord = { texCurrX,texCurrY };
			vtx.push_back(v1);

			currX += sizeOfOneBlock;
			texCurrX += texCoordXstep;

		}
	}
	//
 	for (size_t i = 0; i < numRow; ++i)
	{
		const size_t ixcol = i * (numCol+1);
		const size_t iplus1xcol = (i+1) * (numCol+1);

		for (size_t j = 0; j < numCol; ++j)
		{

			idx.push_back(j + ixcol);
			idx.push_back(j + iplus1xcol);
			idx.push_back(j + ixcol + 1);
 
			idx.push_back(j + iplus1xcol);
			idx.push_back(j + iplus1xcol + 1);
			idx.push_back(j + ixcol + 1);
		}
	}

	return terrain;
}
Mesh H::Graphics::MeshBuilder::CreateCube(float xExtend, float yExtend, float zExtend, const Vector3& centre)
{
	Mesh cube;
	auto& vtx = cube.vertices;
	vtx.resize(24);
	//6 faces
	//z+
	Vector3 tangent = { 1.0f,0.0f,0.0f };
	Vector3 normal = { 0.0f,0.0f,1.0f };
	Vector3 pos = centre + Vector3{ xExtend,yExtend,zExtend };
	vtx[0] = { pos, normal,tangent,{0.0f,0.0f} };
	pos = centre + Vector3{ -xExtend,yExtend,zExtend };
	vtx[1] = { pos, normal,tangent,{1.0f,0.0f} };
	pos = centre + Vector3{ -xExtend,-yExtend,zExtend };
	vtx[2] = { pos, normal,tangent,{1.0f,1.0f} };
	pos = centre + Vector3{ xExtend,-yExtend,zExtend };
	vtx[3] = { pos, normal,tangent,{0.0f,1.0f} };
	//z-
	normal = { 0.0f,0.0f,-1.0f };
	pos = centre + Vector3{ -xExtend,yExtend,-zExtend };
	vtx[4] = { pos, normal,tangent,{0.0f,0.0f} };
	pos = centre + Vector3{ xExtend,yExtend,-zExtend };
	vtx[5] = { pos, normal,tangent,{1.0f,0.0f} };
	pos = centre + Vector3{ xExtend,-yExtend,-zExtend };
	vtx[6] = { pos, normal,tangent,{1.0f,1.0f} };
	pos = centre + Vector3{ -xExtend,-yExtend,-zExtend };
	vtx[7] = { pos, normal,tangent,{0.0f,1.0f} };

	//x+
	normal = { 1.0f,0.0f,0.0f };
	tangent = { 0.0f,0.0f,1.0f };
	pos = centre + Vector3{ xExtend,yExtend,-zExtend };
	vtx[8] = { pos, normal,tangent,{0.0f,0.0f} };
	pos = centre + Vector3{ xExtend,yExtend,zExtend };
	vtx[9] = { pos, normal,tangent,{1.0f,0.0f} };
	pos = centre + Vector3{ xExtend,-yExtend,zExtend };
	vtx[10] = { pos, normal,tangent,{1.0f,1.0f} };
	pos = centre + Vector3{ xExtend,-yExtend,-zExtend };
	vtx[11] = { pos, normal,tangent,{0.0f,1.0f} };
	//x-
	normal = { -1.0f,0.0f,0.0f };
	pos = centre + Vector3{ -xExtend,yExtend,zExtend };
	vtx[12] = { pos, normal,tangent,{0.0f,0.0f} };
	pos = centre + Vector3{ -xExtend,yExtend,-zExtend };
	vtx[13] = { pos, normal,tangent,{1.0f,0.0f} };
	pos = centre + Vector3{ -xExtend,-yExtend,-zExtend };
	vtx[14] = { pos, normal,tangent,{1.0f,1.0f} };
	pos = centre + Vector3{ -xExtend,-yExtend,zExtend };
	vtx[15] = { pos, normal,tangent,{0.0f,1.0f} };

	//y+
	normal = { 0.0f,1.0f,0.0f };
	tangent = { 1.0f,0.0f,0.0f };
	pos = centre + Vector3{ -xExtend,yExtend,-zExtend };
	vtx[16] = { pos, normal,tangent,{0.0f,0.0f} };
	pos = centre + Vector3{ -xExtend,yExtend,zExtend };
	vtx[17] = { pos, normal,tangent,{1.0f,0.0f} };
	pos = centre + Vector3{ xExtend,yExtend,zExtend };
	vtx[18] = { pos, normal,tangent,{1.0f,1.0f} };
	pos = centre + Vector3{ xExtend,yExtend,-zExtend };
	vtx[19] = { pos, normal,tangent,{0.0f,1.0f} };
	//y-
	normal = { 0.0f,-1.0f,0.0f };
	pos = centre + Vector3{ xExtend,-yExtend,-zExtend };
	vtx[20] = { pos, normal,tangent,{0.0f,0.0f} };
	pos = centre + Vector3{ xExtend,-yExtend,zExtend };
	vtx[21] = { pos, normal,tangent,{1.0f,0.0f} };
	pos = centre + Vector3{ -xExtend,-yExtend,zExtend };
	vtx[22] = { pos, normal,tangent,{1.0f,1.0f} };
	pos = centre + Vector3{ -xExtend,-yExtend,-zExtend };
	vtx[23] = { pos, normal,tangent,{0.0f,1.0f} };

	auto& idx = cube.indices;
	constexpr uint32_t indices[] =
	{
		//z+
		0,1,2,
		0,2,3,
		//z-
		4,5,7,
		5,6,7,
		//x+
		8,9,10,
		8,10,11,
		//x-
		12,13,14,
		12,14,15,
		//y+
		16,17,18,
		16,18,19,
		//y-
		20,21,22,
		20,22,23
	};
	idx.insert(idx.end(), std::begin(indices), std::end(indices));

	//auto& vtx = cube.vertices;
	//vtx.resize(8);

	////Vector3 ntpos = H::Math::Normalize(tpos);
	//Vector3 tpos = centre + Vector3{ xExtend,yExtend,-zExtend };
	//Vector3 tangent = { -tpos.z,0.0f,tpos.x };//?
	//vtx[0] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	//tpos = centre + Vector3{ xExtend,yExtend,zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[1] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	//tpos = centre + Vector3{ -xExtend,yExtend,zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[2] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	//tpos = centre + Vector3{ -xExtend,yExtend,-zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[3] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };
	////
	//tpos = centre + Vector3{ xExtend,-yExtend,-zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[4] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	//tpos = centre + Vector3{ xExtend,-yExtend,zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[5] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	//tpos = centre + Vector3{ -xExtend,-yExtend,zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[6] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	//tpos = centre + Vector3{ -xExtend,-yExtend,-zExtend };
	//tangent = { -tpos.z,0.0f,tpos.x };
	//vtx[7] = { tpos, H::Math::Normalize(tpos),tangent,convert_xyz_to_cube_uv(tpos.x,tpos.y,tpos.z) };

	////
	//auto& idx = cube.indices;
	//constexpr uint32_t indices[] =
	//{
	//	//front
	//	4,0,1,
	//	4,1,5,
	//	//back
	//	6,2,3,
	//	6,3,7,
	//	//top
	//	0,3,2,
	//	0,2,1,
	//	//bot
	//	5,6,7,
	//	5,7,4,
	//	//left
	//	7,3,0,
	//	7,0,4,
	//	//right
	//	5,1,2,
	//	5,2,6
	//};
	//idx.insert(idx.end(), std::begin(indices), std::end(indices));
	return cube;
}

Math::AABB H::Graphics::MeshBuilder::ComputeBound(const Mesh& mesh)
{
	if (mesh.vertices.empty())
	{
		return Math::AABB();
	}

	Math::Vector3 min = mesh.vertices[0].position;
	Math::Vector3 max = mesh.vertices[0].position;

	for (auto& v : mesh.vertices)
	{
		min.x = Math::Minf(min.x, v.position.x);
		min.y = Math::Minf(min.y, v.position.y);
		min.z = Math::Minf(min.z, v.position.z);

		max.x = Math::Maxf(max.x, v.position.x);
		max.y = Math::Maxf(max.y, v.position.y);
		max.z = Math::Maxf(max.z, v.position.z);

	}

	return Math::AABB::FromMinMax(min, max);
}


BoneMesh H::Graphics::MeshBuilder::CreateTentacle(size_t numbones, float height, float radius, int slice, const Vector3 & centre)
{
	BoneMesh mesh;

	numbones = 3;//

	if (slice > 0 && numbones > 0)
	{
		const float deg_step = (90.0f / static_cast<float>(slice)) * Math::Constants::DegToRad;
		float curr_deg = 0.0f;
		const int VTX_IN_CIRCLE = slice * 4 + 1;
		const int totalNumVtx = (VTX_IN_CIRCLE) * (numbones + 1) + 2;
		mesh.vertices.reserve(totalNumVtx);


 
		//side
		Vector3 circleCentre{ 0.0f };
		float height_step = height / static_cast<float>(numbones);
		float v_step = 1.0f / static_cast<float>(numbones);
		float u_step = 1.0f / static_cast<float>(VTX_IN_CIRCLE);

		const float blend_step = 1.0f / height_step;

		float currU = 0.0f,currV = 0.0f;
		//#0
		for (int j = 0; j < numbones + 1; j++)
		{
			float blendWght[4] = { 0.0f };
			if (j - 1 >= 0 && j - 1 <= numbones)
				blendWght[j - 1] = 1.0f;

			if (j >= 0 && j <= numbones)
				blendWght[j] = 1.0f;

			//if (j + 1 >= 0 && j + 1 <= numbones)
			//	blendWght[j + 1] = 1.0f;

			for (int i = 0; i < VTX_IN_CIRCLE ; i++)
			{

				Vector3 pos = { radius* cosf(curr_deg), radius * sinf(curr_deg), centre.z + height_step * j };
				Vector3 normal = H::Math::Normalize(pos - Vector3{ centre.x,centre.y,pos.z });

				mesh.vertices.push_back(
					{ pos,
					normal,
					normal.Cross({0.0f,0.0f,1.0f}),
					{currU,currV},
					{0,1,2,3},
					{blendWght[0],blendWght[1],blendWght[2],blendWght[3]}
					});
				curr_deg += deg_step;
				currU += u_step;
			}
			currV += v_step;
			curr_deg = 0.0f;
			currU = 0.0f;
		}
		// bot
		mesh.vertices.push_back({ centre,
					{0.0f,0.0f,1.0f},
					{0.0f,-1.0f,0.0f},
					{0.0f,0.0f},
					{0,1,2,3},
					{1.0f,0.0f,0.0f,0.0f}
			});
		//top
		mesh.vertices.push_back({ {centre.x,centre.y,centre.z + height },
			{0.0f,0.0f,1.0f},
			{0.0f,1.0f,0.0f},
			{1.0f,1.0f},
			{0,1,2,3},
			{0.0f,0.0f,1.0f,0.0f}
			});



		//times = numbone
		mesh.indices.reserve((6 * VTX_IN_CIRCLE) * numbones);

		for (int i = 0; i < numbones; i++)
		{
			int idx = (VTX_IN_CIRCLE * i);

			for (int j = 0; j < VTX_IN_CIRCLE - 1 ; j++)//?
			{

				mesh.indices.push_back(idx + j );
				mesh.indices.push_back(idx + j + VTX_IN_CIRCLE + 1);
				mesh.indices.push_back((idx + j + VTX_IN_CIRCLE) );

				mesh.indices.push_back(idx + j);
				mesh.indices.push_back((idx + j + 1) );
				mesh.indices.push_back(idx + j + VTX_IN_CIRCLE + 1);
			}
		}

		//top/bot
		const int botIndex = mesh.vertices.size() - 2;
		const int topIndex = mesh.vertices.size() - 1;
		const int topStartIndex = mesh.vertices.size() - 3 - VTX_IN_CIRCLE + 1;


		for (int i = 0; i < VTX_IN_CIRCLE - 1; i++)
		{
			//bot
			mesh.indices.push_back(i);
			mesh.indices.push_back(botIndex);
			mesh.indices.push_back((i + 1)%VTX_IN_CIRCLE);
			//top
			mesh.indices.push_back((topStartIndex + i + 1));
			mesh.indices.push_back(topIndex);
			mesh.indices.push_back(topStartIndex + i);
		}
		 


	}

	return mesh;
}

BoneMesh H::Graphics::MeshBuilder::CreateFourBoneTentacle(float height, float radius, int slice, const Vector3 & centre)
{
	BoneMesh mesh;

	const int numbones = 4;

	if (slice > 0 && numbones > 1 && numbones <= 4)
	{
		const int vert_slice = 12;

		const float deg_step = (90.0f / static_cast<float>(slice)) * Math::Constants::DegToRad;
 		const int VTX_IN_CIRCLE = slice * 4 + 1;
		const int totalNumVtx = (VTX_IN_CIRCLE) * (vert_slice) + 2;

		mesh.vertices.reserve(totalNumVtx);


		float curr_deg = 0.0f;

		//side
		Vector3 circleCentre{ 0.0f };
		const float height_step = height / static_cast<float>(vert_slice);
		const float v_step = 1.0f / static_cast<float>(vert_slice);
		const float u_step = 1.0f / static_cast<float>(VTX_IN_CIRCLE);

 
		float currU = 0.0f, currV = 0.0f;


		//blend sections
		const float sec_0 = (height / (numbones + 4));//[0] 1.0
 		const float sec_1 = sec_0 * 2;//[1] 1.0
 		const float sec_2 = sec_0 * 3;//[2] 1.0
 		const float sec_3 = sec_0 * 4;//[3] 1.0
		const float sec_4 = sec_0 * 5;//[3] 1.0
		const float sec_5 = sec_0 * 6;//[3] 1.0
		const float sec_6 = sec_0 * 7;//[3] 1.0
 		

 
		//blend weight step
		const int blendwght_numSlice = (sec_0) / height_step;//how many slices between 
		const float blend_step = 0.5f / blendwght_numSlice;

		int count_0_1 = 0;//[0] 0.5 ~ 0

		int count_1_0 = 0;//[1] 0.0 ~ 1
		int count_1_1 = 0;//[1] 1 ~ 0.0

		int count_2_0 = 0;//[2] 0  ~ 1
		int count_2_1 = 0;//[2] 1 ~ 0.0

		int count_3_0 = 0;//[3] 0.0 ~ 0.5

 
 		//#0
		for (int j = 0; j <= vert_slice; j++)
		{
			int blendIndx[4] = { 0 };
			float blendWght[4] = { 0.0f };
			const float currHeight = (centre.z + height_step * j);

			if (currHeight <= sec_0)//[0] 1
			{
				blendIndx[0] = 0;
				blendWght[0] = 1.0f;
			}
			else if (currHeight > sec_0 && currHeight <= sec_1)//[0] 1 - 0.5 , [1] 0 - 0.5 
			{
				const float stp0 = blend_step * count_0_1++;
				blendIndx[0] = 0;
				blendWght[0] = 1.0f - stp0;

				const float stp1 = blend_step * count_1_0++;

				blendIndx[1] = 1;
				blendWght[1] = 0.0f + stp1;
			}
			else if (currHeight > sec_1 && currHeight <= sec_2)//[0] 0.5 - 0.0 ,[1] 0.5 - 1.0
			{
				const float stp0 = blend_step * count_0_1++;
				blendIndx[0] = 0;
				blendWght[0] = 1.0f - stp0;

				const float stp1 = blend_step * count_1_0++;
				blendIndx[1] = 1;
				blendWght[1] = 0.0f + stp1;


 			}
			else if (currHeight > sec_2 && currHeight <= sec_3)//[1] 1.0 - 0.5 ,[2] 0 - 0.5
			{
				const float stp1 = blend_step * count_1_1++;
				blendIndx[0] = 1;
				blendWght[0] = 1.0f - stp1;

 				const float stp2 = blend_step * count_2_0++;
 				blendIndx[1] = 2;
				blendWght[1] = 0.0f + stp2;
			}
			else if (currHeight > sec_3 && currHeight <= sec_4)//[1] 0.5 - 0 ,[2] 0.5 - 1
			{
				const float stp1 = blend_step * count_1_1++;
				blendIndx[0] = 1;
				blendWght[0] = 1.0f - stp1;

				const float stp2 = blend_step * count_2_0++;
				blendIndx[1] = 2;
				blendWght[1] = 0.0f + stp2;
			}
			else if (currHeight > sec_4 && currHeight <= sec_5)// ,[2]  1 - 0.5 
			{
				const float stp2 = blend_step * count_2_1++;
				blendIndx[0] = 2;
				blendWght[0] = 1.0f - stp2;

				const float stp3 = blend_step * count_3_0++;
				blendIndx[1] = 3;
				blendWght[1] = 0.0f + stp3;

 
			}
			else if (currHeight > sec_5 && currHeight <= sec_6)//[3] 0.5 - 1 ,[2]   0.5 - 0
			{
				const float stp2 = blend_step * count_2_1++;
				blendIndx[0] = 2;
				blendWght[0] = 1.0f - stp2;

				const float stp3 = blend_step * count_3_0++;
				blendIndx[1] = 3;
				blendWght[1] = 0.0f + stp3;
			}
			else if(currHeight > sec_6 /*&& currHeight <= sec_7*/)
			{
				blendIndx[0] = 3;
				blendWght[0] = 1.0f;
			}

			for (int i = 0; i < VTX_IN_CIRCLE; i++)
			{

				Vector3 pos = { radius* cosf(curr_deg), radius * sinf(curr_deg), centre.z + height_step * j };
				Vector3 normal = H::Math::Normalize(pos - Vector3{ centre.x,centre.y,pos.z });

				mesh.vertices.push_back(
					{ pos,
					normal,
					normal.Cross({0.0f,0.0f,1.0f}),
					{currU,currV},
					{blendIndx[0],blendIndx[1],0,0},
					{blendWght[0],blendWght[1],blendWght[2],blendWght[3]}
					});
				curr_deg += deg_step;
				currU += u_step;
			}
			//curr_blendWght_height += blendWght_step;
			currV += v_step;
			curr_deg = 0.0f;
			currU = 0.0f;
		}
		// bot
		mesh.vertices.push_back({ centre,
					{0.0f,0.0f,1.0f},
					{0.0f,-1.0f,0.0f},
					{0.0f,0.0f},
					{0,0,0,0},
					{1.0f,0.0f,0.0f,0.0f}
			});
		//top
		mesh.vertices.push_back({ {centre.x,centre.y,centre.z + height },
			{0.0f,0.0f,1.0f},
			{0.0f,1.0f,0.0f},
			{1.0f,1.0f},
			{3,0,0,0},
			{1.0f,0.0f,0.0f,0.0f}
			});



		//times = numbone
		mesh.indices.reserve((6 * (VTX_IN_CIRCLE - 1)) * vert_slice + 6 * ( VTX_IN_CIRCLE-1));

		for (int i = 0; i < vert_slice; i++)
		{
			int idx = (VTX_IN_CIRCLE * i);

			for (int j = 0; j < VTX_IN_CIRCLE - 1; j++)//?
			{

				mesh.indices.push_back(idx + j);
				mesh.indices.push_back(idx + j + VTX_IN_CIRCLE + 1);
				mesh.indices.push_back((idx + j + VTX_IN_CIRCLE));

				mesh.indices.push_back(idx + j);
				mesh.indices.push_back((idx + j + 1));
				mesh.indices.push_back(idx + j + VTX_IN_CIRCLE + 1);
			}
		}

		//top/bot
		const int botIndex = mesh.vertices.size() - 2;
		const int topIndex = mesh.vertices.size() - 1;
		const int topStartIndex = mesh.vertices.size() - 3 - VTX_IN_CIRCLE + 1;


		for (int i = 0; i < VTX_IN_CIRCLE - 1; i++)
		{
			//bot
			mesh.indices.push_back(i);
			mesh.indices.push_back(botIndex);
			mesh.indices.push_back((i + 1) % VTX_IN_CIRCLE);
			//top
			mesh.indices.push_back((topStartIndex + i + 1));
			mesh.indices.push_back(topIndex);
			mesh.indices.push_back(topStartIndex + i);
		}
 
   
	}

	return mesh;
}

 