#include "TRenderPipeline.h"
//#include "TStandardShader.h"
#include "TRenderPipelineInterface.h"

void TRenderPipeline::Init()
{
    for (auto&& [name, rp] : mRPs)
    {
        rp->Init();
    }
}

void TRenderPipeline::Term()
{
    for (auto&& [name, rp] : mRPs)
    {
        rp->Term();
    }
}

void TRenderPipeline::execute()
{
    for (auto&& [name, rp] : mRPs)
    {
        rp->execute();
        rp->clear();
    }
}

void TRenderPipeline::add(std::unique_ptr<TIRenderPass>&& rp)
{
    mRPs[rp->getName()] = std::move(rp);
}

const TIRenderPass* TRenderPipeline::getRP(const std::string& name) const
{
    if (auto it = mRPs.find(name); it != mRPs.end())
    {
        return it->second.get();
    }
    return nullptr;
}

TIRenderPass* TRenderPipeline::getRP(const std::string& name)
{
    if (auto it = mRPs.find(name); it != mRPs.end())
    {
        return it->second.get();
    }
    return nullptr;
}
