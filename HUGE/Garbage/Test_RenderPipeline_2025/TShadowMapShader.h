#pragma once
#include "TRenderPipelineInterface.h"

class TShadowMapShader : public TIShader {
public:
    void Init() override;
    void Term() override;
    void Bind(ID3D11DeviceContext* context) const override;
    const std::string& getName() const override;
private:
    inline static std::string M_NAME = "ShadowMapShader";
    const std::filesystem::path STANDARD_INSTANCED_FILE_PATH = L"../../Assets/Shaders/ShadowMapInstanced.fx";

    VertexShader mVs;
    PixelShader mPs;

    static constexpr size_t MAX_INSTANCE_COUNT = 100;
};