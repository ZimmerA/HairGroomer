#ifndef SCENE_H
#define SCENE_H

#include "scene_elements/orbitcamera.h"
#include "scene_elements/paintbrush.h"
#include "scene_elements/glModel.h"
#include "scene_elements/glDrawbuffer.h"
#include "scene_elements/light.h"
#include "scene_elements/hairSettings.h"

#include <QOpenGLTexture>
#include <glm.hpp>
#include <qglobal.h>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Scene
{
public:
	void load();
	void resize_hair_feedback_buffer(int size);

	// mvp matrices used by hair and meshes
	glm::mat4 m_defaultmodel_matrix{1};
	glm::mat4 m_defaultview_matrix{1};
	glm::mat4 m_defaultprojection_matrix{1};

	// Shader programs
	std::unique_ptr<QOpenGLShaderProgram> m_default_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_uv_map_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_drawbuffer_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_hair_shader;
	std::unique_ptr<QOpenGLShaderProgram> m_paintbrush_shader;

	// Drawbuffer for painting to 
	GlDrawbuffer m_drawbuffer;
	QOpenGLBuffer m_hair_output_vbo;

	// Quad for rendering textures
	QOpenGLVertexArrayObject m_quad_vao;
	QOpenGLBuffer m_quad_vbo;
	
	// Textures
	std::unique_ptr<QOpenGLTexture> m_paint_brush_texture;

	/* Scene Elements */
	Orbitcamera m_camera;
	Light m_light;
	HairSettings m_hair;
	Paintbrush m_brush;
	GlModel m_fbx_glmodel;

	// index of the growthmesh in the fbx model
	int m_growth_mesh_index;

private:
	void load_shaders();
	void load_textures();
	void create_quad_vao();
};

#endif

