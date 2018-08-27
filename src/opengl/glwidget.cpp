#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include "./mvppresenter.h"
#include <qapplication.h>

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
 * \brief Clean up the buffers and shader programs
 */
void GlWidget::cleanup()
{
	if (m_default_shader_ == nullptr)
		return;

	makeCurrent();
	m_growth_mesh_.cleanup_model();
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

	// Compile and link shaders, set relevant uniforms
	setup_shaders();
	// Create the framebuffer for hair drawing
	create_drawbuffer();
	// load all relevant textures
	load_textures();

	// Create a quad for rendering textures
	create_quad_vao();

	// setup model using the vertex data in our mvpModel
	m_growth_mesh_.setup_model(m_view_->get_presenter()->get_model()->get_reference_model());
}

/**
 * \brief Compiles and links the shader programs, sets up uniform and attribute locations 
 */
void GlWidget::setup_shaders()
{
	// Draws the growth-mesh
	m_default_shader_ = new QOpenGLShaderProgram;
	m_default_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/opengl/shaders/viewport/mesh.vert");
	m_default_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/opengl/shaders/viewport/mesh.frag");
	m_default_shader_->bindAttributeLocation("aVertex", 0);
	m_default_shader_->bindAttributeLocation("aNormal", 1);
	m_default_shader_->bindAttributeLocation("aUV", 2);
	m_default_shader_->link();
	m_default_shader_->bind();
	setup_mvp();

	// Draws the UV map of the loaded growth-mesh
	m_uv_map_shader_ = new QOpenGLShaderProgram;
	m_uv_map_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/opengl/shaders/paintwindow/uvmap.vert");
	m_uv_map_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/opengl/shaders//paintwindow/uvmap.frag");
	m_uv_map_shader_->bindAttributeLocation("aVertex", 0);
	m_uv_map_shader_->bindAttributeLocation("aNormal", 1);
	m_uv_map_shader_->bindAttributeLocation("aUV", 2);
	m_uv_map_shader_->link();

	// used for drawing the framebuffer
	m_drawbuffer_shader_ = new QOpenGLShaderProgram;
	m_drawbuffer_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex,
	                                              "./src/opengl/shaders/paintwindow/drawbuffer.vert");
	m_drawbuffer_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment,
	                                              "./src/opengl/shaders/paintwindow/drawbuffer.frag");
	m_drawbuffer_shader_->bindAttributeLocation("aVertex", 0);
	m_drawbuffer_shader_->bindAttributeLocation("aUV", 1);
	m_drawbuffer_shader_->link();
	// look for the framebuffer texture in texture location 0
	m_drawbuffer_shader_->setUniformValue("screenTexture", 0);

	// used for drawing the paintbrush
	m_paintbrush_shader_ = new QOpenGLShaderProgram;
	m_paintbrush_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex,
	                                              "./src/opengl/shaders/paintwindow/paintbrush.vert");
	m_paintbrush_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment,
	                                              "./src/opengl/shaders/paintwindow/paintbrush.frag");
	m_paintbrush_shader_->bindAttributeLocation("aVertex", 0);
	m_paintbrush_shader_->bindAttributeLocation("aUV", 1);
	m_paintbrush_shader_->link();
	// look for the framebuffer texture in texture location 0
	m_paintbrush_shader_->setUniformValue("screenTexture", 0);

	// used for drawing the hair
	m_hair_shader_ = new QOpenGLShaderProgram;
	m_hair_shader_->addShaderFromSourceFile(QOpenGLShader::Vertex, "./src/opengl/shaders/hair/hair.vert");
	m_hair_shader_->addShaderFromSourceFile(QOpenGLShader::Geometry, "./src/opengl/shaders/hair/hair.geom");
	m_hair_shader_->addShaderFromSourceFile(QOpenGLShader::Fragment, "./src/opengl/shaders/hair/hair.frag");
	m_hair_shader_->bindAttributeLocation("aVertex", 0);
	m_hair_shader_->bindAttributeLocation("aNormal", 1);
	m_hair_shader_->bindAttributeLocation("aUV", 2);
	m_hair_shader_->link();
	m_hair_shader_->bind();
	// look for the framebuffer texture in texture location 0
	m_hair_shader_->setUniformValue("hairMap", 0);
}

