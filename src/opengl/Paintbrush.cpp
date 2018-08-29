#include "Paintbrush.h"
#include <qopenglfunctions.h>


Paintbrush::Paintbrush()
{
	m_brush_size_ = .10f;
	m_intensity_ = 1.0f;

	// Set hair length as default channel (red)
	m_colormask_[0] = true;
	m_colormask_[1] = m_colormask_[2] = false;
}

void Paintbrush::set_brush_size(const float size)
{
	m_brush_size_ = size;	
}

void Paintbrush::set_brush_intensity(const double d)
{
	this->m_intensity_ = d;
}

void Paintbrush::set_opposite_mode(const bool value)
{
	m_opposite_mode_ = value;
}

void Paintbrush::set_position(const float x, const float y)
{
	m_brush_position_ = vec2(x, y);
}

/**
 * \brief Stores the color masking for painting with the brush
 * \param p The brush mode
 */
void Paintbrush::set_paintmode(const paintmode p)
{
	switch(p)
	{
		case length:
			m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
			m_colormask_[0] = true;
			break;
		case tangent:
			m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
			m_colormask_[1] = true;
			break;
		case bitangent:
			m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
			m_colormask_[2] = true;
			break;
	}
}

/**
 * \brief Sets the transform matrix and uniforms in the shader before rendering the paintbrush on screen or into the drawbuffer
 * \param shader The shader program to set the uniforms
 * \param painting_to_framebuffer true if the paintbrush is used to render to the drawbuffer instead of the default framebuffer
 */
void Paintbrush::begin(QOpenGLShaderProgram* shader, const bool painting_to_framebuffer)
{
	m_transform_.setIdentity();
	m_transform_.translate(-1.0f, 1.0f);
	m_transform_.translate(m_brush_position_.x, m_brush_position_.y);
	m_transform_.scale(m_brush_size_);
	auto f = QOpenGLContext::currentContext()->functions();

	//TODO: wrapper class for shaders that stores uniformlocations instead of looking them up every tick
	f->glUniformMatrix4fv(shader->uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_transform_));

	float intensity = m_intensity_;

	if(painting_to_framebuffer)
	{
		// Additive blending
		f->glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		f->glColorMask(m_colormask_[0],m_colormask_[1], m_colormask_[2], GL_TRUE);
		if(m_opposite_mode_)
		{
			intensity = 1.0f - m_intensity_;
		}
	}

	f->glUniform1f(shader->uniformLocation("intensity"), intensity);
}

/**
 * \brief Reset the color mask
 */
void Paintbrush::end()
{
	auto f = QOpenGLContext::currentContext()->functions();
	// Disable color masking again
	f->glColorMask(GL_TRUE,GL_TRUE, GL_TRUE, GL_TRUE);
}


