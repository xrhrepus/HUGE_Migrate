#pragma once
#include "TRenderPipelineInterface.h"
#include "TStandardShader.h"

/// <summary>
/// no interface, specific to each pass
/// you will need to get specific Pass to add command anyways
/// </summary>
struct TStandardDrawCommand {
    TIMaterial* mat;
    H::Graphics::MeshBuffer* meshBuf;
    uint32_t numOfInstance;
    std::vector<H::Graphics::TransformData> tf;
};

class TStarndardRenderPass : public TIRenderPass {
public:
    void Init() override;
    void Term() override;
    void execute() override;
    void clear() override;
    const std::string& getName() const override;

    void add(TStandardDrawCommand&& cmd);
private:
    struct CompareByName {
        bool operator()(TIMaterial* a, TIMaterial* b) const {
            return a->getName() < b->getName();
        }
    };
    inline static const std::string RP_NAME = "TStarndardRenderPass";
    H::Graphics::TypedDynamicStructuredBuffer<TransformData, 100> mTransformBuf;
    std::map<TIMaterial*, std::vector<TStandardDrawCommand>, CompareByName> mDrawRequests;
};
