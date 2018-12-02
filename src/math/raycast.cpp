
#include "raycast.h"

#include <limits>

#define CMP_EPSILON 0.00001f

/**
 * \brief Projects a point on screen into world space
 * \param screen The point in screen space
 * \param model_view_projection The mvp Matrix
 * \return The given point in world space
 */
glm::vec3 project_screen_to_world(
	const glm::vec3 &screen,
	const glm::mat4 &model_view_projection)
{
	const glm::mat4 inv_vp = glm::inverse(model_view_projection);
	const glm::vec4 world = inv_vp * glm::vec4(screen * 2.0f - 1.0f, 1.0);

	return glm::vec3(world) / world.w;
}

/**
 * \brief Checks if the specified ray hit this triangle and returns where in relation to the texture coordinates
 * \param origin The origin of the ray
 * \param direction The direction of the ray
 * \param p1 Point 1 of the checked triangle
 * \param p2 Point 2 of the checked triangle
 * \param p3 Point 3 of the checked triangle
 * \param t The hit time along the ray
 * \param u The hit time along the u texture coordinate
 * \param v The normalized hit time along the v texture coordinate
 * \return True if the ray hit the triangle, false if not
 */
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

/**
 * \brief Iterates over every face of the given mesh to check if the given ray hit it
 * \param origin The origin of the ray
 * \param direction The direction of the ray
 * \param mesh_data The data of the checked mesh
 * \param vertex1 The first Vertex of the face that was hit
 * \param vertex2 The second Vertex of the face that was hit
 * \param vertex3 The third Vertex of the face that was hit
 * \param t The hit time along the ray
 * \param u The hit time along the u texture coordinate
 * \param v The normalized hit time along the v texture coordinate
 * \return True if the ray hit the mesh, false if not
 */
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

/**
 * \brief Casts a ray from origin to direction and checks for intersection with the given mesh
 * \param origin The origin of the ray
 * \param direction The direction of the ray
 * \param mesh_data The mesh data to check for intersections
 * \param hit The resulting hit
 * \return True if the ray hit the mesh, false if not
 */
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