/**
 * \brief Sets up the model view projection uniforms of the standard shader
 * \note we have to use glUniformMatrix4fv here (instead of the QT wrapper function)
 *		 because the QT shader wrapper doesn't have a uniform function that takes a float * as parameter
 *		 and hands it to glUniformMatrix(n)fv which is needed for our math library (linalg)
 */
void GlWidget::setup_mvp()
{
	m_default_shader_->bind();
	m_defaultview_matrix_.setIdentity();
	glUniformMatrix4fv(m_default_shader_->uniformLocation("view"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultview_matrix_));

	m_defaultmodel_matrix_.setIdentity();
	glUniformMatrix4fv(m_default_shader_->uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultmodel_matrix_));

	m_defaultprojection_matrix_ = mat4::perspective(45.0f, (width() / 2.0f) / float(height()), 0.1f, 1000.0f);
	glUniformMatrix4fv(m_default_shader_->uniformLocation("projection"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultprojection_matrix_));
}

/**
 * \brief Creates the framebuffer to draw to with the paintbrush
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
 * \brief Load all relevant textures
 */
void GlWidget::load_textures()
{
	m_paint_brush_texture_ = new QOpenGLTexture(QImage("./res/brush.png").mirrored());
	m_paint_brush_texture_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	m_paint_brush_texture_->setMagnificationFilter(QOpenGLTexture::Linear);
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
 * \brief Renders the opengl context
 */
void GlWidget::paintGL()
{
	process_input();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Left half of context (Mesh)
	render_left_half();
	// Right half of context (UV-map/drawing window)
	render_right_half();
}

/**
 * \brief Renders the left half of the viewport
 */
void GlWidget::render_left_half()
{
	// Split the screen
	glViewport(0, 0, width() / 2, height());
	glScissor(0, 0, width() / 2, height());
	// we don't want the mesh to be a wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_defaultview_matrix_ = m_camera_.get_view_matrix();
	mat4 mvp = m_defaultprojection_matrix_ * m_defaultview_matrix_ * m_defaultmodel_matrix_;
	mat4 vp = m_defaultprojection_matrix_ * m_defaultview_matrix_;

	// draw the growth mesh
	m_default_shader_->bind();
	glUniformMatrix4fv(m_default_shader_->uniformLocation("model"), 1,GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultmodel_matrix_));
	glUniformMatrix4fv(m_default_shader_->uniformLocation("mvp"), 1,GL_FALSE, reinterpret_cast<GLfloat *>(&mvp));
	m_growth_mesh_.draw(m_default_shader_);
	m_default_shader_->release();

	// draw the hair
	m_hair_shader_->bind();
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultmodel_matrix_));
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("view"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultview_matrix_));
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("mvp"), 1, GL_FALSE, reinterpret_cast<GLfloat *>(&mvp));
	glUniformMatrix4fv(m_hair_shader_->uniformLocation("vp"), 1, GL_FALSE, reinterpret_cast<GLfloat *>(&vp));
	// Bind framebuffer texture to texunit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	m_growth_mesh_.draw(m_hair_shader_);
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

	// render brush to framebuffer
	if (m_is_leftmouse_pressed_ && !m_has_mouse_started_in_viewport_)
		paint_to_frame_buffer();

	// Split the Viewport
	glViewport(width() / 2, 0, width() / 2, height());
	glScissor(width() / 2, 0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	// Draw the Texture of the framebuffer to a quad
	render_drawbuffer();

	// Draw the UV map of the growth mesh
	if (m_should_draw_uv_map_)
	{
		render_uv_map();
	}

	// Draw the paintbrush at the mouse position
	render_paintbrush(false);

	// Disable alpha blending for the next frame
	glDisable(GL_BLEND);
	// Enable depth testing for the next frame
	glEnable(GL_DEPTH_TEST);
}

