#ifndef PAINTBRUSH_H
#define PAINTBRUSH_H
#include <glm.hpp>

/**
 * \brief Stores the settings of the paintbrush, used by the rendering
 */
class Paintbrush
{
public:
	enum paintmode
	{
		length = 0,
		curl,
		twist
	};
	Paintbrush();

	void set_brush_size(float size);
	void set_brush_intensity(float d);
	void set_opposite_mode(bool value) noexcept;
	void set_position(float x, float y);
	void set_paintmode(paintmode p) noexcept;
	
	const glm::mat4& get_transform() const noexcept{ return m_transform_;}
	float get_intensity() const noexcept {return m_intensity_;}
	bool get_opposite_mode() const noexcept{return m_opposite_mode_;} 
	bool* get_colormask() noexcept {return m_colormask_;}

private: 
	float m_brush_size_;
	float m_intensity_;
	glm::vec2 m_brush_position_;
	glm::mat4 m_transform_{1}; // for rendering
	bool m_colormask_[3]{}; // Which color channels to draw to
	bool m_opposite_mode_ = false; // Are we currently bending in the opposite direction
};
#endif

