
#include "orbitcamera.h"

#include <gtc/matrix_transform.hpp>

const float default_azimuth = 0.f;
const float default_elevation = 50.f;
const glm::vec3 default_lookat{0,0,0};
const float default_distance = 550.f;
const float default_sensitivity = 20.0f;
const float default_scrollspeed = 15;

OrbitCamera::OrbitCamera() noexcept
{
	m_azimuth_ = default_azimuth;
	m_elevation_ = default_elevation;
	m_distance_ = default_distance;
	m_lookat_point_ = default_lookat;
	m_sensitivity_ = default_sensitivity;
	m_scrollspeed_ = default_scrollspeed;

	calc_position();
}

/**
 * \brief Calculates the new position of the orbit camera using a spherical to karthesian coordinate system
 */
void OrbitCamera::calc_position()
{
	m_view_ = glm::mat4(1);
	m_view_ = translate(m_view_, m_lookat_point_);
	m_view_ = rotate(m_view_, glm::radians(m_azimuth_), glm::vec3(0.f,1.f,0.f));
	m_view_ = rotate(m_view_, glm::radians(m_elevation_ - 90.0f) , glm::vec3(1.f,0.f,0.f));
	m_view_ = translate(m_view_, glm::vec3(0,0,m_distance_));

	m_position = glm::vec3(m_view_[3][0],m_view_[3][1],m_view_[3][2]);

	m_view_ = inverse(m_view_);
}

/**
 * \brief Generates the lookat matrix for the camera using position and pivot point
 * \return The lookatmatrix
 */
glm::mat4 OrbitCamera::get_view_matrix() const noexcept
{
	return m_view_;
}

/**
 * \brief Calculates new azimuth and elevation angles
 * \param delta_x The delta of the mouse motion on the x axis
 * \param delta_y The delta of the mouse motion on the y axis
 */
void OrbitCamera::handle_mouse_move(const float delta_x, const float delta_y)
{
	const float move_x = -delta_x * m_sensitivity_;
	const float move_y = -delta_y * m_sensitivity_;

	m_azimuth_ += glm::radians(move_x);
	m_elevation_ += glm::radians(move_y);

	m_azimuth_ = fmodf(m_azimuth_, 360.0f);

	if (m_elevation_ > 179.0f)
		m_elevation_ = 179.0f;
	if (m_elevation_ < 1.0f)
		m_elevation_ = 1.0f;

	calc_position();
}

/**
 * \brief Moves the pivot point
 * \param x The delta on the up axis
 * \param y The delta on the right axis
 */
void OrbitCamera::move_pivot_point(const float x, const float y)
{
	const glm::mat4 lookat = get_view_matrix();
	const glm::vec3 upvec(lookat[0][1], lookat[1][1], lookat[2][1]);
	const glm::vec3 rightvec(lookat[0][0], lookat[1][0], lookat[2][0]);

	m_lookat_point_ += upvec * y;
	m_lookat_point_ += rightvec * x;

	calc_position();
}

/**
 * \brief Adjusts the distance to the pivot point
 * \param scroll_delta How much to adjust
 */
void OrbitCamera::handle_mouse_wheel(const float scroll_delta)
{
	m_distance_ -= scroll_delta * m_scrollspeed_;

	if (m_distance_ <= 1.0f)
		m_distance_ = 1.0f;

	calc_position();
}

/**
 * \brief Set the camera to its starting position
 */
void OrbitCamera::reset_position()
{
	m_lookat_point_ = default_lookat;
	m_azimuth_= default_azimuth;
	m_elevation_ = default_elevation;
	m_distance_ = default_distance;

	calc_position();
}
