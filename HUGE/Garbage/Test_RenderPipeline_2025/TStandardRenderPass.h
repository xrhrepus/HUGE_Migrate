#pragma once
#include "TRenderPipelineInterface.h"
#include "TStandardShader.h"

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
