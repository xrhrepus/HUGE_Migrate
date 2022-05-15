#include "Precompiled.h"
#include "ObjLoader.h"

using namespace H::Graphics;

void H::Graphics::ObjLoader::Load(const std::filesystem::path & filename, float scale, Mesh & mesh)
{
	std::vector<uint32_t> positionIndices, normalIndices, uvIndices;
	std::vector<Math::Vector3> positions;
	std::vector<Math::Vector3> normals;
	std::vector<Math::Vector2> uvs;

	FILE* file = nullptr;
	fopen_s(&file, filename.u8string().c_str(), "r");
	ASSERT(file, "[ModelLoader] Failed to Load file: %s .", filename.u8string().c_str());

	while (true)
	{
		char buf[128];

		int res = fscanf_s(file, "%s", buf, std::size(buf));
		if (res == EOF)
			break;

		if (strcmp(buf,"v") == 0)
		{
			Math::Vector3 pos;
			fscanf_s(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
			positions.push_back(pos * scale);
		}
		else if (strcmp(buf, "vn") == 0)
		{
			Math::Vector3 norm;
			fscanf_s(file, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
			normals.push_back(norm );
		}
		else if (strcmp(buf, "vt") == 0)
		{
			Math::Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back({ uv.x,1.0f - uv.y });
		}
		else if (strcmp(buf, "f") == 0)
		{
			uint32_t v[4], t[4], n[4];
			fgets(buf, std::size(buf), file);
			if (sscanf_s(buf,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2],
				&v[3], &t[3], &n[3]) == 12)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);

				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[2]);
				positionIndices.push_back(v[3]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[2]);
				uvIndices.push_back(t[3]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[2]);
				normalIndices.push_back(n[3]);

			}
			else if (sscanf_s(buf, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&v[0], &t[0], &n[0],
				&v[1], &t[1], &n[1],
				&v[2], &t[2], &n[2]
				 ) == 9)
			{
				positionIndices.push_back(v[0]);
				positionIndices.push_back(v[1]);
				positionIndices.push_back(v[2]);
				uvIndices.push_back(t[0]);
				uvIndices.push_back(t[1]);
				uvIndices.push_back(t[2]);
				normalIndices.push_back(n[0]);
				normalIndices.push_back(n[1]);
				normalIndices.push_back(n[2]);
			}
			else
			{
				ASSERT(false, "[] unsupport file format");
			}
		}

	}

	fclose(file);

	//reconstruct vtx
	std::vector<Vertex> vertices;
	vertices.reserve(positionIndices.size());
	for (size_t i = 0; i < positionIndices.size(); ++i)
	{
		Vertex vt;
		vt.position = positions[positionIndices[i] - 1];
		vt.normal = normals[normalIndices[i] - 1];
		
		vt.texCoord = uvs[uvIndices[i] - 1];
		vertices.emplace_back(vt);
	}

	std::vector<uint32_t> indices;
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0);

	mesh.vertices = std::move(vertices);
	mesh.indices = std::move(indices);


}
