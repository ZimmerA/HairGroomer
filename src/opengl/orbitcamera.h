#pragma once
#include <linalg.hpp>

class Orbitcamera
{
public:
	Orbitcamera();
	vec3 m_position;
	vec3 m_lookat_point;

	float m_azimuth;
	float m_elevation;
	float m_distance;

	float m_sensitivity;
	float m_scrollspeed;
	void calc_position();
	mat4 get_view_matrix() const;
	void handle_mouse_move(float delta_x, float delta_y);
	void handle_mouse_wheel(float scroll_delta);
	void move_pivot_point(float x, float y);

};

