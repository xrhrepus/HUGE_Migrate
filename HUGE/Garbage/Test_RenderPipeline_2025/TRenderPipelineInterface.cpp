#include "TRenderPipelineInterface.h"

TIMaterial::TIMaterial(const std::string& name, TIShader& shader)
    : mName(name)
    , mShader(shader)
{
}

const TIShader& TIMaterial::GetShader() const
{
    return mShader;
}

const std::string& TIMaterial::getName() const
{
    return mName;
}

void TIRenderPass::addOutputToRenderPipeline(TRenderPipeline&) const
{
    // do nothing
}
