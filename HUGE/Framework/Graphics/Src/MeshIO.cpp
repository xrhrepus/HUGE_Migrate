#include "Precompiled.h"
#include "MeshIO.h"
#include "Graphics.h"
#include <fstream>
#include <sstream>

using namespace H::Graphics;
namespace
{

}
bool MeshIO::Save(const BoneMesh& mesh, std::filesystem::path outfile)
{
	printf("saving \n");
	std::fstream of;
	of.open(outfile,std::fstream::out | std::fstream::app);
	printf("s vert %d\n", mesh.vertices.size());
	printf("s inde %d\n", mesh.indices.size());

	const auto& v = mesh.vertices;
	const auto& idc = mesh.indices;

	const size_t vs = v.size();
	const size_t is = idc.size();
	of  << vs << "\n";
	of  << is << "\n";
 
	for (size_t i = 0; i < vs; ++i)
	{
		//11 float
		of << v[i].position.x << ",";
		of << v[i].position.y << ",";
		of << v[i].position.z << ",";
 
		of << v[i].normal.x << ",";
		of << v[i].normal.y << ",";
		of << v[i].normal.z << ",";
 
		of << v[i].tangent.x << ",";
		of << v[i].tangent.y << ",";
		of << v[i].tangent.z << ",";
 
		of << v[i].texCoord.x << ",";
		of << v[i].texCoord.y << ",";

		//more

		of << "\n";

	}

 
	for (size_t i = 0; i < is; ++i)
	{
		of << idc[i] << ",";
	}

	of << "\n";

	of.close();
	return true;
}
bool MeshIO::Load(BoneMesh& mesh, std::filesystem::path infile)
{
	std::ifstream fs;
	fs.open(infile);

	std::string line;
	std::stringstream token;

	uint32_t noVert = 0;
	uint32_t noIndex = 0;




	if (fs.is_open())
	{
		std::string temp;

		//numVert/index
		std::getline(fs, line);
		token = std::stringstream(line);
 		token >> noVert;
		std::getline(fs, line);
		token = std::stringstream(line);
		token >> noIndex;

		std::vector<uint32_t> tempIndices(noIndex);
		std::vector<H::Graphics::BoneVertex> tempVertecies(noVert);

		//read all vertex
		for (size_t i = 0; i < noVert; i++)
		{
			//11 floats / line
			std::getline(fs, line);
			token = std::stringstream(line);
			BoneVertex tempVert;

			std::getline(token, line , ',');
			tempVert.position.x = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.position.y = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.position.z = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.normal.x = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.normal.y = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.normal.z = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.tangent.x = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.tangent.y = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.tangent.z = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.texCoord.x = static_cast<float>(atof(line.c_str()));
			std::getline(token, line, ',');
			tempVert.texCoord.y = static_cast<float>(atof(line.c_str()));

			//token >> tempVert.position.y;
			//token >> tempVert.position.z;
			//token >> tempVert.normal.x;
			//token >> tempVert.normal.y;
			//token >> tempVert.normal.z;
			//token >> tempVert.tangent.x;
			//token >> tempVert.tangent.y;
			//token >> tempVert.tangent.z;
			//token >> tempVert.texCoord.x;
			//token >> tempVert.texCoord.y;
			//
			tempVertecies[i] = std::move(tempVert);
		}
		//read index
		std::getline(fs, line);
		token = std::stringstream(line);
 
		for (size_t i = 0; i < noIndex; i++)
		{
			std::getline(token, line, ',');

			tempIndices[i] = static_cast<uint32_t>(atof(line.c_str()));
		}

		//construct Mesh
 		mesh.vertices = std::move(tempVertecies);
		mesh.indices = std::move(tempIndices);

	}

	fs.close();
	return true;
}

