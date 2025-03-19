#pragma once

#include <Graphics/Inc/Graphics.h>
#include <Graphics/Src/D3DUtil.h>

using namespace H::Graphics;

class TGameObject {
public:
};

struct ShaderConstantBuffer {
    std::string name;
    UINT size;
    UINT bindPoint;
};

struct ShaderResource {
    std::string name;
    UINT bindPoint;
};


/// <summary>
/// Shader reflect shader file info, and stored semantic names + slots. So only Material only bind necessary data
/// </summary>
class TShader {
public:
    TShader(const std::filesystem::path& path) {
        mVs.Initialize(path.wstring().c_str());
        mPs.Initialize(path.wstring().c_str(),"PS");
    }

    void Bind(ID3D11DeviceContext* context) const {
        mVs.Bind();
        mPs.Bind();
    }
    void ReflectConstantBuffers(ID3DBlob* shaderBlob);
    void ReflectResources(ID3DBlob* shaderBlob);

	VertexShader mVs;
	PixelShader mPs;
    std::vector<ShaderConstantBuffer> mConstantDatas;
    std::vector<ShaderResource> mTextures;
    std::vector<ShaderResource> mSamplers;
};


/// <summary>
/// Contain resource reference.
/// </summary>
class TMaterial {
public:
    TMaterial(const TShader& shader) 
        : mShader(shader) {
    }

    void SetTexture(const std::string& name, H::Graphics::TextureId texture) {
        textures[name] = texture;
    }

    // These should be data that are fixed. Like color.
    void SetConstantBuffer(const std::string& name, const H::Graphics::ConstantBuffer& buffer) {
        constantBuffers.emplace(name, buffer);
    }

    void Bind(ID3D11DeviceContext* context) {
        mShader.Bind(context);

        for (const auto& cb : mShader.mConstantDatas) {
            auto it = constantBuffers.find(cb.name);
            if (it != constantBuffers.end()) {
                it->second.get().BindVS(cb.bindPoint);
                it->second.get().BindPS(cb.bindPoint);
            }
        }

        for (const auto& tex : mShader.mTextures) {
            auto it = textures.find(tex.name);
            if (it != textures.end()) {
                TextureManager::Get()->GetTexture(it->second)->BindPS(0);
            }
        }

        // todo: sampler
    }


    const TShader& mShader;
    // Name in shader, texture
    std::unordered_map<std::string, H::Graphics::TextureId> textures;
    // Material holds the buffer, and use data from elsewhere
    std::unordered_map<std::string, std::reference_wrapper<const H::Graphics::ConstantBuffer>> constantBuffers;
    std::unordered_map<std::string, ID3D11SamplerState*> samplers;

    std::unordered_set<std::string> mNonSharedDataBufferName;
};

/// <summary>
/// Each object will own individual instance.  
/// Runtime data goes here, so render pass doesn't need to rebind shared part of material
/// </summary>
class TMaterialInstance {
public:
	TMaterialInstance(TMaterial& sourceMat) 
        : mSourceMat(sourceMat) {
    }

    TMaterial& mSourceMat;
    std::unordered_set<std::string> mOverrideData;
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


/// <summary>
/// Interface. Each can have optional multiple input and 1 optional output RT. Inout are stored in Pipeline. 
/// Each pass handles 1 Shader to reduce bind cost
/// </summary>
class TRenderPass {
public:
    void execute() {
        // shader bind


    }

    void add() {
        // meshbuffer, 
    }

    TShader* mShader;
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
    TMeshRenderer mMeshRenderer;

    H::Graphics::LightBuffer lb;

    H::Graphics::TypedStructuredBuffer<TInstanceData, MAX_INSTANCE_COUNT> sbi;
    H::Graphics::TypedStructuredBuffer<TransformData, MAX_INSTANCE_COUNT> sbt;
    H::Graphics::TypedStructuredBuffer<Material, MAX_INSTANCE_COUNT> sbm;

    void Init() {
        const wchar_t* STANDARD_INSTANCED_FILE_PATH = L"../../Assets/Shaders/StandardInstanced.fx";

        mShader = std::make_unique<TShader>(STANDARD_INSTANCED_FILE_PATH);
        mMaterial = std::make_unique<TMaterial>(*mShader);

        lb.Initialize();

        sbt.Initialize();
        sbm.Initialize();
        sbi.Initialize();

        auto m = MeshBuilder::CreateCube(5,5,5, { 0.0f,0.0f,0.0f });
        mMeshRenderer.mMesh = m;
        mMeshRenderer.mMeshBuffer.Initialize(mMeshRenderer.mMesh);

        mShader->mConstantDatas.push_back(ShaderConstantBuffer{ "LightBuffer",sizeof(H::Graphics::LightBuffer),1 });
        mMaterial->SetConstantBuffer("LightBuffer", lb);
    }
    void Term() {

    }
    void Draw(const Camera& cam) {
        SamplerManager::Get()->GetSampler(SamplerType::LinearWrap)->BindPS();
        lb.Set(H::Graphics::DirectionalLight{});

        Matrix4 vm = cam.GetViewMatrix();
        Matrix4 pm = cam.GetPerspectiveMatrix();
        Matrix4 worldMat;
        worldMat.Translation({ -10.0f,0.0f,0.0f });
        Matrix4 world = worldMat;
        Matrix4 comp = world * vm * pm;

        TInstanceData insDatas[10];
        Material mats[10];
        TransformData tfs[10];

        //{
        //    // something can be done be render pass
        //    Matrix4 worldMat1;
        //    worldMat1.Translation({ -20.0f,0.0f,0.0f });
        //    Matrix4 world1 = worldMat1;
        //    Matrix4 comp1 = world1 * vm * pm;
        //    TransformData tf1;
        //    tf1.world = H::Math::Transpose(world1);
        //    tf1.wvp = H::Math::Transpose(comp1);
        //    tf1.viewPosition = cam.GetPosition();

        //    Material m{};
        //    m.diffuse = H::Graphics::Colors::Green;

        //    insDatas[1] = { 1,1,1,1 };
        //    tfs[1] = (tf1);
        //    mats[1] = m;
        //}
        for (size_t i = 0; i < 10; i++)
        {
            Matrix4 worldMat1;
            worldMat1.Translation({ -20.0f * i,0.0f,0.0f });
            Matrix4 world1 = worldMat1;
            Matrix4 comp1 = world1 * vm * pm;
            TransformData tf1;
            tf1.world = H::Math::Transpose(world1);
            tf1.wvp = H::Math::Transpose(comp1);
            tf1.viewPosition = cam.GetPosition();

            Material m{};
            m.diffuse = Color(i, i, i, 1);

            UINT insDataIdx = static_cast<UINT>(i);
            insDatas[i] = { insDataIdx,insDataIdx,insDataIdx,insDataIdx };
            tfs[i] = (tf1);
            mats[i] = m;
        }


        sbt.Set(tfs[0]);
        sbm.Set(mats[0]);
        sbi.Set(insDatas[0]);
        
        lb.Set(H::Graphics::DirectionalLight{});

        sbi.BindVS(6);
        //sbi.BindPS(6);
        sbt.BindVS(7);
        //sbt.BindPS(7);
        //sbm.BindVS(8);
        sbm.BindPS(8);

        mMaterial->SetConstantBuffer("LightBuffer", lb);
        mMaterial->Bind(H::Graphics::GetContext());
        mMeshRenderer.mMeshBuffer.RenderInstanced(10);
    }
};