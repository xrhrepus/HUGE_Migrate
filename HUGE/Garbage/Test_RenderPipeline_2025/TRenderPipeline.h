#pragma once

#include <Graphics/Inc/Graphics.h>
#include <Graphics/Src/D3DUtil.h>

using namespace H::Graphics;

class TIShader {
public:
    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void Bind(ID3D11DeviceContext* context) const = 0;
};

class TIMaterial {
public:
    TIMaterial(TIShader& shader);
    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void Bind(ID3D11DeviceContext* context) const = 0;
    const TIShader& GetShader() const;

protected:
    const TIShader& mShader;
};

class TIRenderPass {
public:
    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void execute() = 0;
    virtual void clear() = 0;
    virtual const std::string& getName() const = 0;
};

struct TStandardMaterialInstanceData {
    UINT mLightIdx;
    UINT mMaterialIdx;
    UINT mDiffuseMapIdx;
    UINT padding;
};

class TStandardShader : public TIShader {
public:
    void Init() override;
    void Term() override;
    void Bind(ID3D11DeviceContext* context) const override;
    TStandardMaterialInstanceData addMatToShader(DirectionalLight& dl, Material& mt, TextureId texId);
    void batchMaterialData();

private:
    const std::filesystem::path STANDARD_INSTANCED_FILE_PATH = L"../../Assets/Shaders/StandardInstanced.fx";

	VertexShader mVs;
	PixelShader mPs;

    static constexpr size_t MAX_INSTANCE_COUNT = 100;

    std::vector<DirectionalLight> tempLight;
    std::vector<Material> tempMat;

    H::Graphics::TypedStructuredBuffer<DirectionalLight, MAX_INSTANCE_COUNT> mDirectionalLightsBuf;
    H::Graphics::TypedStructuredBuffer<Material, MAX_INSTANCE_COUNT> mMaterialsBuf;
};

class TStandardMaterial : public TIMaterial {
public:
    TStandardMaterial(TStandardShader& shader, DirectionalLight dl, Material mt, TextureId texId);
    void Init() override;
    void Term() override;

    void Bind(ID3D11DeviceContext* context) const;

private:
    const TStandardShader& mStandardShader;
    TStandardMaterialInstanceData mMaterialInstanceIdx;

    DirectionalLight mDirectionalLight;
    Material mMaterial;
    TextureId mDiffuseTex;

    H::Graphics::TypedConstantBuffer<TStandardMaterialInstanceData> materialInstanceBuf;

};

/// <summary>
/// no interface, specific to each pass
/// you will need to get specific Pass to add command anyways
/// </summary>
struct TStandardDrawCommand {
    TIMaterial* mat;
    H::Graphics::MeshBuffer* meshBuf;
    uint32_t numOfInstance;
    std::vector<H::Graphics::TransformData> tf;
};

class TStarndardRenderPass : public TIRenderPass {
public:
    void Init() override;
    void Term() override;
    void execute() override;
    void clear() override;
    const std::string& getName() const override;

    void add(TStandardDrawCommand&& cmd);
private:
    inline static const std::string RP_NAME = "TStarndardRenderPass";
    H::Graphics::TypedDynamicStructuredBuffer<TransformData, 100> mTransformBuf;
    std::unordered_map<TIMaterial*, std::vector<TStandardDrawCommand>> mDrawRequests;

};

/// <summary>
/// Manages all render pass.  And stored pipeline outputs
/// </summary>
class TRenderPipeline {
public:
    void Init();
    void Term();
    void execute();
    void add(std::unique_ptr<TIRenderPass>&& rp);
    const TIRenderPass* getRP(const std::string& name) const;
    TIRenderPass* getRP(const std::string& name);

    std::unordered_map<std::string, std::unique_ptr<TIRenderPass>> mRPs;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> mRPOutput;
};

class TMeshRenderer {
public:
    //MeshInstanced mMesh;
    H::Graphics::Mesh mMesh;
    H::Graphics::MeshBuffer mMeshBuffer;
};

/// <summary>
/// Draw 1 mesh with different data N times in 1 call
/// </summary>
struct TSampleInstancedRendering {
    std::unique_ptr<TStandardShader> mShader;
    std::unique_ptr<TStandardMaterial> mMaterial;
    std::unique_ptr<TStandardMaterial> mMaterial2;
    TMeshRenderer mMeshRenderer;
    TMeshRenderer mMeshRenderer2;

    TextureId mDiffuseTex;
    TextureId mDiffuseTex2;

    TRenderPipeline mRenderPipeline;

    void Init();
    void Term();
    void DrawWithRenderPass(const Camera& cam);
};


/// <summary>
/// Vertex with additional instanceID to support batch rendering. Later moved to Graphics lib.
/// </summary>
struct VertexInstanced
{
    // todo: update VE and GetVertexLayout to support uint
    VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord);

    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector2 texCoord;
    UINT instanceID;

    void fromVertex(const H::Graphics::Vertex& vtx) {
        position = vtx.position;
        normal = vtx.normal;
        tangent = vtx.tangent;
        texCoord = vtx.texCoord;
    }
};
using MeshInstanced = H::Graphics::MeshBase<VertexInstanced>;