bool H::Graphics::MeshIO::Load(std::vector<H::Graphics::BoneMesh>& meshs, std::filesystem::path infile)
{
	std::ifstream fs;
	fs.open(infile);

	std::string line;
	std::stringstream token;

	uint32_t noVert = 0;
	uint32_t noIndex = 0;


	if (fs.is_open())
	{

		std::string temp;
		
		while (!fs.eof())
		{
			
			//numVert/index
			std::getline(fs, line);
			token = std::stringstream(line);
			token >> noVert;
			std::getline(fs, line);
			token = std::stringstream(line);
			token >> noIndex;

			std::vector<uint32_t> tempIndices(noIndex);
			std::vector<H::Graphics::BoneVertex> tempVertecies(noVert);

			//read all vertex
			for (size_t i = 0; i < noVert; i++)
			{
				//11 floats / line
				std::getline(fs, line);
				token = std::stringstream(line);
				BoneVertex tempVert;

				std::getline(token, line, ',');
				tempVert.position.x = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.position.y = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.position.z = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.normal.x = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.normal.y = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.normal.z = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.tangent.x = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.tangent.y = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.tangent.z = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.texCoord.x = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');
				tempVert.texCoord.y = static_cast<float>(atof(line.c_str()));

				tempVertecies[i] = std::move(tempVert);
			}
			//read index
			std::getline(fs, line);
			token = std::stringstream(line);

			for (size_t i = 0; i < noIndex; i++)
			{
				std::getline(token, line, ',');

				tempIndices[i] = static_cast<uint32_t>(atof(line.c_str()));
			}
			BoneMesh mesh;
			//construct Mesh
			mesh.vertices = std::move(tempVertecies);
			mesh.indices = std::move(tempIndices);

			meshs.push_back(std::move(mesh));
		}

	}

	fs.close();
	return true;
}


