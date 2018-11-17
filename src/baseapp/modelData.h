#ifndef MODEL_H
#define MODEL_H

#include "meshData.h"

#include <vector>
#include <map>
#include <memory>

#include <QOpenGLShaderProgram>
//#include <fbxsdk.h>


/**
 * \brief Holds the info of a bone
 */
struct Bone
{
	glm::mat4 m_global_bindpose{1};
	std::string m_name;
	int m_parent = -1;
};

/**
 * \brief Helper struct used when loading meshes
 */
struct ControlpointInfo
{
	glm::vec3 m_position{};
	std::vector<VertexBoneInfo> m_bone_info;
};

/**
 * \brief Loads and stores all the data of a model, contains a list of MeshData objects that make up the model
 */
class ModelData
{
public:
	explicit ModelData(const QString& path);
	size_t get_num_bones() const noexcept { return m_bone_map_.size(); }

	// MeshData for every sub mesh of the model
	std::vector<MeshData> m_meshes;
	std::vector<std::string> m_name_list;
	// Contains the Bones
	std::vector<Bone> m_bone_list;
	QString m_path;

private:
/*	void load_model(const QString& path);
	void process_skeleton_nodes(FbxNode* node);
	void process_skeleton_hierachy_rec(FbxNode* node, int parent_index);
	void process_mesh_nodes(FbxNode* node, FbxManager* manager);
	
	MeshData process_mesh(FbxMesh* mesh, FbxManager* manager, std::string mesh_name);
	static glm::vec3 read_normal(FbxMesh* mesh, int control_point_index, int vertex_counter);
	static glm::vec2 read_uv(FbxMesh* mesh, int control_point_index, int vertex_counter);
	static glm::vec3 read_binormal(FbxMesh* mesh, int control_point_index, int vertex_counter);
	static glm::vec3 read_tangent(FbxMesh* mesh, int control_point_index, int vertex_counter);*/


	// Maps bone names to the index in the bone_list
	std::map<std::string, uint> m_bone_map_;
};

/**
 * \brief std::unique_ptr wrapper for Fbx pointers to wrap the Create/Destroy mechanism of the fbx sdk
 * \tparam T The pointer type
 */
/*template <typename T>
class FbxPointer : public std::unique_ptr<T, void (*)(T*)>
{
public:
	explicit FbxPointer(T* ptr = nullptr);

	FbxPointer(FbxPointer&& other) noexcept;

	FbxPointer& operator=(FbxPointer&& other) noexcept;

private:
	static void destroy(T* ptr);
};

template <typename T>
FbxPointer<T>::FbxPointer(T* ptr): std::unique_ptr<T, void (*)(T*)>(ptr, &FbxPointer::destroy)
{
} 
template <typename T>
FbxPointer<T>::FbxPointer(FbxPointer&& other) noexcept: std::unique_ptr(std::move(other))
{
}

template <typename T>
FbxPointer<T>& FbxPointer<T>::operator=(FbxPointer&& other) noexcept
{
	std::unique_ptr<T>::operator=(std::move(other));
	return *this;
}

template <typename T>
void FbxPointer<T>::destroy(T* ptr)
{
	if (ptr) ptr->Destroy();
}*/

#endif // MODEL_H
