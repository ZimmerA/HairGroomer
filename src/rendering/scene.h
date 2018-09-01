#ifndef SCENE_H
#define SCENE_H
#include "orbitcamera.h"
#include "glModel.h"

/**
 * \brief stores general light information
 */
struct Light
{
	vec3 m_position = vec3(0.f,550.0f,0.f);
	vec3 m_color;
};

class Scene
{
public:
	Orbitcamera m_camera;

	mat4 m_model_matrix;
	mat4 m_projection_matrix;
	Light m_light;
	QOpenGLShaderProgram m_mesh_shader;
	bool m_should_light_mesh;

	bool m_should_render_grwothmesh;
	bool m_should_render_referencemodel;

	GlModel m_growth_mesh;
	GlModel m_reference_model;
};

#endif

