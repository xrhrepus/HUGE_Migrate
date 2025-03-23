#pragma once

#include <Graphics/Inc/Graphics.h>
#include <Graphics/Src/D3DUtil.h>

using namespace H::Graphics;

struct TRenderInstanceData {
    UINT mLightIdx;
    UINT mMaterialIdx;
    UINT mDiffuseMapIdx;
    UINT padding;
};

class TShader {
public:
    TShader(const std::filesystem::path& path);

    void Bind(ID3D11DeviceContext* context) const;
    TRenderInstanceData addMatToShader(DirectionalLight& dl, Material& mt, TextureId texId);
    void batchMaterialData();


    void ReflectConstantBuffers(ID3DBlob* shaderBlob);
    void ReflectResources(ID3DBlob* shaderBlob);

	VertexShader mVs;
	PixelShader mPs;

    static constexpr size_t MAX_INSTANCE_COUNT = 100;

    std::vector<DirectionalLight> tempLight;
    std::vector<Material> tempMat;
    std::vector<TextureId> tempTexture;

    H::Graphics::TypedStructuredBuffer<DirectionalLight, MAX_INSTANCE_COUNT> sbl;
    H::Graphics::TypedStructuredBuffer<Material, MAX_INSTANCE_COUNT> sbm;
    H::Graphics::TextureArray difuseTextures;
};

class TMaterial {
public:
    TMaterial(TShader& shader, DirectionalLight dl, Material mt, TextureId texId);

    void Bind(ID3D11DeviceContext* context);

    const TShader& mShader;
    TRenderInstanceData mrid;

    DirectionalLight dl;
    Material mt;
    TextureId diffuseTex;
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


/// <summary>
/// Contains other data like transform. Send these and Material data to correct RenderPass.  Also handle optimization like batch rendering
/// </summary>
class TMeshRenderer {
public:
    //MeshInstanced mMesh;
    H::Graphics::Mesh mMesh;
    H::Graphics::MeshBuffer mMeshBuffer;
};


/// <summary>
/// Produced from TMeshRenderer
/// </summary>
struct TRenderContext {
public:
    TMaterial& mat;
    H::Graphics::MeshBuffer& meshBuffer;
    // other data like transform
};


struct TInstanceData {
    UINT mTransformIdx;
    UINT mLightIdx;
    UINT mMaterialIdx;
    UINT mDiffuseMapIdx;
};

struct TDrawCommand {
    TMaterial* mat;
    H::Graphics::MeshBuffer* meshBuf;
    uint32_t numOfInstance;
    std::vector<H::Graphics::TransformData> tf;
};


/// <summary>
/// Interface. Each can have optional multiple input and 1 optional output RT. Inout are stored in Pipeline. 
/// Each pass handles 1 Shader to reduce bind cost
/// </summary>
class TRenderPass {
public:

    void Init();

    void Term();

    // make these pure v
    void execute();

    void add(TDrawCommand&& cmd);

    void clear();

    H::Graphics::TypedConstantBuffer<TRenderInstanceData> mRenderInstanceBuf;
    H::Graphics::TypedStructuredBuffer<TransformData, 100> mTransformBuf;
    std::unordered_map<TMaterial*, std::vector<TDrawCommand>> mDrawRequests;

};

/// <summary>
/// Manages all render pass.  And stored pipeline outputs
/// </summary>
class TRenderPipeline {
public:
    std::vector<TRenderPass*> mRPs;
    std::unordered_map<std::string, ID3D11ShaderResourceView*> mRPOutput;
};

/// <summary>
/// Draw 1 mesh with different data N times in 1 call
/// </summary>
struct TSampleInstancedRendering {
    static const size_t MAX_INSTANCE_COUNT = 10;

    std::unique_ptr<TShader> mShader;
    std::unique_ptr<TMaterial> mMaterial;
    std::unique_ptr<TMaterial> mMaterial2;
    TMeshRenderer mMeshRenderer;

    TextureId mDiffuseTex;
    TextureId mDiffuseTex2;
   // H::Graphics::LightBuffer lb;

    H::Graphics::TypedStructuredBuffer<TInstanceData, MAX_INSTANCE_COUNT> sbi;
    H::Graphics::TypedStructuredBuffer<TransformData, MAX_INSTANCE_COUNT> sbt;
    H::Graphics::TypedStructuredBuffer<Material, MAX_INSTANCE_COUNT> sbm;

    TRenderPass mRenderPass;

    void Init();
    void Term();
    void DrawWithRenderPass(const Camera& cam);

    void Draw(const Camera& cam);
};