#pragma once
#include "Service.h"

namespace H
{
	class MeshService : public Service
	{
	public:

		struct MeshEntry
		{
			H::Graphics::Mesh mesh;
			std::string name;
		};

		META_CLASS_DECLARE;
		void Initialize() override;
		void DebugUI() override;
		void AddMesh(MeshEntry&& entry);
		const MeshEntry& GetMeshEntry(const std::string& name) const;
		void ForEachMesh(std::function<void(const H::MeshService::MeshEntry&)> func) const;

	private:
		std::vector<MeshEntry> mMeshList;
	};

}