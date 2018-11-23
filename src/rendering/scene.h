#ifndef SCENE_H
#define SCENE_H

#include "scene_elements/orbitcamera.h"
#include "scene_elements/paintbrush.h"
#include "scene_elements/glModel.h"
#include "scene_elements/glDrawbuffer.h"
#include "scene_elements/light.h"
#include "scene_elements/hairSettings.h"

#include <QOpenGLTexture>
#include <qglobal.h>

#include <glm.hpp>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Scene
{
public:

	void load();
	void reset();
	void resize_hair_feedback_buffer(int size);
	void set_up_axis(int i);

	// MVP matrices used by hair and meshes
	glm::mat4 m_model_matrix{1};
	glm::mat4 m_view_matrix{1};
	glm::mat4 m_projection_matrix{1};

	// Shader programs
	std::unique_ptr<QOpenGLShaderProgram> m_default_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_uv_map_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_drawbuffer_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_hair_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_paintbrush_shader;

	// Drawbuffer for painting to 
	GLDrawbuffer m_drawbuffer;
	QOpenGLBuffer m_hair_output_vbo;

	// Quad for rendering textures
	QOpenGLVertexArrayObject m_quad_vao;
	QOpenGLBuffer m_quad_vbo;

	// Quad for rendering the floor grid
	QOpenGLVertexArrayObject m_grid_vao;
	QOpenGLBuffer m_grid_vbo;

	// Textures
	std::unique_ptr<QOpenGLTexture> m_paint_brush_texture;
	std::unique_ptr<QOpenGLTexture> m_floor_grid_texture;

	// Scene Elements
	OrbitCamera m_camera;
	Light m_light;
	HairSettings m_hair;
	PaintBrush m_brush;
	GLModel m_fbx_glmodel;

	// index of the growthmesh in the fbx model
	int m_growth_mesh_index;

	bool m_should_reset{false};

private:

	void load_shaders();
	void load_textures();
	void create_quad_vao();
	void create_floor_grid();
};

#endif