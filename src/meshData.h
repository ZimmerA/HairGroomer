#ifndef MESHDATA_H
#define MESHDATA_H

#include <linalg.hpp>
#include <vector>

using namespace std;

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
};

/**
* \brief Holds the data of loaded Meshes to be used by e.g OpenGL.
*/
class MeshData
{
public:
	MeshData(const vector<Vertex>& vertices, const vector<unsigned int>& indices);

	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
};

#endif // MESHDATA_H
