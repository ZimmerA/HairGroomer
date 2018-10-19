#include "modelData.h"
#include <QDebug>
#include <assimp/postprocess.h>

void assimp_to_linalg_matrix(const aiMatrix4x4& from, mat4& to)
{
	for(int x = 0; x < 4; x++)
	{
		for(int y = 0; y < 4; y++)
		{
			to[x][y] = from[y][x]; // Transpose cause of column/row major difference
		}
	}
}


ModelData::ModelData(const char* path)
{
	load_model(path);
}

/**
 * \brief Returns the amount of faces the model has
 * \return The amount of faces
 */
int ModelData::get_num_faces(uint index)
{

	return m_meshes.at(index).m_num_faces;

}

/**
 * \brief Returns the indices of every mesh in the model
 * \return A int vector holding the indices
 */
vector<int> ModelData::get_indices(uint index)
{
	vector<int> indices;

	for(auto const& index : m_meshes.at(index).m_indices)
	{
		indices.push_back(index);
	}

	return indices;
}

/**
 * \brief Returns the texture coordinates of every index of the mesh of the given index
 * \return A vec2 vector holding the texture coordinates
 */
vector<vec2> ModelData::get_face_uvs(uint index)
{
	vector<vec2> texcoords;

	for(auto const& indice : m_meshes.at(index).m_indices)
	{
		texcoords.push_back(m_meshes.at(index).m_vertices.at(indice).m_uv);
	}
	return texcoords;
}

/**
 * \brief Loads the model of the given path using Assimp
 * \param path Path to the model
 */
void ModelData::load_model(const string& path)
{
	Assimp::Importer import;
	import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	// Triangluate the model, flip the texture coordinates so we don't have to flip them in the shader or flip the picture, calculate tangent space if not present, remove duplicate vertices
	const auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qDebug() << "ERROR::ASSIMP::" << import.GetErrorString();
		return;
	}

	m_directory_ = path.substr(0, path.find_last_of('/'));
	// Iterate over every node and load meshes and their bones
	process_node(scene->mRootNode, scene);

	// Iterate over every node again to find the remaining bones and build up the parent relationship
	complete_skeleton(scene->mRootNode, scene, -1);
}

void ModelData::complete_skeleton(const aiNode * node, const aiScene * scene, const int parent_index)
{
	int bone_index = -1;

	// Node is a bone, calc and go on with children
	if(m_bone_map_.find(node->mName.data) != m_bone_map_.end())
	{
		bone_index = m_bone_map_[node->mName.data];
		
		if(parent_index == -1)
		{
			assimp_to_linalg_matrix(node->mTransformation, m_bone_list_.at(bone_index).m_global_bindpose);
		}else
		{
			mat4 local_bindpose;
			assimp_to_linalg_matrix(node->mTransformation,local_bindpose);

			m_bone_list_.at(bone_index).m_global_bindpose = m_bone_list_.at(parent_index).m_global_bindpose *  local_bindpose ;
		}

		m_bone_list_.at(bone_index).m_parent = parent_index;
	}

	// If were at the root node or the current node is a bone, iterate over the children
	if(bone_index != -1 || node == scene->mRootNode)
	{
		for(uint i= 0; i < node->mNumChildren; i++)
		{
			complete_skeleton(node->mChildren[i],scene, bone_index);
		}
	}

		/*
	// Then read out the Bone hiearchy
	string node_name = node->mName.data;

	if(node_name == "Bone_02")
	{
		qDebug() << node_name.data();
		auto  m_GlobalInverseTransform = node->mParent->mTransformation;
		//m_GlobalInverseTransform.Inverse();
		auto test =  m_GlobalInverseTransform * node->mTransformation; // Korrekte bindpose

		for(int x = 0; x < 4; x++)
		{
			for(int y = 0; y< 4; y++)
			{
				qDebug() << test[y][x];
			}
		}
	}
	*/
	/*
	if (m_bone_map_.find(node_name) != m_bone_map_.end()) {
		int index = m_bone_map_[node_name];

		int parent_index = m_bone_map_[node->mParent->mName];
		if(m_bone_map_.)
		m_bone_list_.at(index).m_parent = parent_index;

	}*/
}

/**
 * \brief Recursively iterate over every node in the model and process each mesh in the node
 * \param node The according Assimp node
 * \param scene The according Assimp scene
 */
void ModelData::process_node(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		const auto mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(process_mesh(mesh, scene, node));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i],scene);
	}
}

