#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <linalg.hpp>
#include <modeldata.h>
#include <mainwindow.h>
#include <vector>
#include <glModel.h>
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    static bool isTransparent() { return m_transparent; }
    static void setTransparent(bool t) { m_transparent = t; }

public slots:
    void cleanup();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:

    void setupUniforms();
    static bool m_transparent;
    bool m_core;

	int m_viewLoc;
	int m_modelLoc;
	int m_projLoc;

	mat4 m_model, m_viewMatrix , m_projection;
	QOpenGLShaderProgram *m_program;

	GlModel m_testModel;
	MainWindow * view;
};

#endif
