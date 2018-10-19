#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>

#include <linalg.hpp>

using namespace std;

/**
 * \brief Structure for storing bone info inside a vertex
 */
struct VertexBoneInfo
{
	int m_id;
	float m_weight;
};

/**
 * \brief Structure for storing vertex Data
 */
struct Vertex
{
	vec3 m_position;
	vec3 m_normal;
	vec3 m_tangent;
	vec3 m_bitangent;
	vec2 m_uv;
	std::vector<VertexBoneInfo> m_bones;
};


/**
* \brief Holds the data of loaded Meshes to be used by e.g OpenGL.
*/
class MeshData
{
public:
	MeshData(vector<Vertex> vertices, vector<unsigned int> indices, int num_faces);

	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	int m_num_faces;
};

#endif // MESHDATA_H