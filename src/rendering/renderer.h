#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_3_Core>

#include "scene.h"
#include "baseapp/hairData.h"

class GLWidget;

class Renderer : protected QOpenGLFunctions_3_3_Core
{
public:

	bool m_should_render_growthmesh = false;
	bool m_should_render_refrencemodel = true;
	bool m_should_render_uv_overlay = true;

	// Determines wether transform feedback should be used to write the hair to a buffer
	bool m_should_write_out_hair = false;

	// Stores the hair data retrieved by transform feedback
	HairData m_hairdata_cache;

	void init(int width, int height);
	void render_scene();

	// Getters/Setters
	void set_measurements(const int width, const int height) noexcept { m_width_ = width; m_height_ = height;}
	void set_current_scene(Scene *scene) noexcept { m_current_scene_ = scene; }

private:

	int m_width_ = 0;
	int m_height_ = 0;
	Scene * m_current_scene_ = nullptr;
};

#endif