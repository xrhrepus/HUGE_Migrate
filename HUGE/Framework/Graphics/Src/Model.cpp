#include "Precompiled.h"
#include "TextureManager.h"
#include "Animation.h"
#include "Model.h"
#include "MeshIO.h"
#include "SamplerManager.h"
#include "RasterizerStateManager.h"
using namespace H::Graphics;

void Model::Initialize(std::filesystem::path infile)
{
	//load first
	MeshIO::LoadModel(*this,infile);
	//init
	for (auto& md :meshData)
	{
		md.meshBuffer.Initialize(md.mesh);
	}
	for (auto& md : materialData)
	{
		md.diffuseMapId = TextureManager::Get()->LoadTexture(md.diffuseMapName,false);
		md.specularMapId = TextureManager::Get()->LoadTexture(md.specularMapName, false);
		md.normalMapId = TextureManager::Get()->LoadTexture(md.normalMapName, false);
	}

}
void Model::Terminate()
{
	for (auto& md : meshData)
	{
		md.meshBuffer.Terminate();
	}
 
}

void Model::Render()
{
	//SamplerManager::Get()->GetSampler("LinearWrap")->BindPS();
	//RasterizerStateManager::Get()->GetRasterizerState("Solid")->Set();
	////RasterizerStateManager::Get()->GetRasterizerState("WireFrame")->Set();

	//for (auto& md : materialData)
	//{
	//	TextureManager::Get()->GetTexture(md.diffuseMapId)->BindPS(0);
	//	TextureManager::Get()->GetTexture(md.specularMapId)->BindPS(1);
	//	TextureManager::Get()->GetTexture(md.normalMapId)->BindPS(3);
 //	}
	for (auto& md : meshData)
	{
		md.meshBuffer.Render();
	}

	

}
