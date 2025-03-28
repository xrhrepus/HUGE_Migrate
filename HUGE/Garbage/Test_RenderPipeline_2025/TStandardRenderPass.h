#pragma once
#include "TRenderPipelineInterface.h"
#include "TStandardShader.h"

struct TDirectionalLightSource;

struct ShadowBuffer 
{
    H::Math::Matrix4 lightWVP;
};

/// <summary>
/// no interface, specific to each pass
/// you will need to get specific Pass to add command anyways
/// </summary>
struct TStandardDrawCommand {
    TIMaterial* mat;
    H::Graphics::MeshBuffer* meshBuf;
    uint32_t numOfInstance;
    std::vector<H::Math::Matrix4> worlds;
    std::vector<H::Graphics::TransformData> tf;
};

class TStarndardRenderPass : public TIRenderPass {
public:
    void Init() override;
    void Term() override;
    void execute() override;
    void clear() override;
    const std::string& getName() const override;
    void getInputFromRenderPipeline(const TRenderPipeline& pipeline) override;

    void add(TStandardDrawCommand&& cmd);
    // ideally it should be some global service
    void setLightSource(const TDirectionalLightSource& dl);
private:
    inline static const std::string SHADOW_MAP_NAME = "ShadowMap";
    const H::Graphics::RenderTarget* mShadowMapRTRef = nullptr;
    const TDirectionalLightSource* mDirectionalLightRef = nullptr;

    struct CompareByName {
        bool operator()(TIMaterial* a, TIMaterial* b) const {
            return a->getName() < b->getName();
        }
    };
    inline static const std::string RP_NAME = "TStarndardRenderPass";
    H::Graphics::TypedDynamicStructuredBuffer<TransformData, 100> mTransformBuf;
    H::Graphics::TypedDynamicStructuredBuffer<ShadowBuffer, 100> mShadowBuf;
    std::map<TIMaterial*, std::vector<TStandardDrawCommand>, CompareByName> mDrawRequests;
};
