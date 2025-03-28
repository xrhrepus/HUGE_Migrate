#pragma once

#include <Graphics/Inc/Graphics.h>
#include "TRenderPipelineInterface.h"

using namespace H::Graphics;


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
    void addRPOutput(const std::string& name, const H::Graphics::RenderTarget& outputRT);
    const H::Graphics::RenderTarget* getRPOutput(const std::string& name) const;

    std::unordered_map<std::string, std::unique_ptr<TIRenderPass>> mRPs;
    std::unordered_map<std::string, const H::Graphics::RenderTarget*> mRPOutput;
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
