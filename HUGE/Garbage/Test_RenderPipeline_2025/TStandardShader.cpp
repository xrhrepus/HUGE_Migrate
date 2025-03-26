#include "TStandardShader.h"

void TStandardShader::Init()
{
    mVs.Initialize(STANDARD_INSTANCED_FILE_PATH.wstring().c_str());
    mPs.Initialize(STANDARD_INSTANCED_FILE_PATH.wstring().c_str(), "PS");
    mMaterialsBuf.Initialize();
    mDirectionalLightsBuf.Initialize();
}
void TStandardShader::Term()
{
    mMaterialsBuf.Terminate();
    mDirectionalLightsBuf.Terminate();
    mVs.Terminate();
    mPs.Terminate();
}
void TStandardShader::Bind(ID3D11DeviceContext* context) const {
    mVs.Bind();
    mPs.Bind();

    // t8
    mDirectionalLightsBuf.BindVS(8);
    mDirectionalLightsBuf.BindPS(8);
    // t9
    mMaterialsBuf.BindPS(9);

}

const std::string& TStandardShader::getName() const
{
    return M_NAME;
}

TStandardMaterialInstanceData TStandardShader::addMatToShader(DirectionalLight& dl, Material& mt, TextureId texId) {
    TStandardMaterialInstanceData r;
    tempLight.emplace_back(dl);
    tempMat.emplace_back(mt);
    r.mLightIdx = tempLight.size() - 1;
    r.mMaterialIdx = tempMat.size() - 1;
    return r;
}

void TStandardShader::batchMaterialData() {
    mDirectionalLightsBuf.Set(*tempLight.data());
    mMaterialsBuf.Set(*tempMat.data());
}

TStandardMaterial::TStandardMaterial(const std::string& name, TStandardShader& shader, DirectionalLight dl, Material mt, TextureId texId)
    : TIMaterial(name, shader)
    , mStandardShader(shader)
    , mDirectionalLight(dl)
    , mMaterial(mt)
    , mDiffuseTex(texId)
{
    // data that can be stored in an array are stored in Shader
    // each material instance keeps indices of those data
    mMaterialInstanceIdx = shader.addMatToShader(dl, mt, texId);
}

void TStandardMaterial::Init() {
    materialInstanceBuf.Initialize();
    // These type of data won't change. Only set it once
    materialInstanceBuf.Set(mMaterialInstanceIdx);
}
void TStandardMaterial::Term() {
    materialInstanceBuf.Terminate();
}

void TStandardMaterial::Bind(ID3D11DeviceContext* context) const {
    // if not using formal tex2DArray, let material bind the texture they are using
    TextureManager::Get()->GetTexture(mDiffuseTex)->BindPS(0);

    // b10
    // this buffer is exclusive to each material instance 
    // so it has to be re-bound for each material
    materialInstanceBuf.BindPS(10);
}
