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
    m_defaultShader = NULL;

    // Find our parent window (which is the view in the mvp design pattern)
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

	makeCurrent();
	m_testModel.cleanupModel();
    delete m_defaultShader, m_uvMapShader, m_drawBufferShader, m_hairShader, drawBuffer;
    drawBuffer= m_defaultShader = m_uvMapShader = m_hairShader = m_drawBufferShader =  0;
	doneCurrent();
}


void GLWidget::initializeGL()
{
    // Connect cleanup function for when the current context gets destroyed
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
    // QT function
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
    // Enable Scissor test for viewport splitting
    glEnable(GL_SCISSOR_TEST);
    // Setup shaders
    setupShaders();

    // setup model using the vertex data in our mvpModel
	m_testModel.setupModel(view->getPresenter()->getModel()->getReferenceModel());
    drawBuffer = new QOpenGLFramebufferObject(800,600);

}

void GLWidget::setupShaders()
{
    // Draws the growthmesh with phong lighting
    m_defaultShader = new QOpenGLShaderProgram;
    m_defaultShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vert.vert");
    m_defaultShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/frag.frag");
    m_defaultShader->bindAttributeLocation("aVertex", 0);
    m_defaultShader->bindAttributeLocation("aNormal", 1);
    m_defaultShader->bindAttributeLocation("aUV", 2);
    m_defaultShader->link();
    m_defaultShader->bind();

    m_projLoc = m_defaultShader->uniformLocation("projection");
    m_viewLoc = m_defaultShader->uniformLocation("view");
    m_modelLoc = m_defaultShader->uniformLocation("model");
    setupUniforms();

    // Draws the UV map of the loaded growthmesh
    m_uvMapShader = new QOpenGLShaderProgram;
    m_uvMapShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/uvmap.vert");
    m_uvMapShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uvmap.frag");
    m_uvMapShader->bindAttributeLocation("aVertex", 0);
    m_uvMapShader->bindAttributeLocation("aNormal", 1);
    m_uvMapShader->bindAttributeLocation("aUV", 2);
    m_uvMapShader->link();

    // Draws the framebuffer content on a quad
    m_drawBufferShader = new QOpenGLShaderProgram;
    m_drawBufferShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/drawbuffer.vert");
    m_drawBufferShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/drawbuffer.frag");
    m_drawBufferShader->bindAttributeLocation("aVertex", 0);
    m_drawBufferShader->bindAttributeLocation("aUV", 2);
    m_drawBufferShader->link();

}

// Create the vao/vbo for drawing the content of the framebuffer
void GLWidget::createDrawBufferQuad()
{
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

}

void GLWidget::paintGL()
{
    // Left half of context (Mesh)
    glViewport(0,0,width()/2,height());
    glScissor(0,0,width()/2,height());
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_defaultShader->bind();
    m_testModel.draw(m_defaultShader);
    m_defaultShader->release();

    // Right half of context (UVmap/drawing window)
    glViewport(width()/2,0,width()/2,height());
    glScissor(width()/2,0,width(),height());
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_uvMapShader->bind();
    m_testModel.draw(m_defaultShader);
    m_uvMapShader->release();
}

void GLWidget::resizeGL(int w, int h)
{
	m_projection.setIdentity();
    m_projection = mat4::perspective(45.0f, w/ float(h)/2.0f, 0.1f, 1000.0f);

    m_defaultShader->bind();
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, (GLfloat *)&m_projection);
    m_defaultShader->release();

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
    m_viewMatrix.translate(0, 0, -40.0f);

	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, (GLfloat *)&m_viewMatrix);

	m_model.setIdentity();
	glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, (GLfloat *)&m_model);

    m_projection = mat4::perspective(45.0f, (width()/2.0f) / float(height()), 0.1f, 1000.0f);
	glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, (GLfloat *)&m_projection);
}

