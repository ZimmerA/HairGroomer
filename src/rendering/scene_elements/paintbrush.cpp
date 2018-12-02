
#include <ext/matrix_transform.hpp>

#include "paintbrush.h"

/**
 * \brief Sets the default values and calculates the initial transform
 */
PaintBrush::PaintBrush() : m_brush_position_()
{
	m_brush_size_ = 0.10f;
	m_intensity_ = 1.0f;

	// Set hair length as default channel (red)
	m_colormask_[0] = true;
	m_colormask_[1] = m_colormask_[2] = false;

	update_transform();
}

/**
 * \brief Sets the brush size
 * \param size The new size
 */
void PaintBrush::set_brush_size(const float size)
{
	m_brush_size_ = size;
	update_transform();
}

/**
 * \brief Sets the brush intensity
 * \param intensity The new intensity
 */
void PaintBrush::set_brush_intensity(const float intensity)
{
	this->m_intensity_ = intensity;
	update_transform();
}

/**
 * \brief Sets wether the opposite mode should be on or off
 * \param value The new state of the opposite mode
 */
void PaintBrush::set_opposite_mode(const bool value) noexcept
{
	m_opposite_mode_ = value;
}

/**
 * \brief Sets wether the erase mode should be on or off
 * \param value The new state of the erase mode
 */
void PaintBrush::set_is_erasing(const bool value) noexcept
{
	m_is_erasing_ = value;
}

/**
 * \brief Sets wether the brush is currently being used to draw
 * \param value The new state of the draw mode
 */
void PaintBrush::set_is_drawing(bool value) noexcept
{
	m_is_drawing_ = value;
}

/**
 * \brief Sets the position of the brush in screen space
 * \param x The x position
 * \param y The y position
 */
void PaintBrush::set_position(const float x, const float y)
{
	m_brush_position_ = glm::vec2(x, y);
	update_transform();
}

/**
 * \brief Stores the color masking for painting with the brush
 * \param p The brush mode
 */
void PaintBrush::set_paintmode(const PaintMode p) noexcept
{
	m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;

	switch (p)
	{
	case PaintBrush::PaintMode::Length:
		m_colormask_[0] = true;
		break;
	case PaintBrush::PaintMode::Curl:
		m_colormask_[1] = true;
		break;
	case PaintBrush::PaintMode::Twist:
		m_colormask_[2] = true;
		break;
	}
}

/**
 * \brief Calculate the transform matrix of the brush
 */
void PaintBrush::update_transform()
{
	glm::vec2 uv = m_brush_position_ * 2.0f - 1.0f;
	uv.y = -uv.y;

	m_transform_ = glm::mat4(1.0f);
	m_transform_ = translate(m_transform_, glm::vec3(uv, 0.0f));
	m_transform_ = scale(m_transform_, glm::vec3(m_brush_size_));
}
