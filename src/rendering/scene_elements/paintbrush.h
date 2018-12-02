#ifndef PAINTBRUSH_H
#define PAINTBRUSH_H

#include <glm.hpp>

/**
 * \brief Stores the settings of the paintbrush, used by the rendering
 */
class PaintBrush
{
public:

	enum class PaintMode
	{
		Length,
		Curl,
		Twist
	};

	PaintBrush();

	// Getters/Setters
	const glm::mat4& get_transform() const noexcept { return m_transform_; }
	float get_size() const noexcept { return m_brush_size_; }
	float get_intensity() const noexcept { return m_intensity_; }
	bool get_opposite_mode() const noexcept { return m_opposite_mode_; }
	bool get_is_erasing() const noexcept { return m_is_erasing_; }
	bool get_is_drawing() const noexcept { return m_is_drawing_; }
	bool* get_colormask() noexcept { return m_colormask_; }
	
	void set_brush_size(float size);
	void set_brush_intensity(float d);
	void set_opposite_mode(bool value) noexcept;
	void set_is_erasing(bool value) noexcept;
	void set_is_drawing(bool value) noexcept;
	void set_position(float x, float y);
	void set_paintmode(PaintMode p) noexcept;

private:

	void update_transform();

	float m_brush_size_;
	float m_intensity_;
	glm::vec2 m_brush_position_;
	glm::mat4 m_transform_{1}; // For rendering
	bool m_colormask_[3]{}; // Which color channels to draw to
	bool m_opposite_mode_ = false; // Are we currently bending in the opposite direction
	bool m_is_erasing_ = false;
	bool m_is_drawing_ = false;
};

#endif