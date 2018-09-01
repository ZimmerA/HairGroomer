#pragma once
#include <QOpenGLWidget>
#include "renderer.h"
#include <QOpenGLFunctions_3_3_Core>
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core

{
Q_OBJECT
public:
	GLWidget(QWidget* parent);
	~GLWidget();
	Renderer m_renderer;
	Scene m_scene;

public slots:
	void cleanup();

protected:
	void initializeGL() override;
	void paintGL() override;
};
