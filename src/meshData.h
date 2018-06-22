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

struct Texture
{
	unsigned int m_id;
	std::string m_type;
	std::string m_path;
};

/*
*	Holds the data of loaded Meshes to be used across multiple contexts
*	The data is later loaded by glMeshes
*/
class MeshData
{
public:
	MeshData(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);

	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	vector<Texture> m_textures;
};

#endif // MESHDATA_H
