#pragma once
#include "Service.h"
#include <Graphics/Inc/Light.h>

namespace H
{
	class LightService : public Service
	{
	public:

		struct DirectionalLightEntry
		{
			std::string name;
			H::Graphics::DirectionalLight dirLight;
		};

		META_CLASS_DECLARE;
		void Initialize() override;
		void DebugUI() override;
		void AddDirectionalLight(DirectionalLightEntry&& entry);
		const DirectionalLightEntry& GetDirectionalLightEntry(const std::string& name) const;
		DirectionalLightEntry& GetDirectionalLightEntry(const std::string& name);
		void ForEachDirectionalLightEntry(std::function<void(const DirectionalLightEntry&)> func) const;

	private:
		std::vector<DirectionalLightEntry> mDirectionalLight;
	};

}