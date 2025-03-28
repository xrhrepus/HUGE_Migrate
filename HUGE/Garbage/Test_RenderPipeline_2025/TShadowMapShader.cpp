#include "TShadowMapShader.h"

void TShadowMapShader::Init()
{
    mVs.Initialize(STANDARD_INSTANCED_FILE_PATH.wstring().c_str());
    mPs.Initialize(STANDARD_INSTANCED_FILE_PATH.wstring().c_str(), "PS");
}

void TShadowMapShader::Term()
{
    mVs.Terminate();
    mPs.Terminate();
}

void TShadowMapShader::Bind(ID3D11DeviceContext* context) const
{
    mVs.Bind();
    mPs.Bind();
}

const std::string& TShadowMapShader::getName() const
{
    return M_NAME;
}
