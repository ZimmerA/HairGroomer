
#include "scene.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QMessageBox>

#include "glattributes.h"

static void _check_shader(const std::unique_ptr<QOpenGLShaderProgram> &shader, const QString &info = nullptr)
{
	const QString log = shader->log().trimmed();

	if (log.isEmpty())
		return;

	QString cause = log;

	if (!info.isEmpty())
		cause = info + "\n" + cause;

	QMessageBox::critical(nullptr, "Error", cause);

	exit(1);
}

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
	auto *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	// Draws the growth-mesh
	m_default_shader = std::make_unique<QOpenGLShaderProgram>();
	
	if (!m_default_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "res/shaders/viewport/mesh.vert"))
		_check_shader(m_default_shader, "Failed loading res/shaders/viewport/mesh.vert");
	if (!m_default_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "res/shaders/viewport/mesh.frag"))
		_check_shader(m_default_shader, "Failed loading res/shaders/viewport/mesh.frag");
	
	m_default_shader->bindAttributeLocation("aVertex", ATTRIBUTE_POSITION);
	m_default_shader->bindAttributeLocation("aNormal", ATTRIBUTE_NORMAL);
	m_default_shader->bindAttributeLocation("aTangent", ATTRIBUTE_TANGENT);
	m_default_shader->bindAttributeLocation("aBitangent", ATTRIBUTE_BITANGENT);
	m_default_shader->bindAttributeLocation("aUV", ATTRIBUTE_TEXCOORD);

	m_default_shader->link();
	_check_shader(m_default_shader, "Failed creating default_shader");


	// Draws the UV map of the loaded growth-mesh
	m_uv_map_shader = std::make_unique<QOpenGLShaderProgram>();

	if (!m_uv_map_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "res/shaders/paintwindow/uvmap.vert"))
		_check_shader(m_uv_map_shader, "Failed loading res/shaders/paintwindow/uvmap.vert");
	if (!m_uv_map_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "res/shaders/paintwindow/uvmap.frag"))
		_check_shader(m_uv_map_shader, "Failed loading res/shaders/paintwindow/uvmap.frag");

	m_uv_map_shader->bindAttributeLocation("aVertex", ATTRIBUTE_POSITION);
	m_uv_map_shader->bindAttributeLocation("aNormal", ATTRIBUTE_NORMAL);
	m_uv_map_shader->bindAttributeLocation("aUV", ATTRIBUTE_TEXCOORD);
	
	m_uv_map_shader->link();
	_check_shader(m_uv_map_shader, "Failed creating uv_map_shader");


	// Used for drawing the framebuffer
	m_drawbuffer_shader = std::make_unique<QOpenGLShaderProgram>();

	if (!m_drawbuffer_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "res/shaders/paintwindow/drawbuffer.vert"))
		_check_shader(m_drawbuffer_shader, "Failed loading res/shaders/paintwindow/drawbuffer.vert");
	if (!m_drawbuffer_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "res/shaders/paintwindow/drawbuffer.frag"))
		_check_shader(m_drawbuffer_shader, "Failed loading res/shaders/paintwindow/drawbuffer.frag");

	m_drawbuffer_shader->bindAttributeLocation("aVertex", ATTRIBUTE_POSITION);
	m_drawbuffer_shader->bindAttributeLocation("aUV", ATTRIBUTE_TEXCOORD);

	m_drawbuffer_shader->link();
	_check_shader(m_drawbuffer_shader, "Failed creating drawbuffer_shader");

	m_drawbuffer_shader->bind();

	// Look for the framebuffer texture in texture location 0
	m_drawbuffer_shader->setUniformValue("screenTexture", 0);


	// Used for drawing the paintbrush
	m_paintbrush_shader = std::make_unique<QOpenGLShaderProgram>();

	if (!m_paintbrush_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "res/shaders/paintwindow/paintbrush.vert"))
		_check_shader(m_paintbrush_shader, "Failed loading res/shaders/paintwindow/paintbrush.vert");
	if (!m_paintbrush_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "res/shaders/paintwindow/paintbrush.frag"))
		_check_shader(m_paintbrush_shader, "Failed loading res/shaders/paintwindow/paintbrush.frag");

	m_paintbrush_shader->bindAttributeLocation("aVertex", ATTRIBUTE_POSITION);
	m_paintbrush_shader->bindAttributeLocation("aUV", ATTRIBUTE_TEXCOORD);

	m_paintbrush_shader->link();
	_check_shader(m_paintbrush_shader, "Failed creating paintbrush_shader");

	// Look for the framebuffer texture in texture location 0
	m_paintbrush_shader->setUniformValue("screenTexture", 0);


	// Used for drawing the hair
	m_hair_shader = std::make_unique<QOpenGLShaderProgram>();
	
	if (!m_hair_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "res/shaders/hair/hair.vert"))
		_check_shader(m_hair_shader, "Failed loading res/shaders/hair/hair.vert");
	if (!m_hair_shader->addShaderFromSourceFile(QOpenGLShader::Geometry, "res/shaders/hair/hair.geom"))
		_check_shader(m_hair_shader, "Failed loading res/shaders/hair/hair.geom");
	if (!m_hair_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "res/shaders/hair/hair.frag"))
		_check_shader(m_hair_shader, "Failed loading res/shaders/hair/hair.frag");

	m_hair_shader->bindAttributeLocation("aVertex", ATTRIBUTE_POSITION);
	m_hair_shader->bindAttributeLocation("aNormal", ATTRIBUTE_NORMAL);
	m_hair_shader->bindAttributeLocation("aTangent", ATTRIBUTE_TANGENT);
	m_hair_shader->bindAttributeLocation("aBitangent", ATTRIBUTE_BITANGENT);
	m_hair_shader->bindAttributeLocation("aUV", ATTRIBUTE_TEXCOORD);

	// Setup transform feedback for saving the hair generated by the geometry shader
	const GLchar *feedback_varyings[] = { "worldPosOut" };
	f->glTransformFeedbackVaryings(m_hair_shader->programId(), 1, feedback_varyings, GL_INTERLEAVED_ATTRIBS);
	
	// Link the shader
	m_hair_shader->link();
	_check_shader(m_hair_shader, "Failed creating hair_shader");

	m_hair_shader->bind();
	
	// Look for the framebuffer texture in texture location 0
	m_hair_shader->setUniformValue("hairMap", 0);
}

