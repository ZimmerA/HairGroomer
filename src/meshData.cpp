#include "meshData.h"

MeshData::MeshData(const vector<Vertex>& vertices, const vector<unsigned int>& indices)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
}