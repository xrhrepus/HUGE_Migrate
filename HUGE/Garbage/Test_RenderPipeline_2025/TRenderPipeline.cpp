#include "TRenderPipeline.h"

void TStandardShader::Init()
{
    mVs.Initialize(STANDARD_INSTANCED_FILE_PATH.wstring().c_str());
    mPs.Initialize(STANDARD_INSTANCED_FILE_PATH.wstring().c_str(), "PS");
    mMaterialsBuf.Initialize();
    mDirectionalLightsBuf.Initialize();
}
void TStandardShader::Term()
{
    mMaterialsBuf.Terminate();
    mDirectionalLightsBuf.Terminate();
    mVs.Terminate();
    mPs.Terminate();
}
void TStandardShader::Bind(ID3D11DeviceContext* context) const {
    mVs.Bind();
    mPs.Bind();

    // t8
    mMaterialsBuf.BindPS(8);
    // t9
    mDirectionalLightsBuf.BindVS(9);
    mDirectionalLightsBuf.BindPS(9);

}

TStandardMaterialInstanceData TStandardShader::addMatToShader(DirectionalLight& dl, Material& mt, TextureId texId) {
    TStandardMaterialInstanceData r;
    tempLight.emplace_back(dl);
    tempMat.emplace_back(mt);
    //diffuseTextures.emplace_back(texId);
    r.mLightIdx = tempLight.size() - 1;
    r.mMaterialIdx = tempMat.size() - 1;
    //r.mDiffuseMapIdx = diffuseTextures.size() - 1;
    return r;
}

void TStandardShader::batchMaterialData() {
    mDirectionalLightsBuf.Set(*tempLight.data());
    mMaterialsBuf.Set(*tempMat.data());
}

TIMaterial::TIMaterial(TIShader& shader) 
    : mShader(shader) {
}

const TIShader& TIMaterial::GetShader() const
{
    return mShader;
}



TStandardMaterial::TStandardMaterial(TStandardShader& shader, DirectionalLight dl, Material mt, TextureId texId)
    : TIMaterial(shader)
    , mStandardShader(shader)
    , mDirectionalLight(dl)
    , mMaterial(mt)
    , mDiffuseTex(texId)
{
    mMaterialInstanceIdx = shader.addMatToShader(dl, mt, texId);
}

void TStandardMaterial::Init() {
    materialInstanceBuf.Initialize();
 }
void TStandardMaterial::Term() {
    materialInstanceBuf.Terminate();
}

void TStandardMaterial::Bind(ID3D11DeviceContext* context) const {
    // if not using formal tex2DArray, let material bind the texture they are using
    TextureManager::Get()->GetTexture(mDiffuseTex)->BindPS(0);

    materialInstanceBuf.Set(mMaterialInstanceIdx);
    // b10
    materialInstanceBuf.BindPS(10);

}

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
    for (auto&& [mat, drawCmds] : mDrawRequests)
    {
        // vs,ps
        // and data stored in structuredBuffer, this reduces context switching(reset buffer and bind)
        mat->GetShader().Bind(context);
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
            mTransformBuf.Set(*cmd.tf.data(), sizeof(cmd.tf[0]) * cmd.tf.size());
            mTransformBuf.BindVS(7);
            cmd.meshBuf->RenderInstanced(cmd.numOfInstance);
        }
    }
}
void TStarndardRenderPass::add(TStandardDrawCommand&& cmd) {
    //mTransformBuf.Set(*cmd.tf.data());
    //mDrawRequests.emplace(cmd.mat, std::move(cmd));
    mDrawRequests[cmd.mat].emplace_back(std::move(cmd));
}

