#pragma once
#include "Service.h"
#include <Graphics/Inc/RenderMaterial.h>

namespace H
{
	class RenderMaterialService : public Service
	{
	public:

		struct RenderMaterialEntry
		{
			H::Graphics::RenderMaterial material;
			std::string name;
		};

		META_CLASS_DECLARE;
		void Initialize() override;
		void DebugUI() override;
		void AddRenderMaterial(RenderMaterialEntry&& entry);
		const RenderMaterialEntry& GetRenderMaterialEntry(const std::string& name) const;
		void ForEachRenderMaterial(std::function<void(const RenderMaterialEntry&)> func) const;

	private:
		std::vector<RenderMaterialEntry> mRenderMaterialList;
	};

}