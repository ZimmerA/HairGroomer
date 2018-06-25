#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <mvppresenter.h>
#include <qapplication.h>
#include <QDebug>

GlWidget::GlWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	m_core_ = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
	setMouseTracking(true);
	setFocus();
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
	m_quad_vbo_.destroy();

	delete m_default_shader_;
	m_default_shader_ = nullptr;
	delete m_paintbrush_shader_;
	m_paintbrush_shader_ = nullptr;
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
	// QT function for initializing opengl
	initializeOpenGLFunctions();

	// Enable Scissor test for viewport splitting
	glEnable(GL_SCISSOR_TEST);

	// Compile and link shaders, set relavant uniforms
	setup_shaders();
	// Create the framebuffer for hair drawing
	create_drawbuffer();
	// load all relevant textures
	load_textures();

	// Create a quad for rendering textures
	create_quad_vao();

	m_paintbrush_model_matrix_.scale(0.05f);

	// setup model using the vertex data in our mvpModel
	m_test_model_.setup_model(m_view_->get_presenter()->get_model()->get_reference_model());
	m_colormask_[0] = true;
	m_colormask_[1] = m_colormask_[2] = false;
}

/**
 * \brief Compiles and links the shader programs, sets up uniform and attribute locations 
 */
void GlWidget::setup_shaders()
{
	// Draws the growthmesh with phong lighting
	m_default_shader_ = new QOpenGLShaderProgram;
	m_default_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/shaders/mesh.vert");
	m_default_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/shaders/mesh.frag");
	m_default_shader_->bindAttributeLocation("aVertex", 0);
	m_default_shader_->bindAttributeLocation("aNormal", 1);
	m_default_shader_->bindAttributeLocation("aUV", 2);
	m_default_shader_->link();
	m_default_shader_->bind();

	m_defaultproj_loc_ = m_default_shader_->uniformLocation("projection");
	m_defaultview_loc_ = m_default_shader_->uniformLocation("view");
	m_defaultmodel_loc_ = m_default_shader_->uniformLocation("model");
	setup_mvp();

	// Draws the UV map of the loaded growthmesh
	m_uv_map_shader_ = new QOpenGLShaderProgram;
	m_uv_map_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/shaders/uvmap.vert");
	m_uv_map_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/shaders/uvmap.frag");
	m_uv_map_shader_->bindAttributeLocation("aVertex", 0);
	m_uv_map_shader_->bindAttributeLocation("aNormal", 1);
	m_uv_map_shader_->bindAttributeLocation("aUV", 2);
	m_uv_map_shader_->link();

	// used for drawing the framebuffer
	m_drawbuffer_shader_ = new QOpenGLShaderProgram;
	m_drawbuffer_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/shaders/drawbuffer.vert");
	m_drawbuffer_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/shaders/drawbuffer.frag");
	m_drawbuffer_shader_->bindAttributeLocation("aVertex", 0);
	m_drawbuffer_shader_->bindAttributeLocation("aUV", 1);
	m_drawbuffer_shader_->link();
	// look for the framebuffer texture in texture location 0
	m_drawbuffer_shader_->setUniformValue("screenTexture", 0);

	// used for drawing the paintbrush
	m_paintbrush_shader_ = new QOpenGLShaderProgram;
	m_paintbrush_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/shaders/paintbrush.vert");
	m_paintbrush_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/shaders/paintbrush.frag");
	m_paintbrush_shader_->bindAttributeLocation("aVertex", 0);
	m_paintbrush_shader_->bindAttributeLocation("aUV", 1);
	m_paintbrush_shader_->link();
	// look for the framebuffer texture in texture location 0
	m_paintbrush_shader_->setUniformValue("screenTexture", 0);
	m_paintbrushmodel_loc_ = m_paintbrush_shader_->uniformLocation("model");

	// used for drawing the hair
	m_hair_shader_ = new QOpenGLShaderProgram;
	m_hair_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/shaders/hair.vert");
	m_hair_shader_->addShaderFromSourceFile(QOpenGLShader::Geometry, "./src/shaders/hair.geom");
	m_hair_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/shaders/hair.frag");
	m_hair_shader_->bindAttributeLocation("aVertex", 0);
	m_hair_shader_->bindAttributeLocation("aNormal", 1);
	m_hair_shader_->bindAttributeLocation("aUV", 2);
	// look for the framebuffer texture in texture location 0
	m_hair_shader_->link();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("hairMap", 0);
}

