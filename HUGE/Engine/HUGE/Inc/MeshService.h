#pragma once
#include "Service.h"

namespace H
{
	class MeshService : public Service
	{
	private:
		struct MeshEntry
		{
			H::Graphics::Mesh mesh;
			std::string name;
		};

	public:
		META_CLASS_DECLARE;
		void Initialize() override;
		void DebugUI() override;
		void AddMesh(MeshEntry&& entry);

	private:
		std::vector<MeshEntry> mMeshList;
	};

}