#include "modelData.h"
#include <iostream>
#include <assimp/postprocess.h>

ModelData::ModelData(const char* path)
{
	load_model(path);
}

/**
 * \brief Loads the model of the given path using Assimp
 * \param path Path to the model
 */
void ModelData::load_model(const string& path)
{
	Assimp::Importer import;
	const auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	m_directory_ = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);
}

/**
 * \brief Recursively iterate over every node in the model
 * \param node The according Assimp node
 * \param scene The according Assimp scene
 */
void ModelData::process_node(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		const auto mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(process_mesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene);
	}
}


/**
 * \brief Processes a mesh of a node
 * \param mesh The according Assimp mesh
 * \param scene The according Assimp scene
 * \return A MeshData object which holds the indices and vertices of the mesh
 */
MeshData ModelData::process_mesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.m_position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.m_normal = vector;
		// does the mesh contain texture coordinates?
		if (mesh->mTextureCoords[0]) 
		{
			vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_uv = vec;

			vertex.m_bitangent = vector;
		}
		else
		{
			vertex.m_uv = vec2(0.0f, 0.0f);
		}
		if (mesh->mTangents)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.m_tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.m_bitangent = vector;
		}

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return MeshData(vertices, indices);
}
