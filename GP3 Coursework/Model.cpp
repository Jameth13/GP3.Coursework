#include "pch.h"
#include "Common.h"
#include "Model.h"

Model::Model() {}

Model::~Model()
{
	//Delete meshes
	for (unsigned int i = 0u; i < m_meshes.size(); i++)
		delete m_meshes[i];

	m_meshes.clear();
	m_meshes.shrink_to_fit();
}

void Model::Load(std::string directory)
{
	//Check is meshes already exist.
	if (assetMan->meshes.count(directory) > 0)
	{
		m_meshes = assetMan->meshes[directory];
		//Return to skip load.
		return;
	}
	//Store new meshes directory.
	newMeshesDir = directory;

	//importer should release all resources the moment it goes out of scope
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(directory, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	//Check if model was loaded.
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Failed to load model \"" << directory << "\". Using missing model." << std::endl;

		//Hard-coded missing model.
		if (missingModelData == nullptr)
		{
			missingModelDataString = "v 1.000000 0.000000 0.000000\nv 0.000000 0.000000 -1.000000\nv 0.000000 -1.000000 0.000000\nv 0.000000 1.000000 -0.000000\nv 0.000000 0.000000 1.000000\nv -1.000000 0.000000 0.000000\nvt 0.666667 0.333333\nvt 0.333333 0.500000\nvt 0.666667 0.666667\nvt 0.333333 0.833333\nvt 0.666667 1.000000\nvt 0.333333 1.000000\nvt 0.333333 0.666667\nvt 0.000000 0.833333\nvt 0.333333 0.333333\nvt 0.333333 0.000000\nvt 0.000000 0.166667\nvt 1.000000 0.000000\nvt 0.666667 0.166667\nvt 1.000000 0.333333\nvt 1.000000 0.666667\nvt 1.000000 0.333333\nvt 0.666667 0.500000\nvt 0.666667 0.333333\nvt 0.666667 0.000000\nvt 0.333333 0.166667\nvt 0.000000 0.500000\nvn 0.5774 0.5774 -0.5774\nvn 0.5774 0.5774 0.5774\nvn -0.5774 0.5774 0.5774\nvn -0.5774 0.5774 -0.5774\nvn 0.5774 -0.5774 -0.5774\nvn 0.5774 -0.5774 0.5774\nvn -0.5774 -0.5774 0.5774\nvn -0.5774 -0.5774 -0.5774\nf 2/1/1 4/2/1 1/3/1\nf 4/4/2 5/5/2 1/3/2\nf 4/6/3 6/7/3 5/8/3\nf 2/9/4 6/10/4 4/11/4\nf 3/12/5 2/13/5 1/14/5\nf 1/15/6 5/16/6 3/17/6\nf 5/18/7 6/19/7 3/20/7\nf 3/21/8 6/7/8 2/9/8\n";
			missingModelData = missingModelDataString.c_str();
			missingModelDataLength = missingModelDataString.length();
		}

		const aiScene* missingScene = importer.ReadFileFromMemory(missingModelData, sizeof(char) * missingModelDataLength, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
		processNode(missingScene->mRootNode, missingScene);
		return;
	}
	processNode(scene->mRootNode, scene);
}

void Model::Draw()
{
	for (Mesh* mesh : GetMeshes())
	{
		mesh->Bind();
		glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

	//Add new mesh to asset manager
	assetMan->meshes[newMeshesDir] = m_meshes;
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals, colors and texture coordinates
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tangent.x = mesh->mTangents[i].x;
				vertex.tangent.y = mesh->mTangents[i].y;
				vertex.tangent.z = mesh->mTangents[i].z;
				vertex.bitangent.x = mesh->mBitangents[i].x;
				vertex.bitangent.y = mesh->mBitangents[i].y;
				vertex.bitangent.z = mesh->mBitangents[i].z;
			}
			else
				std::cout << "Error! Assimp failed to generate tangents and bitangents." << std::endl;
		}
		else
			std::cout << "Error! Assimp failed to generate normals." << std::endl;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.texture.x = mesh->mTextureCoords[0][i].x;
			vertex.texture.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texture = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	return new Mesh(vertices, indices);
}
