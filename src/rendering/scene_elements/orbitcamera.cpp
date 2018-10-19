#include "orbitcamera.h"

const float default_azimuth = 0.f;
const float default_elevation = 90.f;
const vec3 default_lookat = vec3(0,0,0);
const float default_distance = 150.f;
const float default_sensitivity = 20.0f;
const float default_scrollspeed = 8;

Orbitcamera::Orbitcamera()
{
	m_azimuth_ = default_azimuth;
	m_elevation_ =  default_elevation;
	m_distance_ = default_distance;
	m_lookat_point_ = default_lookat;
	m_sensitivity_ = default_sensitivity;
	m_scrollspeed_ = default_scrollspeed;
	calc_position();
}

/**
 * \brief Calculates the new position of the orbit camera using a spherical to karthesian coordinate system
 */
void Orbitcamera::calc_position()
{
	// Up right and forward vector of the pivot point
	const vec3 local_x = vec3(1.0f, 0.f, 0.f);
	const vec3 local_y = vec3(0.f, 1.0f, 0.f);
	const vec3 local_z = vec3(0.f, 0.f, 1.0f);

	// Spherical to karthesian coordinates
	const float x = sinf(static_cast<float>(LINALG_DEG2RAD)* m_azimuth_) * sinf(static_cast<float>(LINALG_DEG2RAD) * m_elevation_);
	const float y = cosf(static_cast<float>(LINALG_DEG2RAD) * m_elevation_);
	const float z = cosf(static_cast<float>(LINALG_DEG2RAD) * m_azimuth_) * sinf(static_cast<float>(LINALG_DEG2RAD) * m_elevation_);
	
	vec3 displacement = local_x * x + local_y * y + local_z * z;
	displacement *= m_distance_;
	m_position = m_lookat_point_ + displacement;
}

/**
 * \brief Generates the lookat matrix for the camera using position and pivot point
 * \return The lookatmatrix
 */
mat4 Orbitcamera::get_view_matrix() const
{
	return mat4::lookAt(m_position, m_lookat_point_, vec3(0, 1, 0));
}

/**
 * \brief Calculates new azimuth and elevation angles
 * \param delta_x The delta of the mouse motion on the x axis
 * \param delta_y The delta of the mouse motion on the y axis
 */
void Orbitcamera::handle_mouse_move(const float delta_x, const float delta_y)
{
	const float move_x = delta_x * m_sensitivity_;
	const float move_y = delta_y * m_sensitivity_;

	m_azimuth_ += static_cast<float>(LINALG_DEG2RAD) * move_x;
	m_elevation_ += static_cast<float>(LINALG_DEG2RAD) * move_y;

	m_azimuth_ = fmodf(m_azimuth_, 360.0f);
	if(m_elevation_ > 179.0f)
		m_elevation_ = 179.0f;
	if(m_elevation_ < 1.0f)
		m_elevation_ = 1.0f;

	calc_position();
}

/**
 * \brief Moves the pivot point
 * \param x The delta on the up axis
 * \param y The delta on the right axis
 */
void Orbitcamera::move_pivot_point(const float x, const float y)
{
	mat4 lookat = get_view_matrix();
	const vec3 upvec = lookat.row(1);
	const vec3 rightvec = lookat.row(0);
	m_lookat_point_ += upvec * y;
	m_lookat_point_ += rightvec * x;
	calc_position();
}

/**
 * \brief Adjusts the distance to the pivot point
 * \param scroll_delta How much to adjust
 */
void Orbitcamera::handle_mouse_wheel(float scroll_delta)
{
	m_distance_ += scroll_delta * m_scrollspeed_;
	if(m_distance_ <= 1.0f)
		m_distance_ = 1.0f;
	calc_position();
}

/**
 * \brief Set the camera to its starting position
 */
void Orbitcamera::reset_position()
{
	m_lookat_point_ = default_lookat;
	m_azimuth_= default_azimuth;
	m_elevation_ = default_elevation;
	m_distance_ = default_distance;
	calc_position();
}
