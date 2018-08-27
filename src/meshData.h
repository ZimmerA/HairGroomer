#ifndef MESHDATA_H
#define MESHDATA_H

#include <linalg.hpp>
#include <vector>

using namespace std;

struct Vertex
{
	vec3 m_position;
	vec3 m_normal;
	vec2 m_uv;
};

/*
*	Holds the data of loaded Meshes to be used by opengl or else
*	The data can later be loaded by e.g glMeshes
*/
class MeshData
{
public:
	MeshData(const vector<Vertex>& vertices, const vector<unsigned int>& indices);

	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
};

#endif // MESHDATA_H
