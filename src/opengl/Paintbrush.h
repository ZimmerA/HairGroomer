#pragma once
#include <linalg.hpp>
#include <qopenglshaderprogram.h>

class Paintbrush
{
public:
	enum paintmode
	{
		length = 0,
		tangent,
		bitangent
	};

	Paintbrush();

	void set_brush_size(float size);
	void set_brush_intensity(double d);
	void set_erase_mode(bool value);
	void set_position(float x, float y);
	void set_paintmode(paintmode p);

	void begin(QOpenGLShaderProgram* shader, bool painting_to_framebuffer);
	static void end();

private: 
	float m_brush_size_;
	float m_intensity_;
	vec2 m_brush_position_;
	mat4 m_transform_; // for rendering
	bool m_colormask_[3]{}; // Which color channels to draw to
	bool m_erase_mode_ = false; // Are we currently erasing? (intensity 0)
};

