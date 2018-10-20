#ifndef MODEL_H
#define MODEL_H

#include "meshData.h"

#include <vector>
#include <map>

#include <QOpenGLShaderProgram>
#include <fbxsdk/scene/constraint/fbxconstraintutils.h>
#include <fbxsdk.h>

using namespace std;

/**
 * \brief Holds the info of a bone
 */
struct Bone
{
	mat4 m_global_bindpose;
	string m_name;
	int m_parent = -1;
};

/**
 * \brief Loads and stores all the data of a model, contains a list of MeshData objects that make up the model
 */
class ModelData
{
public:
	explicit ModelData(const char* path);

	int get_num_faces(uint index);
	vector<int> get_indices(uint index);
	vector<vec2> get_face_uvs(uint index);
	int get_num_bones() const {return static_cast<int>(m_bone_map_.size());}

	// MeshData for every sub mesh of the model
	vector<MeshData> m_meshes;
		// Contains the Bones
	vector<Bone> m_bone_list_;
private:
	void load_model(const string& path);
	void process_node(FbxNode* node, FbxManager* manager);
	vec3 read_normal(FbxMesh * mesh, int control_point_index, int vertex_counter);
	MeshData process_mesh(FbxMesh* mesh, FbxNode* node, FbxManager* manager);
	//void complete_skeleton(const aiNode* node, const aiScene* scene, int parent_index);
	// Processes Assimp node in the model tree
	//void process_node(aiNode* node, const aiScene* scene);
	// Create a meshData object from the Assimp data
	//MeshData process_mesh(aiMesh* assimp_mesh, const aiScene* scene, const aiNode* node);

	string m_directory_;
	// Maps bone names to the index in the bone_list
	map<string, uint> m_bone_map_;

};

#endif // MODEL_H