/**
 * \brief Processes a mesh of a node
 * \param assimp_mesh The according Assimp mesh
 * \return A MeshData object which holds the indices and vertices of the mesh
 */
MeshData ModelData::process_mesh(aiMesh* assimp_mesh,const  aiScene* scene, const aiNode* mesh_node)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++)
	{
		// Vertex that will be added to the list at the end
		Vertex vertex;

		// Load Position
		vec3 vector;
		vector.x = assimp_mesh->mVertices[i].x;
		vector.y = assimp_mesh->mVertices[i].y;
		vector.z = assimp_mesh->mVertices[i].z;
		vertex.m_position = vector;

		// Load Normals
		vector.x = assimp_mesh->mNormals[i].x;
		vector.y = assimp_mesh->mNormals[i].y;
		vector.z = assimp_mesh->mNormals[i].z;
		vertex.m_normal = vector;

		// Does the mesh contain texture coordinates?
		if (assimp_mesh->mTextureCoords[0]) 
		{
			// add texcoords
			vec2 vec;
			vec.x = assimp_mesh->mTextureCoords[0][i].x;
			vec.y = assimp_mesh->mTextureCoords[0][i].y;
			vertex.m_uv = vec;

			vertex.m_bitangent = vector;
		}
		else
		{
			// default texcoord (0/0)
			vertex.m_uv = vec2(0.0f, 0.0f);
		}

		// Does the mesh contain tangents/bitangents?
		if (assimp_mesh->mTangents)
		{
			// add tangents
			vector.x = assimp_mesh->mTangents[i].x;
			vector.y = assimp_mesh->mTangents[i].y;
			vector.z = assimp_mesh->mTangents[i].z;
			vertex.m_tangent = vector;
			//add bitangents
			vector.x = assimp_mesh->mBitangents[i].x;
			vector.y = assimp_mesh->mBitangents[i].y;
			vector.z = assimp_mesh->mBitangents[i].z;
			vertex.m_bitangent = vector;
		}else
		{
			// default tangent/bitangent (0,0,0)
			vertex.m_tangent = vec3(0,0,0);
			vertex.m_bitangent = vec3(0,0,0);
		}
		vertices.push_back(vertex);
	}

	// Process bones of this mesh
	for(unsigned int i = 0; i < assimp_mesh->mNumBones; i++)
	{
		uint bone_index;
		string bone_name(assimp_mesh->mBones[i]->mName.data);

		// Check if the bone is in the map, if no add it
		if(m_bone_map_.find(bone_name) == m_bone_map_.end())
		{
			bone_index = static_cast<int>(m_bone_map_.size());
			Bone bone;
			bone.m_name = bone_name;
			//assimp_to_linalg_matrix(assimp_mesh->mBones[i]->mOffsetMatrix,bone.m_offset_matrix);
			m_bone_list_.push_back(bone);
			m_bone_map_[bone_name] = bone_index;

		}else // Bone is already in the map get the index of the bone
		{
			bone_index = m_bone_map_[bone_name];
		}

		// Loop over the bones weights
        for (uint j = 0 ; j < assimp_mesh->mBones[i]->mNumWeights ; j++) {
	        const float bone_weight = assimp_mesh->mBones[i]->mWeights[j].mWeight;
			VertexBoneInfo bone_info{};
			bone_info.m_weight = bone_weight;
			bone_info.m_id = bone_index; // index of the bone in the bone list
			vertices.at(assimp_mesh->mBones[i]->mWeights[j].mVertexId).m_bones.push_back(bone_info);
        }

		// Look for this bone in the hierachy
		auto node = scene->mRootNode->FindNode(bone_name.data());
		if(node)
			node = node->mParent;

		// Travel up until we either hit the mesh node or the parent of the mesh node
		while(node && node != mesh_node && node != mesh_node->mParent)
		{
			string parent_bone_name(node->mName.data);

			// Bone is not in the list, add it
			if(m_bone_map_.find(parent_bone_name) == m_bone_map_.end())
			{
				bone_index = static_cast<int>(m_bone_map_.size());
				Bone bone;
				bone.m_name = parent_bone_name;
				m_bone_list_.push_back(bone);
				m_bone_map_[parent_bone_name] = bone_index;
			}

			node = node->mParent;
		}


	}

	// process indices
	for (unsigned int i = 0; i < assimp_mesh->mNumFaces; i++)
	{
		const auto face = assimp_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return MeshData(vertices, indices, assimp_mesh->mNumFaces);
}
