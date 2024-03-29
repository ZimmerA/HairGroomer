#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "rendering/renderer.h"
#include "baseapp/views/mainwindow.h"

#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget
{
Q_OBJECT

public:

	Renderer m_renderer;
	Scene m_scene;

	explicit GLWidget(QWidget *parent = nullptr);
	
	void load_glmodel_data();

protected:

	void initializeGL() override;

	void paintGL() override;

	void process_input();
	void resizeGL(int w, int h) override;

	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:

	// MVP design pattern reference of view
	MainWindow *m_view_{};

	QPoint m_last_mouse_pos_;
	bool m_has_mouse_started_in_viewport_{};
	QMap<int, bool> m_keys_;
};

#endif