/**
 * \brief Draws the opengl context
 */
void GlWidget::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// Left half of context (Mesh)
	render_left_half();

	// Right half of context (UVmap/drawing window)
	render_right_half();
}

/**
 * \brief Renders the left half of the viewport
 */
void GlWidget::render_left_half()
{
	glViewport(0, 0, width() / 2, height());
	glScissor(0, 0, width() / 2, height());
	// we dont want the mesh to be a wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// draw the growth mesh
	m_default_shader_->bind();
	m_test_model_.draw(m_default_shader_);
	m_default_shader_->release();


	// draw the hair
	m_hair_shader_->bind();
	mat4 mvp = m_defaultprojection_matrix_ * m_defaultview_matrix_ * m_defaultmodel_matrix_;
	mat4 vp = m_defaultprojection_matrix_ * m_defaultview_matrix_;
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultmodel_matrix_));
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("view"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultview_matrix_));
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("mvp"), 1, GL_FALSE, reinterpret_cast<GLfloat *>(&mvp));
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("vp"), 1, GL_FALSE, reinterpret_cast<GLfloat *>(&vp));
	m_hair_shader_->setUniformValue("maxHairLength", 2.0f);
	// Bind framebuffer texture to texunit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	m_test_model_.draw(m_hair_shader_);
	m_hair_shader_->release();
}

/**
 * \brief Renders the right half of the viewport
 */
void GlWidget::render_right_half()
{
	// Disable depth testing cause of alpha blending (were only drawing textures anyways)
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// draw brush to framebuffer
	if (m_is_leftmouse_pressed_)
		paint_to_frame_buffer();

	// Split the Viewport
	glViewport(width() / 2, 0, width() / 2, height());
	glScissor(width() / 2, 0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	// Draw the Texture of the framebuffer to a quad
	render_drawbuffer();

	// Draw the UV map of the growth mesh
	render_uv_map();
	// Draw the paintbrush at the mouse position
	render_paintbrush();

	// Disable alpha blending for the next frame
	glDisable(GL_BLEND);
	// Enable depth testing for coming frames
	glEnable(GL_DEPTH_TEST);
}

/**
 * \brief Draws the paintbrush at its current position into the framebuffer
 */
void GlWidget::paint_to_frame_buffer()
{
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	m_drawbuffer_->bind();
	glViewport(0, 0, 800, 600);
	glScissor(0, 0, 800, 600);
	glColorMask(m_colormask_[0], m_colormask_[1], m_colormask_[2], GL_TRUE);
	// Draw the paintbrush into the framebuffer at the mouse position
	render_paintbrush();
	m_drawbuffer_->release();
	// Disable Colormask again
	glColorMask(GL_TRUE,GL_TRUE, GL_TRUE, GL_TRUE);
}

/**
 * \brief Uses the uvmap shader to draw the uvmap of the growthmesh in wireframe mode
 */
void GlWidget::render_uv_map()
{
	// Draw the uv map (wireframe)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_uv_map_shader_->bind();
	m_test_model_.draw(m_default_shader_);
	m_uv_map_shader_->release();
	// Disable wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
* \brief Renders the draw framebuffer texture to a quad.
*/
void GlWidget::render_drawbuffer()
{
	// Bind framebuffer texture to texunit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	// Draw Texture
	m_drawbuffer_shader_->bind();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_quad_vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_drawbuffer_shader_->release();
}

/**
 * \brief draws the paintbrush to the main framebuffer
 */
void GlWidget::render_paintbrush()
{
	// bind paintbrush texture to texunit 0
	glActiveTexture(GL_TEXTURE0);
	m_paint_brush_texture_->bind();
	// draw the brush
	m_paintbrush_shader_->bind();
	glUniformMatrix4fv(m_paintbrushmodel_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_paintbrush_model_matrix_));
	QOpenGLVertexArrayObject::Binder vao_binder(&m_quad_vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_paintbrush_shader_->release();
}

/**
 * \brief Load all relevant textures
 */
void GlWidget::load_textures()
{
	m_paint_brush_texture_ = new QOpenGLTexture(QImage("./res/brush.png").mirrored());
	m_paint_brush_texture_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	m_paint_brush_texture_->setMagnificationFilter(QOpenGLTexture::Linear);
}

/**
 * \brief Creates the framebuffer for drawing with the paintbrush
 */
void GlWidget::create_drawbuffer()
{
	m_drawbuffer_ = new QOpenGLFramebufferObject(800, 600);
	m_drawbuffer_->bind();
	glClearColor(0.f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_drawbuffer_->release();
}

/**
* \brief Creates and fills the vao/vbo for rendering textures
*/
void GlWidget::create_quad_vao()
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

	m_quad_vao_.create();
	m_quad_vbo_.create();

	QOpenGLVertexArrayObject::Binder vao_binder(&m_quad_vao_);

	m_quad_vbo_.bind();
	m_quad_vbo_.allocate(quad_vertices, sizeof(quad_vertices));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<void *>(2 * sizeof(GLfloat)));
	m_quad_vao_.release();
}

/**
 * \brief Corrects the perspective matrix after resizing the window
 * \param w The new width of the Window
 * \param h The new height of the Window
 */
void GlWidget::resizeGL(const int w, const int h)
{
	m_defaultprojection_matrix_.setIdentity();
	m_defaultprojection_matrix_ = mat4::perspective(45.0f, w / float(h) / 2.0f, 0.1f, 1000.0f);

	m_default_shader_->bind();
	glUniformMatrix4fv(m_defaultproj_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_defaultprojection_matrix_));
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
	m_defaultview_matrix_.setIdentity();
	m_defaultview_matrix_.translate(0, 0, -40.0f);

	glUniformMatrix4fv(m_defaultview_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_defaultview_matrix_));

	m_defaultmodel_matrix_.setIdentity();
	glUniformMatrix4fv(m_defaultmodel_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_defaultmodel_matrix_));

	m_defaultprojection_matrix_ = mat4::perspective(45.0f, (width() / 2.0f) / float(height()), 0.1f, 1000.0f);
	glUniformMatrix4fv(m_defaultproj_loc_, 1, GL_FALSE, reinterpret_cast<GLfloat *>(&m_defaultprojection_matrix_));
}