/**
 * \brief Draws the paintbrush at its current position into the framebuffer
 */
void GlWidget::paint_to_frame_buffer()
{
	// Bind the framebuffer
	m_drawbuffer_->bind();
	glViewport(0, 0, 800, 600);
	glScissor(0, 0, 800, 600);
	// Draw the paintbrush into the framebuffer at the mouse position
	render_paintbrush(true);
	m_drawbuffer_->release();
}

/**
 * \brief Uses the uv-map shader to draw the uv-map of the growth-mesh in wireframe mode
 */
void GlWidget::render_uv_map()
{
	// Draw the uv map (wireframe)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_uv_map_shader_->bind();
	m_growth_mesh_.draw(m_default_shader_);
	m_uv_map_shader_->release();
	// Disable wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
* \brief Renders the paint framebuffer texture to a quad
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
 * \brief Renders the paintbrush
 * \param painting_to_framebuffer used to determine whether the intensity uniform should be 1 or not and if the color mask should be applied
 */
void GlWidget::render_paintbrush(const bool painting_to_framebuffer)
{
	// bind paintbrush texture to texunit 0
	glActiveTexture(GL_TEXTURE0);
	m_paint_brush_texture_->bind();

	// render the brush
	m_paintbrush_shader_->bind();
	m_brush.begin(m_paintbrush_shader_, painting_to_framebuffer);
	QOpenGLVertexArrayObject::Binder vao_binder(&m_quad_vao_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_brush.end();
	m_paintbrush_shader_->release();
}

/**
 * \brief Corrects the perspective matrix after resizing the window
 * \param w The new width of the Window
 * \param h The new height of the Window
 */
void GlWidget::resizeGL(const int w, const int h)
{
	m_defaultprojection_matrix_ = mat4::perspective(45.0f, w / float(h) / 2.0f, 0.1f, 1000.0f);

	m_default_shader_->bind();
	glUniformMatrix4fv(m_default_shader_->uniformLocation("projection"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_defaultprojection_matrix_));
	m_default_shader_->release();
}

void GlWidget::mousePressEvent(QMouseEvent* event)
{
	// Map mouse position to normalized device coordinates
	const auto mouse_pos_x = static_cast<float>(event->pos().x()) / width() * 2.0f - 1.0f;
	switch (event->button())
	{
	case Qt::LeftButton:
		m_is_leftmouse_pressed_ = true;
		// Camera
		m_last_mouse_pos_ = event->pos();
		// Save whether the first click was inside of the model viewport
		if (mouse_pos_x <= 0.0f)
			m_has_mouse_started_in_viewport_ = true;
		else
			m_has_mouse_started_in_viewport_ = false;

		// query call of paintgl
		update();
		break;
	default:
		QOpenGLWidget::mousePressEvent(event);
		break;
	}
}

void GlWidget::grab_drawbuffer_content_to_image(QImage& image)
{
	makeCurrent();
	GLint width, height;
	glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	auto resized = image.scaled(width, height).convertToFormat(QImage::Format_RGBA8888);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, resized.bits());
	image.swap(resized);
}

void GlWidget::set_drawbuffer_content(QImage& image)
{
	makeCurrent();
	glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGBA,GL_UNSIGNED_BYTE, image.bits());
}

/**
 * \brief Sets the position of the paintbrush at mouse position if the mouse is in the uv-map window
 * \param event The mouse event sent by QT
 */
