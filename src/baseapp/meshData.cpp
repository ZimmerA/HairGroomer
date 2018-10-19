#include "meshData.h"
#include <utility>

MeshData::MeshData(vector<Vertex> vertices, vector<unsigned int> indices, const int num_faces) :
	m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_num_faces(num_faces)
{
}
