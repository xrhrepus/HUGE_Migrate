#pragma once
#include <Graphics/Inc/Graphics.h>
#include <Graphics/Src/D3DUtil.h>

using namespace H::Graphics;

class TIShader {
public:
    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void Bind(ID3D11DeviceContext* context) const = 0;
    virtual const std::string& getName() const = 0;
};

class TIMaterial {
public:
    TIMaterial(const std::string& name, TIShader& shader);
    virtual void Init() = 0;
    virtual void Term() = 0;
    virtual void Bind(ID3D11DeviceContext* context) const = 0;
    const TIShader& GetShader() const;
    const std::string& getName() const;

protected:
    const std::string mName;
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
