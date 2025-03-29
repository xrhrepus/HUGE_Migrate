#include "TCastShadowRenderPass.h"
#include "TRenderPipeline.h"
#include "TDirectionalLightSource.h"


void TCastShadowRenderPass::Init()
{
    mShader.Init();
    mLightSourceData.Initialize();
    mTransformBuf.Initialize();
    auto gSys = GraphicSystem::Get();
    mShadowMapRT.Initialize(2048, 2048,
        RenderTarget::Format::RGBA_U8);
    mShadowMapRT.SetClearColor(0, 0, 0, 1);
}

void TCastShadowRenderPass::Term()
{
    mShader.Term();
    mLightSourceData.Terminate();
    mTransformBuf.Terminate();
    mShadowMapRT.Terminate();
}

void TCastShadowRenderPass::execute()
{
    mShadowMapRT.BeginRender();
    DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
    auto context = GetContext();
    mShader.Bind(context);
    mTransformBuf.BindVS(7);
    mLightSourceData.BindVS(8);
    for (auto&& cmd : mDrawRequests)
    {
        if (cmd.worldPoss.empty())
        {
            continue;
        }
        std::vector<H::Graphics::TransformData> tf;
        for (auto&& w : cmd.worldPoss)
        {
            TransformData tfd = calculateTFData(w, *mDirectionalLightRef);
            //tfd.world = w;
            ////tfd.viewPosition = mLightViewMatrix;
            //
            //H::Math::Matrix4 wvp = w * mLightViewMatrix * mLightProjectionMatrix;
            //tfd.wvp = wvp.Transpose();
            //tfd.world = w.Transpose();
            //tfd.wvp = tfd.wvp.Transpose();
            tf.emplace_back(tfd);
        }

        // transform index is same as instanceID
        // all same mesh and material, just draw with different transform
        mTransformBuf.Map(*tf.data(), sizeof(tf[0]) * tf.size());

        //mLightSourceData.Set(calculateTFData(cmd.worldPoss));


        cmd.meshBuf->RenderInstanced(cmd.numOfInstance);
    }

    mShadowMapRT.EndRender();
}

void TCastShadowRenderPass::clear()
{
    mDrawRequests.clear();
}

const std::string& TCastShadowRenderPass::getName() const
{
    return RP_NAME;
}

void TCastShadowRenderPass::addOutputToRenderPipeline(TRenderPipeline& pipeline) const
{
    pipeline.addRPOutput(mOutShadowMapName, mShadowMapRT);
}

void TCastShadowRenderPass::add(TCastShadowDrawCommand&& cmd)
{
    mDrawRequests.emplace_back(std::move(cmd));
}

//void TCastShadowRenderPass::updateLightVPMatrix(const H::Math::Vector3& lightDir, const H::Math::Vector3& lightPos)
//{
//    const H::Math::Vector3 right = H::Math::Cross(H::Math::Vector3::yAxis(), lightDir);
//    const H::Math::Vector3 up = H::Math::Cross(lightDir, right);
//
//    mLightViewMatrix = H::Graphics::ComputeViewMatrix(right, up, lightDir, lightPos);
//    //mLightProjectionMatrix = H::Graphics::ComputePerspectiveMatrix(1.0f, 1000.0f, 60.0f * H::Math::Constants::DegToRad, 2.66f);
//    mLightProjectionMatrix = H::Graphics::CreateOrthographicMatrix(2048, 2048, 1.0f, 1000.0f);
//
//}
//
//const H::Math::Matrix4& TCastShadowRenderPass::getLightViewMatrix() const
//{
//    return mLightViewMatrix;
//}
//
//const H::Math::Matrix4& TCastShadowRenderPass::getLightProjectionMatrix() const
//{
//    return mLightProjectionMatrix;
//}

TransformData TCastShadowRenderPass::calculateTFData(const H::Math::Matrix4& meshWorld, const TDirectionalLightSource& dls) const
{
    TransformData tf;
    tf.world = meshWorld;
    tf.wvp = tf.world * dls.mLightViewMatrix * dls.mLightProjectionMatrix;
    tf.world = tf.world.Transpose();
    tf.wvp = tf.wvp.Transpose();

    return tf;
}

void TCastShadowRenderPass::setLightSource(const TDirectionalLightSource& dl)
{
    mDirectionalLightRef = &dl;
}

ID3D11ShaderResourceView* TCastShadowRenderPass::getRTTexture() const
{
    return mShadowMapRT.GetShaderResourceView();
}
