#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <linalg.hpp>
#include <mainwindow.h>
#include <modeldata.h>
#include <glModel.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

#include <vector>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
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

    // Framebuffer for painting hair
    QOpenGLFramebufferObject *drawBuffer;

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
