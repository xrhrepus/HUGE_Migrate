#include "ExampleRenderPass.h"
#include "TStandardShader.h"
#include "TStandardRenderPass.h"
#include "TCastShadowRenderPass.h"

void TSampleInstancedRendering::Init() {
    mDirectionalLight.mLightPos = { -121.0f, 75.0f, 100.0f };
    mDirectionalLight.mLight.direction = -H::Math::Normalize(mDirectionalLight.mLightPos);
    mDirectionalLight.updateLightVPMatrix(mDirectionalLight.mLight.direction, mDirectionalLight.mLightPos);

    mDiffuseTex = TextureManager::Get()->LoadTexture("earth.jpg");
    mDiffuseTex2 = TextureManager::Get()->LoadTexture("fruit42_x10.png");

    mShader = std::make_unique<TStandardShader>();
    mShader->Init();
    Material mt;
    mt.power = 32;
    mMaterial = std::make_unique<TStandardMaterial>("stdmat1", *mShader, mDirectionalLight.mLight, mt, mDiffuseTex);
    mMaterial->Init();

    mMaterial2 = std::make_unique<TStandardMaterial>("stdmat2", *mShader, mDirectionalLight.mLight, mt, mDiffuseTex2);
    mMaterial2->Init();

    mShader->batchMaterialData();

    auto m = MeshBuilder::CreateCube(5, 5, 5, { 0.0f,0.0f,0.0f });
    mMeshRenderer.mMesh = m;
    mMeshRenderer.mMeshBuffer.Initialize(mMeshRenderer.mMesh);

    auto m2 = MeshBuilder::CreateShpere(5, 5, { 0.0f,0.0f,0.0f });
    mMeshRenderer2.mMesh = m2;
    mMeshRenderer2.mMeshBuffer.Initialize(mMeshRenderer2.mMesh);

    auto m3 = MeshBuilder::CreateQuad(222, 222, { 0.0f,0.0f,0.0f });
    mMeshRenderer3.mMesh = m3;
    mMeshRenderer3.mMeshBuffer.Initialize(mMeshRenderer3.mMesh);


    mRenderPipeline.add(std::make_unique<TCastShadowRenderPass>());
    mRenderPipeline.add(std::make_unique<TStarndardRenderPass>());
    mRenderPipeline.Init();
    TCastShadowRenderPass* shadowPass = static_cast<TCastShadowRenderPass*>(mRenderPipeline.getRP("TCastShadowRenderPass"));
    //shadowPass->updateLightVPMatrix(mDirectionalLight.mLight.direction, mDirectionalLight.mLightPos);

}
void TSampleInstancedRendering::Term() {
    mRenderPipeline.Term();
}
void TSampleInstancedRendering::DrawWithRenderPass(const Camera& cam) {

    TStarndardRenderPass* standardPass = static_cast<TStarndardRenderPass*>(mRenderPipeline.getRP("TStarndardRenderPass"));
    TCastShadowRenderPass* shadowPass = static_cast<TCastShadowRenderPass*>(mRenderPipeline.getRP("TCastShadowRenderPass"));
    if (!standardPass || !shadowPass)
    {
        return;
    }
    standardPass->setLightSource(mDirectionalLight);
    shadowPass->setLightSource(mDirectionalLight);
    {
        std::vector<TransformData> tfs;
        std::vector<H::Math::Matrix4> worlds;
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
                Vector3 wp = { -20.0f * i,0.0f,0.0f };
                Matrix4 worldMat1;
                worldMat1.Translation(wp);
                Matrix4 world1 = worldMat1 * rot;
                Matrix4 comp1 = world1 * vm * pm;
                //Matrix4 comp1 = pm * vm * world1;
                TransformData tf1;
                tf1.world = H::Math::Transpose(world1);
                tf1.wvp = H::Math::Transpose(comp1);
                tf1.viewPosition = cam.GetPosition();
                tfs.emplace_back(tf1);

                worlds.emplace_back(world1);
            }
        }
        //===============
        TStandardDrawCommand cmd;
        cmd.mat = mMaterial2.get();
        cmd.meshBuf = &mMeshRenderer.mMeshBuffer;
        cmd.numOfInstance = 5;
        cmd.tf = tfs;
        cmd.worlds = worlds;

        standardPass->add(std::move(cmd));

        TCastShadowDrawCommand scmd;
        scmd.meshBuf = &mMeshRenderer.mMeshBuffer;
        scmd.numOfInstance = 5;
        scmd.worldPoss = worlds;
        shadowPass->add(std::move(scmd));
    }
    {
        std::vector<TransformData> tfs;
        std::vector<H::Math::Matrix4> worlds;
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
                Vector3 wp = { -30.0f * i - 5.0f,10.0f,0.0f };
                Matrix4 worldMat1;
                worldMat1.Translation(wp);
                Matrix4 world1 = rot * worldMat1;
                Matrix4 comp1 = world1 * vm * pm;
                //Matrix4 comp1 = pm * vm * world1;
                TransformData tf1;
                tf1.world = H::Math::Transpose(world1);
                tf1.wvp = H::Math::Transpose(comp1);
                tf1.viewPosition = cam.GetPosition();
                tfs.emplace_back(tf1);

                worlds.emplace_back(world1);
            }
        }
        //===============
        {
            TStandardDrawCommand cmd;
            cmd.mat = mMaterial.get();
            cmd.meshBuf = &mMeshRenderer2.mMeshBuffer;
            cmd.numOfInstance = 7;
            cmd.tf = tfs;
            cmd.worlds = worlds;

            standardPass->add(std::move(cmd));

            TCastShadowDrawCommand scmd;
            scmd.meshBuf = &mMeshRenderer2.mMeshBuffer;
            scmd.numOfInstance = 7;
            scmd.worldPoss = worlds;
            shadowPass->add(std::move(scmd));
        }
    }
    {
        std::vector<TransformData> tfs;
        std::vector<H::Math::Matrix4> worlds;

        Matrix4 vm = cam.GetViewMatrix();
        Matrix4 pm = cam.GetPerspectiveMatrix();
        Matrix4 rot;
        rot.RotationX(90 * H::Math::Constants::DegToRad);
        Matrix4 worldMat;
        worldMat.Translation({ 0.0f,-20.0f,0.0f });
        Matrix4 world = rot * worldMat;
        Matrix4 comp = world * vm * pm;

        TransformData tf1;
        tf1.world = H::Math::Transpose(world);
        tf1.wvp = H::Math::Transpose(comp);
        tf1.viewPosition = cam.GetPosition();
        tfs.emplace_back(tf1);
        worlds.emplace_back(world);

        TStandardDrawCommand cmd;
        cmd.mat = mMaterial2.get();
        cmd.meshBuf = &mMeshRenderer3.mMeshBuffer;
        cmd.numOfInstance = 1;
        cmd.tf = tfs;
        cmd.worlds = worlds;

        standardPass->add(std::move(cmd));
    }

    mRenderPipeline.execute();
}

void TSampleInstancedRendering::DebugUI()
{
    TStarndardRenderPass* standardPass = static_cast<TStarndardRenderPass*>(mRenderPipeline.getRP("TStarndardRenderPass"));
    TCastShadowRenderPass* shadowPass = static_cast<TCastShadowRenderPass*>(mRenderPipeline.getRP("TCastShadowRenderPass"));
    if (!standardPass || !shadowPass)
    {
        return;
    }

    ImGui::Begin("testsdmap");
    ImGui::Image(shadowPass->getRTTexture(), { 480,480 });
    ImGui::End();
}
