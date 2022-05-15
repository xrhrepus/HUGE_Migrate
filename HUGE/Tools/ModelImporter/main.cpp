#include <Graphics\Inc\Graphics.h>
#include <assimp\Importer.hpp> // c++ importer interface
#include <assimp\scene.h>//output data structure
#include <assimp\postprocess.h>//post processing flags
#include <cstdio>
#include "Graphics\Src\MeshIO.h"
struct Arguments
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
};
using namespace H::Graphics;
//..\..\Assets\Meshes\paladin_prop_j_nordstrom.fbx dedebug.txt
//..\..\Assets\Meshes\Goofy Running.fbx dedebug.txt
//..\Assets\Meshes\StumbleBackwards.fbx ..\Assets\Meshes\stumbleback
//..\..\Assets\Meshes\StumbleBackwards.fbx stumbleback
 using BoneIndexLookUp = std::map<std::string, int>;

inline H::Math::Vector3 Convert(const aiVector3D& v)
{
	return { v.x,v.y,v.z };
}

inline H::Math::Quaternion Convert(const aiQuaternion& q)
{
	return { q.x,q.y,q.z,q.w };
}
inline H::Math::Matrix4 Convert(const aiMatrix4x4& m)
{
	H::Math::Matrix4 mat = *(reinterpret_cast<const H::Math::Matrix4*>(&m));
	return mat.Transpose();
}

//check if a bone is in skeleton, exist return index, otws add in to skeleton
//bone has to have a name
int TryAddBone(const aiBone* inputBone, H::Graphics::Skeleton& skeleton , BoneIndexLookUp& boneIndexLookUp)
{

	std::string name = inputBone->mName.C_Str();
	ASSERT(!name.empty(), "[ModelImporter.exe] Error, bone must have a name");
	auto iter = boneIndexLookUp.find(name);
	if (iter != boneIndexLookUp.end())
		return iter->second;

	//add new bone
	auto& newBone =  skeleton.bones.emplace_back(std::make_unique<H::Graphics::Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = Convert(inputBone->mOffsetMatrix);

	//cache bone index
	boneIndexLookUp.emplace(newBone->name, newBone->index);
	return newBone->index;
}

//recursively walk aiscene tree and add/link bones to our skeleton as we find them 
H::Graphics::Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookUp& boneIndexLookUp)
{
	Bone* bone = nullptr;

	std::string name = sceneNode.mName.C_Str();
	auto iter = boneIndexLookUp.find(name);
	if (iter != boneIndexLookUp.end())
	{
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Matrix4::Identity();//might break
		if (name.empty())
		{
			bone->name = "NoName" + std::to_string(bone->index);
		}
		else
		{
			bone->name = std::move(name);
		}

		boneIndexLookUp.emplace(bone->name, bone->index);
	}

	bone->parent = parent;
	bone->parentIndex = parent == nullptr ? -1 : parent->index;
	bone->toParentTransform = Convert(sceneNode.mTransformation);

	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone * child = BuildSkeleton(*sceneNode.mChildren[i], bone, skeleton, boneIndexLookUp);
		bone->children.push_back(child);
		
	}
	return bone;
}

//bool ParseArgs(int argc, char* argv[], Arguments& args)
//{
//
//}


std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	//need at least 3
	if (argc < 3)
	{
		return std::nullopt;// an option has nothing
	}
	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	return args;
}
void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage: \n"
		"	ModelImporter.exe [Options] <InputFile> <OutputFile> \n"
		"\n"
		"Options: \n"
		"\n"
		"	<None>\n"
		"\n"
	);
}

std::string aiTextureType_ToString(aiTextureType texturetype)
{
	switch (texturetype)
	{

	case aiTextureType_DIFFUSE: return  std::string("_diffuse_"); break;
	case aiTextureType_SPECULAR: return  std::string("_specular_"); break;
	case aiTextureType_NORMALS: return  std::string("_normal_"); break;
	default: return std::string(); break;
	}
	return std::string();
}

