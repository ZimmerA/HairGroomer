#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm.hpp>

#include "baseapp/meshData.h"

struct RaycastHit
{
	glm::vec3 point;
	glm::vec3 barycentric_coordinate;
	glm::vec2 uv;
	glm::vec3 normal;
};

glm::vec3 project_screen_to_world(
	const glm::vec3 &screen,
	const glm::mat4 &view_projection);

bool ray_triangle_intersects(
	const glm::vec3 &origin, const glm::vec3 &direction,
	const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3,
	float &t, float &u, float &v);

bool ray_mesh_intersects(
	const glm::vec3 &origin, const glm::vec3 &direction,
	const MeshData *mesh_data,
	const Vertex **vertex1, const Vertex **vertex2, const Vertex **vertex3,
	float &t, float &u, float &v);

bool raycast(
	const glm::vec3 &origin, const glm::vec3 &direction,
	const MeshData *mesh_data,
	RaycastHit &hit);

#endif