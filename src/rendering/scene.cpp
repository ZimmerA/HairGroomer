#include "scene.h"

#include <QOpenGLShaderProgram>
#include <qopenglfunctions.h>
#include <QOpenGLFunctions_3_3_Core>

void Scene::load()
{
	load_shaders();
	load_textures();
	create_quad_vao();
	create_floor_grid();
	m_growth_mesh_index = 0;
	m_drawbuffer.create();
	m_hair_output_vbo.create();
	m_hair_output_vbo.setUsagePattern(QOpenGLBuffer::StaticRead);
}

void Scene::reset()
{
	m_should_reset = true;
}

void Scene::load_shaders()
{
	auto* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	// Draws the growth-mesh
	m_default_shader = std::make_unique<QOpenGLShaderProgram>();
	m_default_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/rendering/shaders/viewport/mesh.vert");
	m_default_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/rendering/shaders/viewport/mesh.frag");
	m_default_shader->bindAttributeLocation("aVertex", 0);
	m_default_shader->bindAttributeLocation("aNormal", 1);
	m_default_shader->bindAttributeLocation("aTangent", 2);
	m_default_shader->bindAttributeLocation("aBitangent", 3);
	m_default_shader->bindAttributeLocation("aUV", 4);
	m_default_shader->link();

	// Draws the UV map of the loaded growth-mesh
	m_uv_map_shader = std::make_unique<QOpenGLShaderProgram>();
	m_uv_map_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/rendering/shaders/paintwindow/uvmap.vert");
	m_uv_map_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
	                                         "./src/rendering/shaders//paintwindow/uvmap.frag");
	m_uv_map_shader->bindAttributeLocation("aVertex", 0);
	m_uv_map_shader->bindAttributeLocation("aNormal", 1);
	m_uv_map_shader->bindAttributeLocation("aUV", 4);
	m_uv_map_shader->link();

	// used for drawing the framebuffer
	m_drawbuffer_shader = std::make_unique<QOpenGLShaderProgram>();
	m_drawbuffer_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
	                                             "./src/rendering/shaders/paintwindow/drawbuffer.vert");
	m_drawbuffer_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
	                                             "./src/rendering/shaders/paintwindow/drawbuffer.frag");
	m_drawbuffer_shader->bindAttributeLocation("aVertex", 0);
	m_drawbuffer_shader->bindAttributeLocation("aUV", 1);
	m_drawbuffer_shader->link();
	m_drawbuffer_shader->bind();
	// look for the framebuffer texture in texture location 0
	m_drawbuffer_shader->setUniformValue("screenTexture", 0);

	// used for drawing the paintbrush
	m_paintbrush_shader = std::make_unique<QOpenGLShaderProgram>();
	m_paintbrush_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,
	                                             "./src/rendering/shaders/paintwindow/paintbrush.vert");
	m_paintbrush_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,
	                                             "./src/rendering/shaders/paintwindow/paintbrush.frag");
	m_paintbrush_shader->bindAttributeLocation("aVertex", 0);
	m_paintbrush_shader->bindAttributeLocation("aUV", 1);
	m_paintbrush_shader->link();
	// look for the framebuffer texture in texture location 0
	m_paintbrush_shader->setUniformValue("screenTexture", 0);

	// used for drawing the hair
	m_hair_shader = std::make_unique<QOpenGLShaderProgram>();
	m_hair_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/rendering/shaders/hair/hair.vert");
	m_hair_shader->addShaderFromSourceFile(QOpenGLShader::Geometry, "./src/rendering/shaders/hair/hair.geom");
	m_hair_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/rendering/shaders/hair/hair.frag");
	m_hair_shader->bindAttributeLocation("aVertex", 0);
	m_hair_shader->bindAttributeLocation("aNormal", 1);
	m_hair_shader->bindAttributeLocation("aTangent", 2);
	m_hair_shader->bindAttributeLocation("aBitangent", 3);
	m_hair_shader->bindAttributeLocation("aUV", 4);
	// Setup transform feedback for saving the hair generated by the geometry shader
	const GLchar* feedback_varyings[] = {"worldPosOut"};
	f->glTransformFeedbackVaryings(m_hair_shader->programId(), 1, feedback_varyings, GL_INTERLEAVED_ATTRIBS);
	//Link the shader
	m_hair_shader->link();
	m_hair_shader->bind();
	// look for the framebuffer texture in texture location 0
	m_hair_shader->setUniformValue("hairMap", 0);
}

/**
 * \brief Load all relevant textures
 */
void Scene::load_textures()
{
	m_paint_brush_texture = std::make_unique<QOpenGLTexture>(QImage("./res/brush.png").mirrored());
	m_paint_brush_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	m_paint_brush_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_floor_grid_texture = std::make_unique<QOpenGLTexture>(QImage("./res/Textures/grid_transparent_black_new.png").mirrored());
	m_floor_grid_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	m_floor_grid_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_floor_grid_texture->setWrapMode(QOpenGLTexture::Repeat);
}

/**
* \brief Creates and fills the quad vao/vbo for rendering textures
*/
void Scene::create_quad_vao()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	const float quad_vertices[] = {
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	m_quad_vao.create();
	m_quad_vbo.create();

	QOpenGLVertexArrayObject::Binder vao_binder(&m_quad_vao);

	m_quad_vbo.bind();
	m_quad_vbo.allocate(quad_vertices, sizeof(quad_vertices));
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
	                         reinterpret_cast<void *>(2 * sizeof(GLfloat)));
	m_quad_vao.release();
}

void Scene::create_floor_grid()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

	const int uv_max = 16;
	const float grid_size_x = 128;
	const float grid_size_y = 128;
	const float grid_vertices[] = {
		// positions   // texCoords
		-grid_size_x/2.0f, grid_size_y/2.0f, 0.0f, uv_max,
		-grid_size_x/2.0f, -grid_size_y/2.0f, 0.0f, 0.0f,
		grid_size_x/2.0f, -grid_size_y/2.0f, uv_max, 0.0f,

		-grid_size_x/2.0f, grid_size_y/2.0f, 0.0f, uv_max,
		grid_size_x/2.0f, -grid_size_y/2.0f, uv_max, 0.0f,
		grid_size_x/2.0f, grid_size_y/2.0f, uv_max, uv_max
	};

	m_grid_vao.create();
	m_grid_vbo.create();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_grid_vao);

	m_grid_vbo.bind();
	m_grid_vbo.allocate(grid_vertices, sizeof(grid_vertices));
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
	                         reinterpret_cast<void *>(2 * sizeof(GLfloat)));
	m_grid_vao.release();
}

/**
 * \brief Resizes the vbo used for the transform feedback of the hair
 * \param size the new size in bytes
 */
void Scene::resize_hair_feedback_buffer(const int size)
{
	m_hair_output_vbo.bind();
	m_hair_output_vbo.allocate(nullptr, size);
}

void Scene::set_up_axis(const int i)
{
	m_model_matrix = glm::mat4(1);

	if(i == 1)
		m_model_matrix = rotate(m_model_matrix, glm::radians(-90.f), glm::vec3(1.0f,0.0f,0.0f));
}