std::string FindTexture(const Arguments& fileNames, const aiScene* scene, aiTextureType texturetype , const aiMaterial* inputMaterial)
{
	const uint32_t textureCount = inputMaterial->GetTextureCount(texturetype);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (inputMaterial->GetTexture(
			texturetype, 0, &texturePath) == aiReturn_SUCCESS)
		{
			//if texture path starts with *, tex is embeded
			if (auto inputTexture = scene->GetEmbeddedTexture(texturePath.C_Str()))
			{
				std::string fileName = fileNames.inputFileName;

				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += aiTextureType_ToString(texturetype);
				//fileName += texturePath.C_Str()[1];

				ASSERT(scene->HasTextures(), "[ModelImporter.exe] Error : no embeded texture found.");
				int textureIndex = atoi(texturePath.C_Str() + 1);
				ASSERT(textureIndex < (int)scene->mNumTextures, "[ModelImporter.exe] Error : invalid texture index.");

				//const aiTexture* inputTexture = scene->mTextures[textureIndex];
				ASSERT(inputTexture->mHeight == 0, "[ModelImporter.exe] Error : uncompressed texture found.");

				if (inputTexture->CheckFormat("jpg"))
					fileName += ".jpg";
				else if (inputTexture->CheckFormat("png"))
					fileName += ".png";
				else
				{
					ASSERT(inputTexture->mHeight == 0, "[ModelImporter.exe] Error : unrecognized texture format.");
					return std::string();
				}
				printf("[ModelImporter.exe] extracting embedded texture %s \n", fileName.c_str());

				std::string fullfilename = fileNames.outputFileName;
				fullfilename = fullfilename.substr(0, fullfilename.rfind('/') + 1);
				fullfilename += fileName;

				FILE* file = nullptr;
				fopen_s(&file, fullfilename.c_str(), "wb");
				size_t written = fwrite(inputTexture->pcData, 1, inputTexture->mWidth, file);
				ASSERT(written == inputTexture->mWidth, "[ModelImporter.exe] Error : failed to extract embedded texture.");
				fclose(file);

				printf("[ModelImporter.exe] adding texture %s , at %s \n", fileName.c_str(), fullfilename.c_str());

				//model.materialData[i].diffuseMapName = (std::move(fileName));
				return fileName;

			}
			//else
			//{
			//	std::filesystem::path filePath = texturePath.C_Str();
			//	std::string fileName = filePath.filename().u8string();
			//	printf("[ModelImporter.exe] adding texture %s \n", fileName.c_str());


			//	//model.textureNames.emplace_back(fileName.c_str());
			//	//model.materialData[i].diffuseMapName = fileName.c_str();
			//	return fileName;
			//}
		}


	}
	return std::string();
}

void SaveModel(const Arguments& args, const Model& model)
{
	std::filesystem::path path = args.outputFileName;

	path.replace_extension("model");

 
	H::Graphics::MeshIO::SaveModel(model, path);

}

void SaveSkeleton(const Arguments& args, const Skeleton& skeleton)
{
	std::filesystem::path path = args.outputFileName;

	path.replace_extension("skeleton");
 
	H::Graphics::MeshIO::SaveSkeleton(skeleton, path);

}
void SaveBoneAnimation(const Arguments& args, const AnimationClips& animClips)
{
	std::filesystem::path path = args.outputFileName;

	path.replace_extension("animset");
 
	H::Graphics::MeshIO::SaveAnimationClips(animClips, path);

}



