
#include "raycast.h"

#include <limits>

#define CMP_EPSILON 0.00001f

glm::vec3 project_screen_to_world(
	const glm::vec3 &screen,
	const glm::mat4 &view_projection)
{
	const glm::mat4 inv_vp = glm::inverse(view_projection);
	const glm::vec4 world = inv_vp * glm::vec4(screen * 2.0f - 1.0f, 1.0);

	return glm::vec3(world) / world.w;
}

bool ray_triangle_intersects(
	const glm::vec3 &origin, const glm::vec3 &direction,
	const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3,
	float &t, float &u, float &v)
{
	glm::vec3 edge1 = p2 - p1;
	glm::vec3 edge2 = p3 - p1;

	glm::vec3 p = glm::cross(direction, edge2);
	float det = glm::dot(edge1, p);

	if (det == 0.0f)
		return false;

	det = 1.0f / det;

	glm::vec3 vt = origin - p1;
	u = glm::dot(vt, p) * det;

	if ((u < 0.0f) || (u > 1.0f))
		return false;

	glm::vec3 vq = glm::cross(vt, edge1);
	v = glm::dot(direction, vq) * det;

	if ((v < 0.0f) || ((u + v) > 1.0f))
		return false;

	t = glm::dot(edge2, vq) * det;

	return t > CMP_EPSILON;
}

bool ray_mesh_intersects(
	const glm::vec3 &origin, const glm::vec3 &direction,
	const MeshData *mesh_data,
	const Vertex **vertex1, const Vertex **vertex2, const Vertex **vertex3,
	float &t, float &u, float &v)
{
	bool hit = false;
	float t_min = std::numeric_limits<float>::infinity();
	float _t, _u, _v;

	for (int face = 0; face < mesh_data->m_num_triangles; ++face)
	{
		const Vertex &v1 = mesh_data->m_vertices[mesh_data->m_indices[face * 3 + 0]];
		const Vertex &v2 = mesh_data->m_vertices[mesh_data->m_indices[face * 3 + 1]];
		const Vertex &v3 = mesh_data->m_vertices[mesh_data->m_indices[face * 3 + 2]];

		if (ray_triangle_intersects(
			origin, direction,
			v1.m_position, v2.m_position, v3.m_position,
			_t, _u, _v))
		{
			if (_t < t_min)
			{
				hit = true;
				t_min = _t;

				*vertex1 = &v1;
				*vertex2 = &v2;
				*vertex3 = &v3;

				t = _t;
				u = _u;
				v = _v;
			}
		}
	}

	return hit;
}

bool raycast(
	const glm::vec3 &origin, const glm::vec3 &direction,
	const MeshData *mesh_data,
	RaycastHit &hit)
{
	const Vertex *vertex1, *vertex2, *vertex3;
	float t, u, v;

	if (!ray_mesh_intersects(origin, direction, mesh_data, &vertex1, &vertex2, &vertex3, t, u, v))
		return false;

	hit.point = origin + direction * t;

	hit.barycentric_coordinate = glm::vec3(1.0f - u - v, u, v);

	hit.uv = vertex1->m_uv * hit.barycentric_coordinate.x +
	         vertex2->m_uv * hit.barycentric_coordinate.y +
	         vertex3->m_uv * hit.barycentric_coordinate.z;

	hit.normal = glm::normalize(
	    vertex1->m_normal * hit.barycentric_coordinate.x +
	    vertex2->m_normal * hit.barycentric_coordinate.y +
	    vertex3->m_normal * hit.barycentric_coordinate.z);

	return true;
}
