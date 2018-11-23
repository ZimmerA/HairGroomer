#ifndef MESHDATA_H
#define MESHDATA_H

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/hash.hpp>

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
	glm::vec3 m_position{0};
	glm::vec3 m_normal{0};
	glm::vec3 m_tangent{0};
	glm::vec3 m_bitangent{0};
	glm::vec2 m_uv{0};

	std::vector<VertexBoneInfo> m_bones;

	// Used by unordered_map to find identical vertices
	bool operator==(const Vertex &other) const
	{
		return (m_position == other.m_position) &&
		       (m_normal == other.m_normal) &&
		       (m_tangent == other.m_tangent) &&
		       (m_bitangent == other.m_bitangent) &&
		       (m_uv == other.m_uv);
	}
};

// Used by unordered_map to find identical vertices
namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const &vertex) const
		{
			return ((hash<glm::vec3>()(vertex.m_position) ^
					(hash<glm::vec3>()(vertex.m_normal) << 1)) >> 1) ^
					(hash<glm::vec2>()(vertex.m_uv) << 1);
		}
	};
}

/**
* \brief Holds the data of loaded Meshes to be used by e.g OpenGL.
*/
class MeshData
{
public:

	MeshData(
		std::string name,
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		int num_triangles);

	int get_num_triangles() const noexcept;
	const std::vector<unsigned int>& get_indices() const noexcept;
	std::vector<glm::vec2> get_face_uvs();

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	int m_num_triangles;
	std::string m_name;
};

#endif //MESHDATA_H