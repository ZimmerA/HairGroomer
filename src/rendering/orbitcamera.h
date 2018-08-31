#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H
#include <linalg.hpp>

/**
 * \brief The Orbit camera for the perspective viewport
 */
class Orbitcamera
{
public:
	Orbitcamera();

	mat4 get_view_matrix() const;
	void handle_mouse_move(float delta_x, float delta_y);
	void handle_mouse_wheel(float scroll_delta);
	void reset_position();
	void move_pivot_point(float x, float y);
	vec3 m_position;
private:
	float m_azimuth_;
	float m_elevation_;
	float m_distance_;
	float m_sensitivity_;
	float m_scrollspeed_;

	void calc_position();
	vec3 m_lookat_point_;
};
#endif