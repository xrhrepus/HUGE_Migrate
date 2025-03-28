#pragma once
#include "TRenderPipelineInterface.h"
#include "TStandardShader.h"
#include "TShadowMapShader.h"

struct TCastShadowDrawCommand {
    H::Graphics::MeshBuffer* meshBuf;
    uint32_t numOfInstance;
    std::vector<H::Math::Matrix4> worldPoss;
};

class TCastShadowRenderPass : public TIRenderPass {
public:
    void Init() override;
    void Term() override;
    void execute() override;
    void clear() override;
    const std::string& getName() const override;
    void addOutputToRenderPipeline(TRenderPipeline& pipeline) const override;

    void add(TCastShadowDrawCommand&& cmd);
    void updateLightVPMatrix(const H::Math::Vector3& lightDir, const H::Math::Vector3& lightPos);
    const H::Math::Matrix4& getLightViewMatrix() const;
    const H::Math::Matrix4& getLightProjectionMatrix() const;
    TransformData calculateTFData(const H::Math::Vector3& meshWorldPos) const;

    ID3D11ShaderResourceView* getRTTexture() const;
private:
    inline static const std::string RP_NAME = "TCastShadowRenderPass";
    TShadowMapShader mShader;

    H::Math::Matrix4 mLightViewMatrix;
    H::Math::Matrix4 mLightProjectionMatrix;

    H::Graphics::TypedDynamicStructuredBuffer<TransformData, 1> mLightSourceData;
    H::Graphics::TypedDynamicStructuredBuffer<TransformData, 100> mTransformBuf;
    std::vector<TCastShadowDrawCommand> mDrawRequests;

    const std::string mOutShadowMapName = "ShadowMap";
    H::Graphics::RenderTarget mShadowMapRT;
};