void TStarndardRenderPass::clear() {
    mDrawRequests.clear();
}
const std::string& TStarndardRenderPass::getName() const
{
    return RP_NAME;
}
void TSampleInstancedRendering::Init() {
    mDiffuseTex = TextureManager::Get()->LoadTexture("earth.jpg");
    mDiffuseTex2 = TextureManager::Get()->LoadTexture("fruit42_x10.png");

    mShader = std::make_unique<TStandardShader>();
    mShader->Init();
    mMaterial = std::make_unique<TStandardMaterial>(*mShader, DirectionalLight{}, Material{}, mDiffuseTex);
    mMaterial->Init();
    mMaterial2 = std::make_unique<TStandardMaterial>(*mShader, DirectionalLight{}, Material{}, mDiffuseTex2);
    mMaterial2->Init();

    mShader->batchMaterialData();

    auto m = MeshBuilder::CreateCube(5, 5, 5, { 0.0f,0.0f,0.0f });
    mMeshRenderer.mMesh = m;
    mMeshRenderer.mMeshBuffer.Initialize(mMeshRenderer.mMesh);

    auto m2 = MeshBuilder::CreateShpere(5, 5, { 0.0f,0.0f,0.0f });
    mMeshRenderer2.mMesh = m2;
    mMeshRenderer2.mMeshBuffer.Initialize(mMeshRenderer2.mMesh);


    mRenderPipeline.add(std::make_unique<TStarndardRenderPass>());
    mRenderPipeline.Init();

}
void TSampleInstancedRendering::Term() {
    mRenderPipeline.Term();
}
void TSampleInstancedRendering::DrawWithRenderPass(const Camera& cam) {

    TStarndardRenderPass* standardPass = static_cast<TStarndardRenderPass*>(mRenderPipeline.getRP("TStarndardRenderPass"));
    if (!standardPass)
    {
        return;
    }
    {
        std::vector<TransformData> tfs;
        //===============
        {
            Matrix4 vm = cam.GetViewMatrix();
            Matrix4 pm = cam.GetPerspectiveMatrix();
            Matrix4 worldMat;
            worldMat.Translation({ -10.0f,0.0f,0.0f });
            Matrix4 world = worldMat;
            Matrix4 comp = world * vm * pm;

            const float halfpi = H::Math::Constants::Pi * 0.25f;
            Matrix4 ry;
            ry.RotationY(halfpi);
            Matrix4 rot = ry;

            for (size_t i = 0; i < 5; i++)
            {
                Matrix4 worldMat1;
                worldMat1.Translation({ -20.0f * i,0.0f,0.0f });
                Matrix4 world1 = worldMat1 * rot;
                Matrix4 comp1 = world1 * vm * pm;
                //Matrix4 comp1 = pm * vm * world1;
                TransformData tf1;
                tf1.world = H::Math::Transpose(world1);
                tf1.wvp = H::Math::Transpose(comp1);
                tf1.viewPosition = cam.GetPosition();
                tfs.emplace_back(tf1);
            }
        }
        //===============
        TStandardDrawCommand cmd;
        cmd.mat = mMaterial.get();
        cmd.meshBuf = &mMeshRenderer.mMeshBuffer;
        cmd.numOfInstance = 5;
        cmd.tf = std::move(tfs);

        standardPass->add(std::move(cmd));
    }
    {
        std::vector<TransformData> tfs;
        //===============
        {
            Matrix4 vm = cam.GetViewMatrix();
            Matrix4 pm = cam.GetPerspectiveMatrix();
            Matrix4 worldMat;
            worldMat.Translation({ -10.0f,0.0f,0.0f });
            Matrix4 world = worldMat;
            Matrix4 comp = world * vm * pm;

            const float halfpi = H::Math::Constants::Pi * 0.25f;
            Matrix4 ry;
            ry.RotationY(halfpi);
            Matrix4 rot = ry;

            for (size_t i = 0; i < 8; i++)
            {
                Matrix4 worldMat1;
                worldMat1.Translation({ -30.0f * i,0.0f,0.0f });
                Matrix4 world1 = rot * worldMat1;
                Matrix4 comp1 = world1 * vm * pm;
                //Matrix4 comp1 = pm * vm * world1;
                TransformData tf1;
                tf1.world = H::Math::Transpose(world1);
                tf1.wvp = H::Math::Transpose(comp1);
                tf1.viewPosition = cam.GetPosition();
                tfs.emplace_back(tf1);
            }
        }
        //===============
        {
            TStandardDrawCommand cmd;
            cmd.mat = mMaterial.get();
            cmd.meshBuf = &mMeshRenderer2.mMeshBuffer;
            cmd.numOfInstance = 7;
            cmd.tf = std::move(tfs);

            standardPass->add(std::move(cmd));
        }
    }

    mRenderPipeline.execute();
}

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
