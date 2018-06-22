#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <mvppresenter.h>
#include <qapplication.h>

GlWidget::GlWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	m_core_ = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;

	// Find our parent window (which is the view in the mvp design pattern)
	foreach(QWidget *widget, QApplication::topLevelWidgets())
	{
		if (auto* v = dynamic_cast<MainWindow*>(widget))
			m_view_ = v;
	}
}
/**
* \brief The destructor, used to call cleanup()
*/
GlWidget::~GlWidget()
{
	cleanup();
}

/**
 * \brief Clean up the buffers and shaderprograms
 */
void GlWidget::cleanup()
{
	if (m_default_shader_ == nullptr)
		return;

	makeCurrent();
	m_test_model_.cleanup_model();
	m_drawbuffer_quad_vbo_.destroy();

	delete m_default_shader_;
	m_default_shader_ = nullptr;
	delete m_uv_map_shader_;
	m_uv_map_shader_ = nullptr;
	delete m_drawbuffer_shader_;
	m_drawbuffer_shader_ = nullptr;
	delete m_hair_shader_;
	m_hair_shader_ = nullptr;
	delete m_drawbuffer_;
	m_drawbuffer_ = nullptr;
	doneCurrent();
}


/**
 * \brief Initialize anything opengl related
 */
void GlWidget::initializeGL()
{
	// Connect cleanup function for when the current context gets destroyed
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GlWidget::cleanup);
	// QT function
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	// Enable Scissor test for viewport splitting
	glEnable(GL_SCISSOR_TEST);
	// Setup shaders
	setup_shaders();
	create_drawbuffer();
	create_drawbuffer_quad();

	// setup model using the vertex data in our mvpModel
	m_test_model_.setup_model(m_view_->get_presenter()->get_model()->get_reference_model());

}

/**
 * \brief Compiles and links the shader programs, sets up uniform and attribute locations 
 */
void GlWidget::setup_shaders()
{
	// Draws the growthmesh with phong lighting
	m_default_shader_ = new QOpenGLShaderProgram;
	m_default_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vert.vert");
	m_default_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/frag.frag");
	m_default_shader_->bindAttributeLocation("aVertex", 0);
	m_default_shader_->bindAttributeLocation("aNormal", 1);
	m_default_shader_->bindAttributeLocation("aUV", 2);
	m_default_shader_->link();
	m_default_shader_->bind();

	m_proj_loc_ = m_default_shader_->uniformLocation("projection");
	m_view_loc_ = m_default_shader_->uniformLocation("view");
	m_model_loc_ = m_default_shader_->uniformLocation("model");
	setup_mvp();

	// Draws the UV map of the loaded growthmesh
	m_uv_map_shader_ = new QOpenGLShaderProgram;
	m_uv_map_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/uvmap.vert");
	m_uv_map_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uvmap.frag");
	m_uv_map_shader_->bindAttributeLocation("aVertex", 0);
	m_uv_map_shader_->bindAttributeLocation("aNormal", 1);
	m_uv_map_shader_->bindAttributeLocation("aUV", 2);
	m_uv_map_shader_->link();

	// used for drawing the framebuffer
	m_drawbuffer_shader_ = new QOpenGLShaderProgram;
	m_drawbuffer_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/drawbuffer.vert");
	m_drawbuffer_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/drawbuffer.frag");
	m_drawbuffer_shader_->bindAttributeLocation("aVertex", 0);
	m_drawbuffer_shader_->bindAttributeLocation("aUV", 1);
	m_drawbuffer_shader_->link();
	m_drawbuffer_shader_->bind();
	m_drawbuffer_shader_->setUniformValue("screenTexture", 0);
}

/**
 * \brief Draws the opengl context
 */
void GlWidget::paintGL()
{
	m_drawbuffer_->bind();
	glViewport(0, 0, width(), height());
	glScissor(0, 0, width(), height());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_default_shader_->bind();
	m_test_model_.draw(m_default_shader_);
	m_default_shader_->release();
	m_drawbuffer_->release();

	// Left half of context (Mesh)
	glViewport(0, 0, width() / 2, height());
	glScissor(0, 0, width() / 2, height());
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_default_shader_->bind();
	m_test_model_.draw(m_default_shader_);
	m_default_shader_->release();


	// Right half of context (UVmap/drawing window)
	glViewport(width() / 2, 0, width() / 2, height());
	glScissor(width() / 2, 0, width(), height());
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_uv_map_shader_->bind();
	m_test_model_.draw(m_default_shader_);
	m_uv_map_shader_->release();
	*/

	render_drawbuffer();
	
}

/**
* \brief Renders the framebuffer content to a quad.
*/
void GlWidget::render_drawbuffer()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_drawbuffer_shader_->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	QOpenGLVertexArrayObject::Binder vao_binder(&m_drawbuffer_quad_vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_drawbuffer_shader_->release();
}

void GlWidget::create_drawbuffer()
{
	m_drawbuffer_ = new QOpenGLFramebufferObject(width()/2, height());
}

/**
* \brief Creates and fills the vao/vbo for the rendering of the framebuffer content
*/
void GlWidget::create_drawbuffer_quad()
{
	float quad_vertices[] = {
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	m_drawbuffer_quad_vao_.create();
	m_drawbuffer_quad_vbo_.create();

	QOpenGLVertexArrayObject::Binder vao_binder(&m_drawbuffer_quad_vao_);

	m_drawbuffer_quad_vbo_.bind();
	m_drawbuffer_quad_vbo_.allocate(quad_vertices, sizeof(quad_vertices));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void *>(2 * sizeof(GLfloat)));
	m_drawbuffer_quad_vao_.release();
}


/**
 * \brief Corrects the perspective matrix after resizing the window
 * \param w The new width of the Window
 * \param h The new height of the Window
 */
void GlWidget::resizeGL(const int w, const int h)
{
	m_projection_matrix_.setIdentity();
	m_projection_matrix_ = mat4::perspective(45.0f, w / float(h) / 2.0f, 0.1f, 1000.0f);

	m_default_shader_->bind();
	glUniformMatrix4fv(m_proj_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_projection_matrix_));
	m_default_shader_->release();
}


/**
 * \brief Sets up the model view projection uniforms of the standard shader
 * \note we have to use glUniformMatrix4fv here (instead of the QT wrapper function)
 *		 because the QT shader wrapper doesnt have a uniform function that takes a float * as parameter
 *		 and hands it to glUniformMatrix(n)fv which is needed for our math library (linalg)
 */
void GlWidget::setup_mvp()
{
	m_view_matrix_.setIdentity();
	m_view_matrix_.translate(0, 0, -40.0f);

	glUniformMatrix4fv(m_view_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_view_matrix_));

	m_model_matrix_.setIdentity();
	glUniformMatrix4fv(m_model_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_model_matrix_));

	m_projection_matrix_ = mat4::perspective(45.0f, (width() / 2.0f) / float(height()), 0.1f, 1000.0f);
	glUniformMatrix4fv(m_proj_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_projection_matrix_));
}