/**
 * \brief  Sets the position of the paintbrush at mouse position if the mouse is in the uvmap window
 * \param event The mouseevent sent by QT
 */
void GlWidget::mouseMoveEvent(QMouseEvent* event)
{
	// Map mouse position to normalized device coordinates
	const auto mouse_pos_x = static_cast<float>(event->pos().x()) / width() * 2.0f - 1.0f;
	const auto mouse_pos_y = static_cast<float>(event->pos().y()) / height() * 2.0f - 1.0f;

	// were inside the drawing window
	if (mouse_pos_x >= 0.0f)
	{
		m_paintbrush_model_matrix_.setIdentity();
		m_paintbrush_model_matrix_.translate(-1.0f, 1.0f);
		m_paintbrush_model_matrix_.translate(mouse_pos_x * 2.0f, - (mouse_pos_y + 1));
		m_paintbrush_model_matrix_.scale(0.1f);
		// call paintgl
		update();
	}
}

void GlWidget::mousePressEvent(QMouseEvent* event)
{
	m_is_leftmouse_pressed_ = true;
	// Call paintgl
	update();
}

void GlWidget::mouseReleaseEvent(QMouseEvent* event)
{
	m_is_leftmouse_pressed_ = false;
	// call paintgl
	update();
}

void GlWidget::keyPressEvent(QKeyEvent* event)
{

	qDebug() <<"test";
	switch (event->key())
	{
	case Qt::Key_L:
		m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
		m_colormask_[0] = true;
		break;
	case Qt::Key_T:
		m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
		m_colormask_[1] = true;
		break;
	case Qt::Key_B:
		m_colormask_[0] = m_colormask_[1] = m_colormask_[2] = false;
		m_colormask_[2] = true;
		break;
	default:
		QOpenGLWidget::keyPressEvent(event);
		break;
	}
}
