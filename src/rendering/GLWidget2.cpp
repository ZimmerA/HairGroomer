#include "GLWidget2.h"


GLWidget::GLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
		setMouseTracking(true);
	setFocus();
}


GLWidget::~GLWidget()
{
	cleanup();
}

void GLWidget::cleanup()
{
}

void GLWidget::initializeGL()
{
	// Connect cleanup function for when the current context gets destroyed
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
	// QT function for initializing opengl
	initializeOpenGLFunctions();
}

void GLWidget::paintGL()
{

	m_renderer.render(&m_scene);
}
