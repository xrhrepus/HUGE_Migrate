#pragma once
#include "TStandardShader.h"
#include "TRenderPipelineInterface.h"
#include "TRenderPipeline.h"
#include "TMeshRenderer.h"
#include "TDirectionalLightSource.h"

/// <summary>
/// Draw 1 mesh with different data N times in 1 call
/// </summary>
struct TSampleInstancedRendering {
    std::unique_ptr<TStandardShader> mShader;
    std::unique_ptr<TStandardMaterial> mMaterial;
    std::unique_ptr<TStandardMaterial> mMaterial2;
    TMeshRenderer mMeshRenderer;
    TMeshRenderer mMeshRenderer2;
    TMeshRenderer mMeshRenderer3;

    TextureId mDiffuseTex;
    TextureId mDiffuseTex2;

    TRenderPipeline mRenderPipeline;
    TDirectionalLightSource mDirectionalLight;

    void Init();
    void Term();
    void DrawWithRenderPass(const Camera& cam);
    void DebugUI();
};