bool MeshIO::SaveModel(const H::Graphics::Model& model, std::filesystem::path outfile)
{
	printf("saving \n");
	std::fstream of;
	of.open(outfile, std::fstream::out /*| std::fstream::app*/);
	
	{
		printf("start saving meshes ...\n");
		const auto& meshdata = model.meshData;
		const size_t numMeshes = meshdata.size();
		if (numMeshes > 0)
		{
			printf("num of meshes %d\n", numMeshes);
			of << "Num_of_Mesh: " << numMeshes << "\n";//Num_of_Mesh:

			for (size_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
			{
				printf("saving mesh %d ...\n", meshIndex);

				auto& rVertex = meshdata[meshIndex].mesh.vertices;
				const size_t numVert = rVertex.size();
				printf("num of vertices %d\n", numVert);

				of << "Material_Index: " << meshdata[meshIndex].materialIndex << "\n";//Material_Index:
				of << "Num_of_Vertices: " << numVert << "\n";//Num_of_Vertices:

				for (size_t i = 0; i < numVert; i++)
				{
					//11 float
					of << rVertex[i].position.x << ",";
					of << rVertex[i].position.y << ",";
					of << rVertex[i].position.z << ",";
					of << rVertex[i].normal.x << ",";
					of << rVertex[i].normal.y << ",";
					of << rVertex[i].normal.z << ",";
					of << rVertex[i].tangent.x << ",";
					of << rVertex[i].tangent.y << ",";
					of << rVertex[i].tangent.z << ",";
					of << rVertex[i].texCoord.x << ",";
					of << rVertex[i].texCoord.y << ",";

					for (size_t blendIdx = 0; blendIdx < 4; blendIdx++)
					{
						of << rVertex[i].blendIndices[blendIdx] << ",";
						of << rVertex[i].blendWeights[blendIdx] << ",";
					}


					of << "\n";
				}
				auto& rIndex = meshdata[meshIndex].mesh.indices;
				const size_t numIndex = rIndex.size();
				printf("num of indices %d\n", numIndex);

				of << "Num_of_Indices: " << numIndex << "\n";//Num_of_Indices:

				for (size_t i = 0; i < numIndex; ++i)
				{
					of << rIndex[i] << ",";
				}

				of << "\n";
			}


		}
	}

	printf("start saving materials ...\n");
	const auto& materialData = model.materialData;
	const size_t numMaterials = materialData.size();
	if (numMaterials > 0)
	{
		printf("num of materials %d\n", numMaterials);
		of << "Num_of_Materials: " << numMaterials << "\n";//Num_of_Materials:

		for (size_t i = 0; i < numMaterials; i++)
		{
			of << "DiffuseMap: " << materialData[i].diffuseMapName << "\n";
			of << "SpecularMap: " << materialData[i].specularMapName << "\n";
			of << "NormalMap: " << materialData[i].normalMapName << "\n";
		}
	}

	of << "\n";
	of.close();
	return true;
}


bool MeshIO::LoadModel(H::Graphics::Model& model, std::filesystem::path fromfile)
{
	std::ifstream fs;
	fs.open(fromfile);

	std::string line;
	std::stringstream token;


	if (fs.is_open())
	{
		std::string temp;
		uint32_t numMesh = 0;

		if (!fs.eof())
		{
			std::getline(fs, line); //Num_of_Mesh:
			token = std::stringstream(line);
			token >> temp;
			token >> numMesh;

 			auto& meshData = model.meshData;
			meshData.resize(numMesh);

			size_t numVert = 0;
			for (size_t meshIndex = 0; meshIndex < numMesh; ++meshIndex)
			{
				//Material_Index:
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> meshData[meshIndex].materialIndex;

				//Num_of_Vertices:
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> numVert;

				//read vertices info
				std::vector<BoneVertex> tempVertecies;
				tempVertecies.resize(numVert);

				for (size_t j = 0; j < numVert; ++j)
				{
					//11 floats / line
					std::getline(fs, line);
					token = std::stringstream(line);
					BoneVertex tempVert;

					std::getline(token, line, ',');
					tempVert.position.x = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.position.y = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.position.z = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.normal.x = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.normal.y = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.normal.z = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.tangent.x = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.tangent.y = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.tangent.z = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.texCoord.x = static_cast<float>(atof(line.c_str()));
					std::getline(token, line, ',');
					tempVert.texCoord.y = static_cast<float>(atof(line.c_str()));

					//[idx,weight] * 4
					for (size_t bidx = 0; bidx < 4; bidx++)
					{
						std::getline(token, line, ',');
						tempVert.blendIndices[bidx] = static_cast<float>(atof(line.c_str()));
						std::getline(token, line, ',');
						tempVert.blendWeights[bidx] = static_cast<float>(atof(line.c_str()));
					}

 					tempVertecies[j] = std::move(tempVert);
				}

				//read index
				size_t numIndices = 0;

				//Num_of_Indices:
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> numIndices;
				std::vector<uint32_t> tempIndices;
				tempIndices.resize(numIndices);

				//get the line of indices
				std::getline(fs, line);
				token = std::stringstream(line);
				for (size_t i = 0; i < numIndices; i++)
				{
					std::getline(token, line, ',');
					tempIndices[i] = static_cast<uint32_t>(atof(line.c_str()));
				}

				//save mesh data
				meshData[meshIndex].mesh.vertices = std::move(tempVertecies);
				meshData[meshIndex].mesh.indices = std::move(tempIndices);

			}
			//read materials

			size_t numMaterials = 0;
			//Num_of_Materials:
			std::getline(fs, line);
			token = std::stringstream(line);
			token >> temp;
			token >> numMaterials;
			model.materialData.resize(numMaterials);
			//material data
			for (size_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
			{
				//DiffuseMap
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> model.materialData[materialIndex].diffuseMapName;

				//SpecularMap
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> model.materialData[materialIndex].specularMapName;

				//NormalMap
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> model.materialData[materialIndex].normalMapName;

			}

 		}

	}

	fs.close();
	return true;

}

bool H::Graphics::MeshIO::SaveSkeleton(const H::Graphics::Skeleton & skeleton, std::filesystem::path outfile)
{
	printf("saving skeleton.. \n");
	std::fstream of;
	of.open(outfile, std::fstream::out /*| std::fstream::app*/);

	{
		const auto& bones = skeleton.bones;
		const size_t numBones = bones.size();
		of << "NumBones: " << numBones << "\n";
		//of << "RootIndex: " << skeleton.root->index << "\n";

		for (size_t boneIdx = 0; boneIdx < numBones; ++boneIdx)
		{
			const auto& currBone = bones[boneIdx];

			of << currBone->name << ","	<< currBone->index << ","<< currBone->parentIndex << "," << "\n";

			of << "NumChildren: " << currBone->children.size() << "\n";
			for (size_t childidx = 0; childidx < currBone->children.size(); childidx++)
			{
				of << currBone->children[childidx]->index << ",";
			}
			of << "\n";

			of  << currBone->toParentTransform._11 << ","
				<< currBone->toParentTransform._12 << ","
				<< currBone->toParentTransform._13 << ","
				<< currBone->toParentTransform._14 << ","
				<< currBone->toParentTransform._21 << ","
				<< currBone->toParentTransform._22 << ","
				<< currBone->toParentTransform._23 << ","
				<< currBone->toParentTransform._24 << ","
				<< currBone->toParentTransform._31 << ","
				<< currBone->toParentTransform._32 << ","
				<< currBone->toParentTransform._33 << ","
				<< currBone->toParentTransform._34 << ","
				<< currBone->toParentTransform._41 << ","
				<< currBone->toParentTransform._42 << ","
				<< currBone->toParentTransform._43 << ","
				<< currBone->toParentTransform._44 << ","
				<< "\n";

			of 	<< currBone->offsetTransform._11 << "," 
				<< currBone->offsetTransform._12 << "," 
				<< currBone->offsetTransform._13 << "," 
				<< currBone->offsetTransform._14 << "," 
				<< currBone->offsetTransform._21 << "," 
				<< currBone->offsetTransform._22 << "," 
				<< currBone->offsetTransform._23 << "," 
				<< currBone->offsetTransform._24 << "," 
				<< currBone->offsetTransform._31 << "," 
				<< currBone->offsetTransform._32 << "," 
				<< currBone->offsetTransform._33 << "," 
				<< currBone->offsetTransform._34 << "," 
				<< currBone->offsetTransform._41 << "," 
				<< currBone->offsetTransform._42 << "," 
				<< currBone->offsetTransform._43 << "," 
				<< currBone->offsetTransform._44 << "," 
				<< "\n";
		}

	}

	of.close();
	return true;
}

bool H::Graphics::MeshIO::LoadSkeleton(H::Graphics::Skeleton& skeleton, std::filesystem::path fromfile)
{
	std::ifstream fs;
	fs.open(fromfile);

	std::string line;
	std::stringstream token;

	//Skeleton retSkeleton;

	if (fs.is_open())
	{
		std::string temp;
		uint32_t numBone = 0;
		uint32_t rootIndex = 0;
		if (!fs.eof())
		{
			std::getline(fs, line); //Num_of_bones:
			token = std::stringstream(line);
			token >> temp;
			token >> numBone;
 

			Bone* bone = nullptr;
			for (size_t i = 0; i < numBone; i++)
			{
				bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
				std::getline(fs, line);
				token = std::stringstream(line);
 
				std::getline(token, line, ',');
				bone->name = line.c_str();

				std::getline(token, line, ',');
				bone->index = atoi(line.c_str());

				std::getline(token, line, ',');
				bone->parentIndex = atoi(line.c_str());
				if (bone->parentIndex == -1)
				{
					skeleton.root = bone;
				}
				//else
				//{
				//	bone->parent = skeleton.bones[bone->parentIndex].get();
				//	bone->parent->children.emplace_back(bone);
				//}

				//num of child
				size_t numChildren = 0;
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> numChildren;

				//children index
				std::getline(fs, line);
				token = std::stringstream(line);
				for (size_t ci = 0; ci < numChildren; ci++)
				{
					std::getline(token, line, ',');
					bone->childIndices.emplace_back(atoi(line.c_str()));
				}
				//toparent
				std::getline(fs, line);
				token = std::stringstream(line);

				std::getline(token, line, ',');	bone->toParentTransform._11 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._12 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._13 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._14 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._21 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._22 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._23 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._24 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._31 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._32 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._33 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._34 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._41 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._42 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._43 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->toParentTransform._44 = static_cast<float>(atof(line.c_str()));


				//offset
				std::getline(fs, line);
				token = std::stringstream(line);

				std::getline(token, line, ',');	bone->offsetTransform._11 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._12 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._13 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._14 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._21 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._22 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._23 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._24 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._31 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._32 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._33 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._34 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._41 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._42 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._43 = static_cast<float>(atof(line.c_str()));
				std::getline(token, line, ',');	bone->offsetTransform._44 = static_cast<float>(atof(line.c_str()));
 
			}
 
		}

		fs.close();

		//connect parent and children

		for (size_t i = 0; i < skeleton.bones.size(); i++)
		{
			Bone* currbone = skeleton.bones[i].get();

			currbone->parent = currbone->parentIndex > -1 ? skeleton.bones[currbone->parentIndex].get() : nullptr;
			for (size_t j = 0; j < currbone->childIndices.size(); j++)
			{
				currbone->children.emplace_back(skeleton.bones[currbone->childIndices[j]].get());
			}
		}

		return true;
	}
 
	return false;
}

bool H::Graphics::MeshIO::SaveAnimationClips(const H::Graphics::AnimationClips& animClips, std::filesystem::path outfile)
{
	printf("saving bone animation.. \n");
	std::fstream of;
	of.open(outfile, std::fstream::out);

  	{
		//numClips
		const size_t numClips = animClips.size();
		of << "NumClips: " << numClips << "\n";

		for (size_t clipIdx = 0; clipIdx < numClips; clipIdx++)
		{
			const auto& currClip =  animClips[clipIdx];


			// n d t
			of << "ClipName: " << currClip->name << "\n";
			of << "Duration: " << currClip->duration << "\n";
			of << "TickPerSecond: " << currClip->tickPerSecond << "\n";

			//bone anims
			const auto& boneAnims = currClip->boneAnimations;
			const size_t maxBoneAnim = boneAnims.size();
			size_t numBoneAnim = 0;

			for (size_t noaIdx = 0; noaIdx < boneAnims.size(); noaIdx++)
			{
				if (boneAnims[noaIdx] != nullptr)
				{
					numBoneAnim++;
				}
			}
			
			of << "MaxBoneAnim: " << maxBoneAnim << "\n";
			of << "NumBoneAnims: " << numBoneAnim << "\n";

			for (size_t baIndex = 0; baIndex < maxBoneAnim; ++baIndex)
			{
				const auto& currAnim = boneAnims[baIndex];
				if (currAnim == nullptr)
				{
					continue;
				}

				of << "BoneIndex: " << baIndex << "\n";
				of << "BoneName: " << currAnim->boneName << "\n";

				//position
				const size_t numPosKey = currAnim->animation.mPositions.size();
				of << "NumPositions: " << numPosKey << "\n";
				for (size_t posIndex = 0; posIndex < numPosKey; ++posIndex)
				{
					//vec3 , time
					of << currAnim->animation.mPositions[posIndex].key.x << ",";
					of << currAnim->animation.mPositions[posIndex].key.y << ",";
					of << currAnim->animation.mPositions[posIndex].key.z << ",";
					of << currAnim->animation.mPositions[posIndex].time << "\n";
				}
				//rotation
				const size_t numRotKey = currAnim->animation.mRotations.size();
				of << "NumRotations: " << numRotKey << "\n";
				for (size_t rotIndex = 0; rotIndex < numRotKey; ++rotIndex)
				{
					//quat xyzw , time
					of << currAnim->animation.mRotations[rotIndex].key.x << ",";
					of << currAnim->animation.mRotations[rotIndex].key.y << ",";
					of << currAnim->animation.mRotations[rotIndex].key.z << ",";
					of << currAnim->animation.mRotations[rotIndex].key.w << ",";
					of << currAnim->animation.mRotations[rotIndex].time << "\n";
				}
				//scale
				const size_t numScalKey = currAnim->animation.mScales.size();
				of << "NumScales: " << numScalKey << "\n";
				for (size_t scaIndex = 0; scaIndex < numScalKey; ++scaIndex)
				{
					//vec3 , time
					of << currAnim->animation.mScales[scaIndex].key.x << ",";
					of << currAnim->animation.mScales[scaIndex].key.y << ",";
					of << currAnim->animation.mScales[scaIndex].key.z << ",";
					of << currAnim->animation.mScales[scaIndex].time << "\n";
				}

			}

		}
 

 
	}

	of.close();
	printf("bone animation done \n");

	return true;

}

bool H::Graphics::MeshIO::LoadAnimationClips(H::Graphics::AnimationClips& animClips, std::filesystem::path fromfile)
{
	std::ifstream fs;
	fs.open(fromfile);
	
	std::string line;
	std::stringstream token;
  
	if (fs.is_open())
	{
		std::string temp;

		uint32_t numClips = 0;

		
		if (!fs.eof())
		{
			//numClips
			std::getline(fs, line);
			token = std::stringstream(line);
			token >> temp;
			token >> numClips;

			animClips.resize(numClips);

			for (size_t clipIdx = 0; clipIdx < numClips; clipIdx++)
			{
				auto& currClip = animClips[clipIdx] = std::make_unique<AnimationClip>();
				//c d t
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> currClip->name;
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> currClip->duration;
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> currClip->tickPerSecond;


				size_t maxBoneAnim = 0;
				// max BoneAnims
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> maxBoneAnim;

				uint32_t numBoneAnim = 0;
				//numBoneAnims
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> numBoneAnim;

				auto& boneAnims = currClip->boneAnimations;
				boneAnims.resize(maxBoneAnim);

				for (size_t bai = 0; bai < numBoneAnim; ++bai)
				{
					//boneanim index
					size_t baIdx = 0;
					std::getline(fs, line);
					token = std::stringstream(line);
					token >> temp;
					token >> baIdx;
					const auto& currBA = boneAnims[baIdx] = std::make_unique<BoneAnimation>();
					//boneanim name
					std::getline(fs, line);
					token = std::stringstream(line);
 					token >> temp;
					token >> currBA->boneName;

					//boneanim anim 3 channels
					//numPos
					{
						size_t numPos = 0;
						std::getline(fs, line);
						token = std::stringstream(line);
						token >> temp;
						token >> numPos;
						for (size_t i = 0; i < numPos; i++)
						{
							//vec3 time
							Vector3 pos;
							float t;

							std::getline(fs, line);
							token = std::stringstream(line);

							std::getline(token, line, ',');
							pos.x = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							pos.y = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							pos.z = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							t = static_cast<float>(atof(line.c_str()));
							currBA->animation.AddPosition(pos, t);
						}
					}
					//numrot
					{
						size_t numRot = 0;
						std::getline(fs, line);
						token = std::stringstream(line);
						token >> temp;
						token >> numRot;
						for (size_t i = 0; i < numRot; i++)
						{
							//vec3 time
							Quaternion quat;
							float t;

							std::getline(fs, line);
							token = std::stringstream(line);

							std::getline(token, line, ',');
							quat.x = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							quat.y = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							quat.z = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							quat.w = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							t = static_cast<float>(atof(line.c_str()));
							currBA->animation.AddRotation(quat, t);
						}

					}
					//numscale
					{
						size_t numScale = 0;
						std::getline(fs, line);
						token = std::stringstream(line);
						token >> temp;
						token >> numScale;
						for (size_t i = 0; i < numScale; i++)
						{
							//vec3 time
							Vector3 sca;
							float t;

							std::getline(fs, line);
							token = std::stringstream(line);

							std::getline(token, line, ',');
							sca.x = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							sca.y = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							sca.z = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							t = static_cast<float>(atof(line.c_str()));
							currBA->animation.AddScale(sca, t);
						}
					}

				}

			}





 
		}
 		fs.close();
 		return true;
	}
 	return false;
}

bool H::Graphics::MeshIO::LoadAnimationClip(std::unique_ptr < H::Graphics::AnimationClip>& animClip, std::filesystem::path fromfile)
{
	std::ifstream fs;
	fs.open(fromfile);

	std::string line;
	std::stringstream token;

	if (fs.is_open())
	{
		std::string temp;

		uint32_t numClips = 0;


		if (!fs.eof())
		{
			//numClips
			std::getline(fs, line);
			token = std::stringstream(line);
			token >> temp;
			token >> numClips;

 
			for (size_t clipIdx = 0; clipIdx < numClips; clipIdx++)
			{
				auto& currClip = animClip = std::make_unique<AnimationClip>();
				//c d t
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> currClip->name;
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> currClip->duration;
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> currClip->tickPerSecond;


				size_t maxBoneAnim = 0;
				// max BoneAnims
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> maxBoneAnim;

				uint32_t numBoneAnim = 0;
				//numBoneAnims
				std::getline(fs, line);
				token = std::stringstream(line);
				token >> temp;
				token >> numBoneAnim;

				auto& boneAnims = currClip->boneAnimations;
				boneAnims.resize(maxBoneAnim);

				for (size_t bai = 0; bai < numBoneAnim; ++bai)
				{
					//boneanim index
					size_t baIdx = 0;
					std::getline(fs, line);
					token = std::stringstream(line);
					token >> temp;
					token >> baIdx;
					const auto& currBA = boneAnims[baIdx] = std::make_unique<BoneAnimation>();
					//boneanim name
					std::getline(fs, line);
					token = std::stringstream(line);
					token >> temp;
					token >> currBA->boneName;

					//boneanim anim 3 channels
					//numPos
					{
						size_t numPos = 0;
						std::getline(fs, line);
						token = std::stringstream(line);
						token >> temp;
						token >> numPos;
						for (size_t i = 0; i < numPos; i++)
						{
							//vec3 time
							Vector3 pos;
							float t;

							std::getline(fs, line);
							token = std::stringstream(line);

							std::getline(token, line, ',');
							pos.x = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							pos.y = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							pos.z = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							t = static_cast<float>(atof(line.c_str()));
							currBA->animation.AddPosition(pos, t);
						}
					}
					//numrot
					{
						size_t numRot = 0;
						std::getline(fs, line);
						token = std::stringstream(line);
						token >> temp;
						token >> numRot;
						for (size_t i = 0; i < numRot; i++)
						{
							//vec3 time
							Quaternion quat;
							float t;

							std::getline(fs, line);
							token = std::stringstream(line);

							std::getline(token, line, ',');
							quat.x = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							quat.y = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							quat.z = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							quat.w = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							t = static_cast<float>(atof(line.c_str()));
							currBA->animation.AddRotation(quat, t);
						}

					}
					//numscale
					{
						size_t numScale = 0;
						std::getline(fs, line);
						token = std::stringstream(line);
						token >> temp;
						token >> numScale;
						for (size_t i = 0; i < numScale; i++)
						{
							//vec3 time
							Vector3 sca;
							float t;

							std::getline(fs, line);
							token = std::stringstream(line);

							std::getline(token, line, ',');
							sca.x = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							sca.y = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							sca.z = static_cast<float>(atof(line.c_str()));
							std::getline(token, line, ',');
							t = static_cast<float>(atof(line.c_str()));
							currBA->animation.AddScale(sca, t);
						}
					}

				}

			}






		}
		fs.close();
		return true;
	}
	return false;
}
