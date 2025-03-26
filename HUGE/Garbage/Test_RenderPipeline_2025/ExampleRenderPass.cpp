#include "ExampleRenderPass.h"
#include "TStandardShader.h"
#include "TStandardRenderPass.h"

void TSampleInstancedRendering::Init() {
    mDiffuseTex = TextureManager::Get()->LoadTexture("earth.jpg");
    mDiffuseTex2 = TextureManager::Get()->LoadTexture("fruit42_x10.png");

    mShader = std::make_unique<TStandardShader>();
    mShader->Init();
    DirectionalLight dl;
    Material mt;
    mt.power = 32;
    mMaterial = std::make_unique<TStandardMaterial>("stdmat1", *mShader, dl, mt, mDiffuseTex);
    mMaterial->Init();

    mMaterial2 = std::make_unique<TStandardMaterial>("stdmat2", *mShader, dl, mt, mDiffuseTex2);
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
        cmd.mat = mMaterial2.get();
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