/**
 * \brief Load all relevant textures
 */
void Scene::load_textures()
{
	m_paint_brush_texture = std::make_unique<QOpenGLTexture>(QImage("res/brush.png").mirrored());
	m_paint_brush_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	m_paint_brush_texture->setMagnificationFilter(QOpenGLTexture::Linear);

	m_floor_grid_texture = std::make_unique<QOpenGLTexture>(QImage("res/textures/grid_transparent_black_new.png").mirrored());
	m_floor_grid_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	m_floor_grid_texture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_floor_grid_texture->setWrapMode(QOpenGLTexture::Repeat);
}

/**
* \brief Creates and fills the quad vao/vbo for rendering textures
*/
void Scene::create_quad_vao()
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

	static constexpr float quad_vertices[] = {
		// X, Y, U, V
		-1.0f, +1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		+1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, +1.0f, 0.0f, 1.0f,
		+1.0f, -1.0f, 1.0f, 0.0f,
		+1.0f, +1.0f, 1.0f, 1.0f
	};

	m_quad_vao.create();
	m_quad_vbo.create();

	QOpenGLVertexArrayObject::Binder vao_binder(&m_quad_vao);

	m_quad_vbo.bind();
	m_quad_vbo.allocate(quad_vertices, sizeof(quad_vertices));

	f->glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	f->glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
	f->glVertexAttribPointer(ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	f->glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<const void*>(2 * sizeof(GLfloat)));

	m_quad_vao.release();
}

void Scene::create_floor_grid()
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

	static constexpr int uv_max = 16;
	static constexpr float grid_size_x = 128;
	static constexpr float grid_size_y = 128;
	static constexpr float grid_vertices[] = {
		// X, Y, U, V
		-grid_size_x / 2.0f, +grid_size_y / 2.0f, 0.0f, uv_max,
		-grid_size_x / 2.0f, -grid_size_y / 2.0f, 0.0f, 0.0f,
		+grid_size_x / 2.0f, -grid_size_y / 2.0f, uv_max, 0.0f,

		-grid_size_x / 2.0f, +grid_size_y / 2.0f, 0.0f, uv_max,
		+grid_size_x / 2.0f, -grid_size_y / 2.0f, uv_max, 0.0f,
		+grid_size_x / 2.0f, +grid_size_y / 2.0f, uv_max, uv_max
	};

	m_grid_vao.create();
	m_grid_vbo.create();

	QOpenGLVertexArrayObject::Binder vao_binder(&m_grid_vao);

	m_grid_vbo.bind();
	m_grid_vbo.allocate(grid_vertices, sizeof(grid_vertices));

	f->glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	f->glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);
	f->glVertexAttribPointer(ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	f->glVertexAttribPointer(ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<const void*>(2 * sizeof(GLfloat)));

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

	if (i == 1)
		m_model_matrix = rotate(m_model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}
