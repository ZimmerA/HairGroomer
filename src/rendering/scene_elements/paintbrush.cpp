
#include <ext/matrix_transform.hpp>

#include "paintbrush.h"

PaintBrush::PaintBrush() : m_brush_position_()
{
	m_brush_size_ = .10f;
	m_intensity_ = 1.0f;

	// Set hair length as default channel (red)
	m_colormask_[0] = true;
	m_colormask_[1] = m_colormask_[2] = false;

	_update_transform();
}

void PaintBrush::set_brush_size(const float size)
{
	m_brush_size_ = size;
	_update_transform();
}

void PaintBrush::set_brush_intensity(const float d)
{
	this->m_intensity_ = d;
	_update_transform();
}

void PaintBrush::set_opposite_mode(const bool value) noexcept
{
	m_opposite_mode_ = value;
}

void PaintBrush::set_position(const float x, const float y)
{
	m_brush_position_ = glm::vec2(x, y);
	_update_transform();
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

void PaintBrush::_update_transform()
{
	glm::vec2 uv = m_brush_position_ * 2.0f - 1.0f;
	uv.y = -uv.y;

	m_transform_ = glm::mat4(1.0f);
	m_transform_ = translate(m_transform_, glm::vec3(uv, 0.0f));
	m_transform_ = scale(m_transform_, glm::vec3(m_brush_size_));
}
