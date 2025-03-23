#include "TRenderPipeline.h"

TShader::TShader(const std::filesystem::path& path) {
    mVs.Initialize(path.wstring().c_str());
    mPs.Initialize(path.wstring().c_str(), "PS");
    sbm.Initialize();
    sbl.Initialize();
}
void TShader::Bind(ID3D11DeviceContext* context) const {
    mVs.Bind();
    mPs.Bind();

    // t8
    sbm.BindPS(8);
    // t9
    sbl.BindVS(9);
    sbl.BindPS(9);
    // t10
    difuseTextures.BindPS(10);
}

TRenderInstanceData TShader::addMatToShader(DirectionalLight& dl, Material& mt, TextureId texId) {
    TRenderInstanceData r;
    tempLight.emplace_back(dl);
    tempMat.emplace_back(mt);
    tempTexture.emplace_back(texId);
    r.mLightIdx = tempLight.size() - 1;
    r.mMaterialIdx = tempMat.size() - 1;
    r.mDiffuseMapIdx = tempTexture.size() - 1;
    return r;
}

void TShader::batchMaterialData() {
    sbl.Set(*tempLight.data());
    sbm.Set(*tempMat.data());
    for (auto&& tid : tempTexture)
    {
        difuseTextures.PushBackTexture(*TextureManager::Get()->GetTexture(tid));
    }
}
TMaterial::TMaterial(TShader& shader, DirectionalLight dl, Material mt, TextureId texId)
    : mShader(shader)
    // member assign
{
    mrid = shader.addMatToShader(dl, mt, texId);
}
void TMaterial::Bind(ID3D11DeviceContext* context) {

}
void TRenderPass::Init() {
    // 
    mRenderInstanceBuf.Initialize();
    mTransformBuf.Initialize();
}
void TRenderPass::execute() {
    DepthStencilManager::Get()->GetDepthStencilState("ZTest")->Set();
    auto context = GetContext();
    for (auto&& [mat, drawCmds] : mDrawRequests)
    {
        // vs,ps
        mat->mShader.Bind(context);
        // indices in shader of tex,color data used by this material 
        mRenderInstanceBuf.Set(mat->mrid);
        // b10
        mRenderInstanceBuf.BindPS(10);

        for (auto&& cmd : drawCmds)
        {
            // transform index is same as instanceID
            // all same mesh and material, just draw with different transform
            mTransformBuf.Set(*cmd.tf.data());
            mTransformBuf.BindVS(7);
            cmd.meshBuf->RenderInstanced(cmd.numOfInstance);
        }
    }
}
void TRenderPass::add(TDrawCommand&& cmd) {
    mTransformBuf.Set(*cmd.tf.data());
    //mDrawRequests.emplace(cmd.mat, std::move(cmd));
    mDrawRequests[cmd.mat].emplace_back(std::move(cmd));
}

void TRenderPass::clear() {
    mDrawRequests.clear();
}
void TSampleInstancedRendering::Init() {
    const wchar_t* STANDARD_INSTANCED_FILE_PATH = L"../../Assets/Shaders/StandardInstanced.fx";

    mDiffuseTex = TextureManager::Get()->LoadTexture("earth.jpg");
    mDiffuseTex2 = TextureManager::Get()->LoadTexture("fruit42_x10.png");
    mShader = std::make_unique<TShader>(STANDARD_INSTANCED_FILE_PATH);
    mMaterial = std::make_unique<TMaterial>(*mShader, DirectionalLight{}, Material{}, mDiffuseTex);
    mMaterial2 = std::make_unique<TMaterial>(*mShader, DirectionalLight{}, Material{}, mDiffuseTex2);

    mShader->batchMaterialData();

    sbt.Initialize();
    sbm.Initialize();
    sbi.Initialize();

    auto m = MeshBuilder::CreateCube(5, 5, 5, { 0.0f,0.0f,0.0f });
    mMeshRenderer.mMesh = m;
    mMeshRenderer.mMeshBuffer.Initialize(mMeshRenderer.mMesh);

    mRenderPass.Init();

}
void TSampleInstancedRendering::Term() {
}
void TSampleInstancedRendering::DrawWithRenderPass(const Camera& cam) {
    mRenderPass.clear();

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
        TDrawCommand cmd;
        cmd.mat = mMaterial.get();
        cmd.meshBuf = &mMeshRenderer.mMeshBuffer;
        cmd.numOfInstance = 5;
        cmd.tf = std::move(tfs);

        mRenderPass.add(std::move(cmd));
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
        TDrawCommand cmd;
        cmd.mat = mMaterial2.get();
        cmd.meshBuf = &mMeshRenderer.mMeshBuffer;
        cmd.numOfInstance = 7;
        cmd.tf = std::move(tfs);

        mRenderPass.add(std::move(cmd));
    }

    mRenderPass.execute();
}


