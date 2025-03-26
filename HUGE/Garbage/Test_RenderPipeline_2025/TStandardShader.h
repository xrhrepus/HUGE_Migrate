#pragma once
#include "TRenderPipelineInterface.h"

struct TStandardMaterialInstanceData {
    UINT mLightIdx;
    UINT mMaterialIdx;
    UINT mDiffuseMapIdx;
    UINT padding;
};

class TStandardShader : public TIShader {
public:
    void Init() override;
    void Term() override;
    void Bind(ID3D11DeviceContext* context) const override;
    const std::string& getName() const override;
    TStandardMaterialInstanceData addMatToShader(DirectionalLight& dl, Material& mt, TextureId texId);
    void batchMaterialData();

private:
    inline static std::string M_NAME = "StandardShader";
    const std::filesystem::path STANDARD_INSTANCED_FILE_PATH = L"../../Assets/Shaders/StandardInstanced.fx";

    VertexShader mVs;
    PixelShader mPs;

    static constexpr size_t MAX_INSTANCE_COUNT = 100;

    std::vector<DirectionalLight> tempLight;
    std::vector<Material> tempMat;

    H::Graphics::TypedStructuredBuffer<DirectionalLight, MAX_INSTANCE_COUNT> mDirectionalLightsBuf;
    H::Graphics::TypedStructuredBuffer<Material, MAX_INSTANCE_COUNT> mMaterialsBuf;
};

class TStandardMaterial : public TIMaterial {
public:
    TStandardMaterial(const std::string& name, TStandardShader& shader, DirectionalLight dl, Material mt, TextureId texId);
    void Init() override;
    void Term() override;

    void Bind(ID3D11DeviceContext* context) const;

private:
    const TStandardShader& mStandardShader;
    TStandardMaterialInstanceData mMaterialInstanceIdx;

    DirectionalLight mDirectionalLight;
    Material mMaterial;
    TextureId mDiffuseTex;

    H::Graphics::TypedConstantBuffer<TStandardMaterialInstanceData> materialInstanceBuf;

};

