
#include "meshData.h"

MeshData::MeshData(const std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, const int num_faces) :
	m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_num_faces(num_faces), m_name(name)
{

}

/**
 * \brief Returns the amount of faces the mesh has
 * \return The amount of faces
 */
int MeshData::get_num_faces() const noexcept
{
	return m_num_faces;
}

/**
 * \brief Returns the indices of the mesh
 * \return A std::vector<int> holding the indices
 */
std::vector<unsigned int> MeshData::get_indices() const noexcept
{
	return m_indices;
}

/**
 * \brief Returns the texture coordinates of every index of the mesh
 * \return A std::vector<vec2> holding the texture coordinates
 */
std::vector<glm::vec2> MeshData::get_face_uvs()
{
	std::vector<glm::vec2> face_uvs;

	for (auto const& indice : m_indices)
	{
		face_uvs.push_back(m_vertices.at(indice).m_uv);
	}

	return face_uvs;
}