int main(int argc, char* argv[])
{
 
	const auto argOpt = ParseArgs(argc, argv);
	if (!argOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argOpt.value();

	//create an instance of importer to do parse
	Assimp::Importer importer;

	//try import model into scene
	const aiScene* scene = importer.ReadFile(args.inputFileName, 
		aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);

	if (scene == nullptr)
	{
		printf("[ModelImporter.exe] Error: %s\n" , importer.GetErrorString());
		return -1;
	}
 
	//
	H::Graphics::Model model;
	H::Graphics::Skeleton skeleton;
	BoneIndexLookUp boneIndexLookUp;
	AnimationClips animationClips;
	//look for mesh data
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");
		uint32_t numMeshes = scene->mNumMeshes;
		model.meshData.resize(numMeshes);
		printf("num of mesh %d\n",numMeshes);

 
		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			printf("Start reading mesh [%d] \n", meshIndex);

			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			
			uint32_t numVertices  = inputMesh->mNumVertices;
			uint32_t numIndecices  = inputMesh->mNumFaces * 3;
			printf("num of vert %d\n", numVertices);
			printf("num of inde %d\n", numIndecices);

			printf("Reading vertices... \n");

			std::vector<H::Graphics::BoneVertex> vertecies(numVertices);
			//std::vector<uint32_t> indices(numIndecices);
			std::vector<uint32_t> indices;
			indices.reserve(numIndecices);


			const aiVector3D* positions = inputMesh->mVertices;
			printf("has positions: [%s] ,", positions ? "true" : "false");
			const aiVector3D* normals = inputMesh->mNormals;
			printf("has normals: [%s] ,", normals ? "true" : "false");
			const aiVector3D* tangents = inputMesh->mTangents;
			printf("has tangents: [%s] ,", tangents ? "true" : "false");
			const aiVector3D* texcoords = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;
			printf("has texcoords: [%s] \n", texcoords ? "true" : "false");

			//get all pointers

			for (size_t i = 0; i < numVertices; ++i)
			{
				Vector3 pos = positions ? Vector3{positions[i].x, positions[i].y, positions[i].z} : Vector3{0.0f};
				Vector3 nor = normals ? Vector3{ normals[i].x, normals[i].y, normals[i].z } : Vector3{ 0.0f };
				Vector3 tan = tangents ? Vector3{ tangents[i].x, tangents[i].y, tangents[i].z } : Vector3{ 0.0f };
				Vector2 tex = texcoords ? Vector2{ texcoords[i].x, texcoords[i].y} : Vector2{ 0.0f };

				H::Graphics::BoneVertex v{ pos ,nor,tan,tex };



				//H::Graphics::BoneVertex v{ 
				//	{positions[i].x, positions[i].y, positions[i].z} ,
				//	{normals[i].x, normals[i].y, normals[i].z},
				//	{tangents[i].x, tangents[i].y, tangents[i].z},
				//	{texcoords[i].x, texcoords[i].y},
				//};
 
				vertecies[i] = std::move(v);
 
			}
			for (size_t j = 0; j < inputMesh->mNumFaces; j++)
			{
				const aiFace& faces = (inputMesh->mFaces)[j];
				size_t nofi = faces.mNumIndices;

				for (size_t i = 0; i < nofi; i++)
				{
					indices.push_back (faces.mIndices[i]);
				}

			}

			if (inputMesh->HasBones())
			{
				printf("reading bone weights\n");

				//num of weight added to each vertice so far
				//at most 4
				std::vector<int> numWeights(vertecies.size(), 0);

				for (uint32_t meshboneIndex = 0; meshboneIndex < inputMesh->mNumBones; ++meshboneIndex)
				{
					printf("reading bone %d\n", meshboneIndex);

					aiBone* inputBone = inputMesh->mBones[meshboneIndex];
					int boneIndex = TryAddBone(inputBone, skeleton, boneIndexLookUp);

					for (uint32_t weightIndex = 0; weightIndex < inputBone->mNumWeights; ++weightIndex)
					{
						const aiVertexWeight& weight = inputBone->mWeights[weightIndex];
						auto& vertex = vertecies[weight.mVertexId];
						auto& count = numWeights[weight.mVertexId];

						if (count < 4)
						{
							vertex.blendIndices[count] = boneIndex;
							vertex.blendWeights[count] = weight.mWeight;
							++count;
						}
					}
				}

			}

 
			H::Graphics::BoneMesh mesh;
			mesh.vertices = std::move(vertecies);
			mesh.indices =  std::move(indices);
			
			//store into model
			model.meshData[meshIndex].mesh = std::move(mesh);
			model.meshData[meshIndex].materialIndex = inputMesh->mMaterialIndex;


  			//save
			//H::Graphics::MeshIO::Save(mesh, args.outputFileName);
 
			//model.meshes.push_back(mesh);
		}
	}

	//look for material data
	if (scene->HasMaterials())
	{
		printf("Reading materials... \n");
		const size_t numMaterial = scene->mNumMaterials;

		model.materialData.resize(numMaterial);
		printf("num of materials %d\n", numMaterial);

		for (uint32_t i = 0; i < numMaterial; ++i)
		{

			const aiMaterial* inputMaterial = scene->mMaterials[i];
			//diffusemap

			//write 2 below into txt file and load in app
			model.materialData[i].diffuseMapName = FindTexture(args, scene, aiTextureType_DIFFUSE, inputMaterial);
			model.materialData[i].specularMapName = FindTexture(args, scene, aiTextureType_SPECULAR, inputMaterial);
			model.materialData[i].normalMapName = FindTexture(args, scene, aiTextureType_NORMALS, inputMaterial);

		}
	}

	//save
	//H::Graphics::MeshIO::SaveModel(model, args.outputFileName);


	//skeleton infos
	if (!skeleton.bones.empty())
	{
		printf("building skeleton... \n");
		BuildSkeleton(*scene->mRootNode, nullptr, skeleton, boneIndexLookUp);

	}

	//add a save skeleton
	//look for Animation data
	if (scene->HasAnimations())
	{
		printf("Reading animations... \n");
		const size_t numAnimation = scene->mNumAnimations;
		printf("num of animations %d\n", numAnimation);

		for (uint32_t animindex = 0; animindex < numAnimation; ++animindex)
		{
			const aiAnimation* inputAnim = scene->mAnimations[animindex];
			auto& clip = animationClips.emplace_back(std::make_unique<AnimationClip>());

			if (inputAnim->mName.length > 0)
				clip->name = inputAnim->mName.C_Str();
			else
				clip->name = "Anim" + std::to_string(animindex);

			clip->duration = static_cast<float>(inputAnim->mDuration);
			clip->tickPerSecond = static_cast<float>(inputAnim->mTicksPerSecond);

			printf("reading animation for %s ...", clip->name.c_str());
			//not all bone has animation
			//resize to enable index look up
			clip->boneAnimations.resize(skeleton.bones.size());

			for (uint32_t boneAnimIndex = 0; boneAnimIndex < inputAnim->mNumChannels; ++boneAnimIndex)
			{
				const aiNodeAnim* inputBoneAnim = inputAnim->mChannels[boneAnimIndex];

				int slotindex = boneIndexLookUp[inputBoneAnim->mNodeName.C_Str()];
				
				auto& boneAnim = clip->boneAnimations[slotindex];
				boneAnim = std::make_unique<BoneAnimation>();
				boneAnim->boneName = inputBoneAnim->mNodeName.C_Str();

				
				AnimationBuilder ab;

				for (uint32_t  keyIndex = 0; keyIndex < inputBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mPositionKeys[keyIndex];
					ab.AddPositionKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mRotationKeys[keyIndex];
					ab.AddRotationKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mScalingKeys[keyIndex];
					ab.AddScaleKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				
				boneAnim->animation = ab.Build();
				//add bone anim to correspond slot

				//clip->boneAnimations[slotindex] = std::move(boneAnim);
			}

			
		}



	}
	
	//save anime here
	if (scene->HasMeshes() && scene->HasMaterials())
 		SaveModel(args, model);

 	if (!skeleton.bones.empty())
		SaveSkeleton(args, skeleton);

	if (scene->HasAnimations())
		SaveBoneAnimation(args, animationClips);

 	printf("All done !\n");
	return 0;
}