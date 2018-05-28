#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <linalg.hpp>
#include <mainwindow.h>
#include <modeldata.h>
#include <glModel.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLFramebufferObject>

#include <vector>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

public slots:
    void cleanup();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    void setupShaders();
    void setupUniforms();
    void createDrawBufferQuad();
    void renderDrawBuffer();
    // Framebuffer for painting hair
    QOpenGLFramebufferObject *m_drawBuffer;
    // Contains the a quad for rendering the drawBuffer content
    QOpenGLVertexArrayObject m_drawBufferQuadVao;
    QOpenGLBuffer m_drawBufferQuadVbo;

    // is core profile active?
    bool m_core;

    // default shader uniform locations
	int m_viewLoc;
	int m_modelLoc;
	int m_projLoc;

    // Matrices
	mat4 m_model, m_viewMatrix , m_projection;

    // Shader Programs
    QOpenGLShaderProgram *m_defaultShader, *m_uvMapShader,*m_drawBufferShader, *m_hairShader;

    // Test model
	GlModel m_testModel;

    // MVP design pattern reference of view
	MainWindow * view;
};

#endif
