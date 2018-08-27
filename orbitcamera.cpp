#include "orbitcamera.h"
#include <QDebug>


Orbitcamera::Orbitcamera()
{
	m_azimuth = LINALG_DEG2RAD * 0;
	m_elevation = LINALG_DEG2RAD * 75;
	m_distance = 350;
	m_lookat_point = vec3(0, 0, 0);
	m_sensitivity = 0.5f;
	m_scrollspeed = 15;
	calc_position();
}


void Orbitcamera::calc_position()
{
	const vec3 local_x = vec3(1.0f, 0.f, 0.f);
	const vec3 local_y = vec3(0.f, 1.0f, 0.f);
	const vec3 local_z = vec3(0.f, 0.f, 1.0f);

	const float x = sinf(m_azimuth) * sinf(m_elevation);
	const float y = cosf(m_elevation);
	const float z = cosf(m_azimuth) * sinf(m_elevation);

	vec3 displacement = local_x * x + local_y * y + local_z * z;
	displacement *= m_distance;
	m_position = m_lookat_point + displacement;
}

mat4 Orbitcamera::get_view_matrix() const
{
	return mat4::lookAt(m_position, m_lookat_point, vec3(0, 1, 0));
}

void Orbitcamera::handle_mouse_move(const float delta_x, const float delta_y)
{
	const float move_x = delta_x * m_sensitivity;
	const float move_y = delta_y * m_sensitivity;

	m_azimuth += LINALG_DEG2RAD * move_x;
	m_elevation += LINALG_DEG2RAD * move_y;

	calc_position();
}

void Orbitcamera::move_pivot_point(const float x, const float y)
{
	mat4 lookat = get_view_matrix();
	const vec3 upvec = lookat.row(1);
	const vec3 rightvec = lookat.row(0);
	m_lookat_point += upvec * y;
	m_lookat_point += rightvec * x;
	calc_position();
}

void Orbitcamera::handle_mouse_wheel(float scroll_delta)
{
	m_distance += scroll_delta * m_scrollspeed;
	calc_position();
}

