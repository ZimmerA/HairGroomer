#include "meshData.h"

MeshData::MeshData(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures)
{
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_textures = textures;
}