#pragma once
#include "Mesh.h"

namespace H::Graphics
{
	class ObjLoader
	{
	public:
		static void Load(const std::filesystem::path& filename, float scale, Mesh& mesh);
 	private:

	};
 
}
