
#include "glwidget.h"

#include <qapplication.h>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
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
 * \brief Initialize anything opengl related
 */
void GLWidget::initializeGL()
{
	m_renderer.init(width(), height());
	m_renderer.set_measurements(width(), height());
	m_renderer.set_current_scene(&m_scene);
	m_scene.load();
	m_scene.m_projection_matrix = glm::perspective(glm::radians(45.0f),
	                                               (width() / 2.0f) / static_cast<float>(height()), 0.1f,
	                                               1000.0f);
}

void GLWidget::load_glmodel_data()
{
	makeCurrent();
	m_scene.m_fbx_glmodel.setup_model(m_view_->get_presenter()->get_model()->get_fbx_model());
	doneCurrent();
}

/**
 * \brief Corrects the perspective matrix after resizing the window
 * \param w The new width of the Window
 * \param h The new height of the Window
 */
void GLWidget::resizeGL(const int w, const int h)
{
	m_renderer.set_measurements(w, h);
	m_scene.m_projection_matrix = glm::perspective(glm::radians(45.0f), w / static_cast<float>(h) / 2.0f, 0.1f,
	                                               1000.0f);
}

/**
 * \brief Gets called by qt when the rendering needs to be updated
 */
void GLWidget::paintGL()
{
	process_input();
	m_renderer.render_scene();
}

void GLWidget::process_input()
{
	if (m_keys_[Qt::Key_R])
		m_scene.m_drawbuffer.reset();

	if (m_keys_[Qt::Key_Shift])
		m_scene.m_brush.set_opposite_mode(true);
	else
		m_scene.m_brush.set_opposite_mode(false);

	if (m_keys_[Qt::Key_Q])
		m_scene.m_camera.reset_position();

	if (m_keys_[Qt::Key_I])
		m_renderer.m_should_write_out_hair = true;
}

/**
 * \brief Gets called when a mouse button is pressed
 * \param event The mouse event
 */
void GLWidget::mousePressEvent(QMouseEvent* event)
{
	if (!event)
		return;

	// Map mouse position to normalized device coordinates
	const auto mouse_pos_x = static_cast<float>(event->pos().x()) / width() * 2.0f - 1.0f;

	switch (event->button())
	{
		case Qt::LeftButton:

			if (mouse_pos_x > 0.0f)
			{
				m_renderer.m_is_drawing = true;
			}

		case Qt::MiddleButton:
			// Camera
			m_last_mouse_pos_ = event->pos();

			// Save whether the first click was inside of the model viewport
			if (mouse_pos_x <= 0.0f)
			{
				m_has_mouse_started_in_viewport_ = true;
			}
			else
			{
				m_has_mouse_started_in_viewport_ = false;
			}

			update();
			break;
		default:
			QOpenGLWidget::mousePressEvent(event);
			break;
	}
}

/**
 * \brief Gets called when the mouse is moved
 * \param event The mouse event sent by QT
 */
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (!event)
		return;
	// Map mouse position to normalized device coordinates
	const auto mouse_pos_x = static_cast<float>(event->pos().x()) / width() * 2.0f - 1.0f;
	const auto mouse_pos_y = static_cast<float>(event->pos().y()) / height() * 2.0f - 1.0f;

	// mouse is still inside the drawing window
	if (mouse_pos_x >= 0.0f)
	{
		m_scene.m_brush.set_position(mouse_pos_x, mouse_pos_y * 0.5f + 0.5f);
		// query call of paintgl
		update();
	}

	if (m_keys_[Qt::Key_Alt])
	{
		// If the first mouse click was in the viewport, update camera as well
		if (m_has_mouse_started_in_viewport_)
		{
			const int delta_x = event->x() - m_last_mouse_pos_.x();
			const int delta_y = event->y() - m_last_mouse_pos_.y();
			if (event->buttons() & Qt::MiddleButton)
			{
				m_scene.m_camera.move_pivot_point(delta_x, delta_y);
			}
			else if (event->buttons() & Qt::LeftButton)
			{
				m_scene.m_camera.handle_mouse_move(delta_x, delta_y);
			}
			// query call of paintgl
			update();
			m_last_mouse_pos_ = event->pos();
		}
	}
}

/**
 * \brief Gets called on mouse button release
 * \param event The Mouse event
 */
void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (!event)
		return;

	switch (event->button())
	{
	case Qt::LeftButton:
		m_renderer.m_is_drawing = false;
		// query call of paintgl
		update();
		break;
	default:
		QOpenGLWidget::mousePressEvent(event);
		break;
	}
}

/**
 * \brief Gets called when the mousewheel is scrolled
 * \param event The scroll event
 */
void GLWidget::wheelEvent(QWheelEvent* event)
{
	if (!event)
		return;

	const auto num_degrees = event->angleDelta() / 8;
	const auto num_steps = num_degrees / 15;
	m_scene.m_camera.handle_mouse_wheel(num_steps.y());
	update();
}

/**
 * \brief Gets called on key press
 */
void GLWidget::keyPressEvent(QKeyEvent* event)
{
	if (!event)
		return;

	m_keys_[event->key()] = true;
	QWidget::keyPressEvent(event);
	update();
}

/**
 * \brief Gets called on key release
 */
void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (!event)
		return;

	m_keys_[event->key()] = false;
	QWidget::keyReleaseEvent(event);
	update();
}
