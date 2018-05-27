#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <mvppresenter.h>
#include <qapplication.h>

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
	m_program = NULL;

	// Find our parent window (which is the view in the mvp pattern)
	foreach(QWidget *widget, QApplication::topLevelWidgets()) {
		if (MainWindow* v = dynamic_cast<MainWindow*>(widget))
			view = v;
	}

}

GLWidget::~GLWidget()
{
	cleanup();
}


void GLWidget::cleanup()
{
	if (m_program == NULL)
		return;

	makeCurrent();
	m_testModel.cleanupModel();
	delete m_program;
	m_program = 0;

	doneCurrent();
}


void GLWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

	initializeOpenGLFunctions();

	glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	m_program = new QOpenGLShaderProgram;
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vert.vert");
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/frag.frag");
	m_program->bindAttributeLocation("aVertex", 0);
	m_program->bindAttributeLocation("aNormal", 1);
	m_program->bindAttributeLocation("aUv", 2);
	m_program->link();
	m_program->bind();
	m_projLoc = m_program->uniformLocation("projection");
	m_viewLoc = m_program->uniformLocation("view");
	m_modelLoc = m_program->uniformLocation("model");
	setupUniforms();

	m_testModel.setupModel(view->getPresenter()->getModel()->getReferenceModel());
}

void GLWidget::paintGL()
{
    glViewport(0,0,width()/2,height());
    glScissor(0,0,width()/2,height());
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind();
	m_testModel.draw(m_program);
	m_program->release();

    glScissor(width()/2,0,width(),height());
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(width()/2,0,width()/2,height());
    m_program->bind();
    m_testModel.draw(m_program);
    m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
	m_projection.setIdentity();
    m_projection = mat4::perspective(45.0f, w/ float(h)/2.0f, 0.1f, 1000.0f);

    m_program->bind();
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, (GLfloat *)&m_projection);
    m_program->release();

}

/*
 *  Setup standard uniforms
 *  Note:
 *  we have to use glUniformMatrix4fv here (instead of the QT wrapper function)
 *  because the QT shader wrapper doesnt have a uniform function that takes a float * as parameter
 *  and hands it to glUniformMatrix(n)fv which is needed for our math library (linalg)
 */
void GLWidget::setupUniforms()
{
	m_viewMatrix.setIdentity();
    m_viewMatrix.translate(10, -80, -190.0f);

	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, (GLfloat *)&m_viewMatrix);

	m_model.setIdentity();
	glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, (GLfloat *)&m_model);

    m_projection = mat4::perspective(45.0f, (width()/2.0f) / float(height()), 0.1f, 1000.0f);
	glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, (GLfloat *)&m_projection);


    glScissor(0,0,width()/2,float(height()));
}

