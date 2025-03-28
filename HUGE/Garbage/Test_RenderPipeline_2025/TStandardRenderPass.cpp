#include "TStandardRenderPass.h"
#include "TRenderPipeline.h"


void TStarndardRenderPass::Init() {
    // 
    mTransformBuf.Initialize();
}
void TStarndardRenderPass::Term()
{
    mTransformBuf.Terminate();
}

void TStarndardRenderPass::execute() {
    DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
    auto context = GetContext();
    const TIShader* lastShader = nullptr;

    // some buffers required in the pass
    // only bind once and update data below
    mTransformBuf.BindVS(7);

    for (auto&& [mat, drawCmds] : mDrawRequests)
    {
        // reduce shader bound
        if (&mat->GetShader() != lastShader)
        {
            lastShader = &mat->GetShader();
            // vs,ps
            // and data stored in structuredBuffer, this reduces context switching(reset buffer and bind)
            mat->GetShader().Bind(context);
        }

        // other data that can't be stored together(i.e textures, this material's renderinstanceIdx)
        mat->Bind(context);

        for (auto&& cmd : drawCmds)
        {
            if (cmd.tf.empty())
            {
                continue;
            }
            // transform index is same as instanceID
            // all same mesh and material, just draw with different transform
            mTransformBuf.Map(*cmd.tf.data(), sizeof(cmd.tf[0]) * cmd.tf.size());
            cmd.meshBuf->RenderInstanced(cmd.numOfInstance);
        }
    }
}
void TStarndardRenderPass::add(TStandardDrawCommand&& cmd) {
    mDrawRequests[cmd.mat].emplace_back(std::move(cmd));
}

void TStarndardRenderPass::clear() {
    mDrawRequests.clear();
}
const std::string& TStarndardRenderPass::getName() const
{
    return RP_NAME;
}

void TStarndardRenderPass::getInputFromRenderPipeline(const TRenderPipeline& pipeline)
{
    mShadowMapRTRef = pipeline.getRPOutput(SHADOW_MAP_NAME);
    // assert if get nullptr
    ASSERT(mShadowMapRTRef, "");
}