void GlWidget::mouseMoveEvent(QMouseEvent* event)
{
	// Map mouse position to normalized device coordinates
	const auto mouse_pos_x = static_cast<float>(event->pos().x()) / width() * 2.0f - 1.0f;
	const auto mouse_pos_y = static_cast<float>(event->pos().y()) / height() * 2.0f - 1.0f;

	// mouse is still inside the drawing window
	if (mouse_pos_x >= 0.0f)
	{
		m_brush.set_position(mouse_pos_x * 2.0f, - (mouse_pos_y + 1));
		// query call of paintgl
		update();
	}

	// If the first mouse click was in the viewport, update camera as well
	if (m_has_mouse_started_in_viewport_)
		if (event->buttons() & Qt::LeftButton)
		{
			const int delta_x = event->x() - m_last_mouse_pos_.x();
			const int delta_y = event->y() - m_last_mouse_pos_.y();

			if (!m_keys_[Qt::Key_Alt])
			{
				m_camera_.handle_mouse_move(delta_x, delta_y);



			}
			else
			{
				m_camera_.move_pivot_point(delta_x, delta_y);
			}
							update();
							m_last_mouse_pos_ = event->pos();
		}
}

void GlWidget::mouseReleaseEvent(QMouseEvent* event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		m_is_leftmouse_pressed_ = false;
		// query call of paintgl
		update();
		break;
	default:
		QOpenGLWidget::mousePressEvent(event);
		break;
	}
}


/**
 * \brief The keyboard inside of the gl widget
 */
void GlWidget::keyPressEvent(QKeyEvent* event)
{
	m_keys_[event->key()] = true;
	QWidget::keyPressEvent(event);
	update();
}

/**
 * \brief The keyboard inside of the gl widget
 */
void GlWidget::keyReleaseEvent(QKeyEvent* event)
{
	m_keys_[event->key()] = false;
	QWidget::keyReleaseEvent(event);
	update();
}

void GlWidget::wheelEvent(QWheelEvent* event)
{
	QPoint numDegrees = event->angleDelta() / 8;
	QPoint numSteps = numDegrees / 15;
	m_camera_.handle_mouse_wheel(numSteps.y());

	update();
}


void GlWidget::process_input()
{
	if (m_keys_[Qt::Key_R])
		reset_drawbuffer();

	if (m_keys_[Qt::Key_Shift])
		m_brush.set_erase_mode(true);
	else
		m_brush.set_erase_mode(false);
}

/**
 * \brief Clears the content of the drawbuffer
 */
void GlWidget::reset_drawbuffer()
{
	makeCurrent();
	m_drawbuffer_->bind();
	glViewport(0, 0, 800, 600);
	glScissor(0, 0, 800, 600);
	glClearColor(0.f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_drawbuffer_->release();
	update();
}

void GlWidget::set_uv_overlay_visible(const bool visible)
{
	m_should_draw_uv_map_ = visible;
}

void GlWidget::set_hair_length_uniform(const float length)
{
	makeCurrent();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("maxHairLength", length);
}

void GlWidget::set_hair_num_segments_uniform(const int segment_count)
{
	makeCurrent();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("numSegments", segment_count);
}

void GlWidget::set_hair_color_unfirom(const int r, const int g, const int b)
{
	makeCurrent();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("hairColor", r / 255.0f, g / 255.0f, b / 255.0f);
}

void GlWidget::set_hair_root_color_uniform(const int r, const int g, const int b)
{
	makeCurrent();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("rootColor", r / 255.0f, g / 255.0f, b / 255.0f);
}

void GlWidget::set_light_hair_uniform(const bool enabled)
{
	makeCurrent();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("lighting", enabled);
}

void GlWidget::set_light_color_uniform(const int r, const int g, const int b)
{
	makeCurrent();
	m_hair_shader_->bind();
	m_hair_shader_->setUniformValue("lightColor", r / 255.0f, g / 255.0f, b / 255.0f);
	m_default_shader_->bind();
	m_default_shader_->setUniformValue("lightColor", r / 255.0f, g / 255.0f, b / 255.0f);;
}
