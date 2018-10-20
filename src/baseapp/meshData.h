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

	friend bool operator < (const Vertex& v1, const Vertex& v2)
	{
		if(v1.m_position < v2.m_position)
			return true;
		if(v1.m_position > v2.m_position)
			return false;

		if(v1.m_normal < v2.m_normal)
			return true;
		if(v1.m_normal > v2.m_normal)
			return false;

		//if(v1.m_tangent < v2.m_tangent)
		//	return true;
		//if(v1.m_tangent > v2.m_tangent)
		//	return false;

		//if(v1.m_bitangent < v2.m_bitangent)
		//	return true;
		//if(v1.m_bitangent > v2.m_bitangent)
		//	return false;
		//
		//if(v1.m_uv < v2.m_uv)
		//	return true;
		//if(v1.m_uv > v2.m_uv)
		//	return false;

		return false;
	}
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
