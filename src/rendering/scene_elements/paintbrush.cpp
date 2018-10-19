#include "paintbrush.h"

Paintbrush::Paintbrush()
{
	m_brush_size_ = .10f;
	m_intensity_ = 1.0f;

	// Set hair length as default channel (red)
	m_colormask_[0] = true;
	m_colormask_[1] = m_colormask_[2] = false;
	m_transform_.setIdentity();
	m_transform_.translate(-1.0f, 1.0f);
	m_transform_.translate(m_brush_position_.x, m_brush_position_.y);
	m_transform_.scale(m_brush_size_);
}

void Paintbrush::set_brush_size(const float size)
{
	m_brush_size_ = size;
	m_transform_.setIdentity();
	m_transform_.translate(-1.0f, 1.0f);
	m_transform_.translate(m_brush_position_.x, m_brush_position_.y);
	m_transform_.scale(m_brush_size_);
}

void Paintbrush::set_brush_intensity(const float d)
{
	this->m_intensity_ = d;
	m_transform_.setIdentity();
	m_transform_.translate(-1.0f, 1.0f);
	m_transform_.translate(m_brush_position_.x, m_brush_position_.y);
	m_transform_.scale(m_brush_size_);
}

void Paintbrush::set_opposite_mode(const bool value)
{
	m_opposite_mode_ = value;
}

void Paintbrush::set_position(const float x, const float y)
{
	m_brush_position_ = vec2(x, y);
	m_transform_.setIdentity();
	m_transform_.translate(-1.0f, 1.0f);
	m_transform_.translate(m_brush_position_.x, m_brush_position_.y);
	m_transform_.scale(m_brush_size_);
}

/**
 * \brief Stores the color masking for painting with the brush
 * \param p The brush mode
 */
void Paintbrush::set_paintmode(const paintmode p)
{
	switch (p)
	{
	case length:
		m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
		m_colormask_[0] = true;
		break;
	case curl:
		m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
		m_colormask_[1] = true;
		break;
	case twist:
		m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
		m_colormask_[2] = true;
		break;
	}
}