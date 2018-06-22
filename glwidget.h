#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <linalg.hpp>
#include <mainwindow.h>
#include <glModel.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
Q_OBJECT

public:
	explicit GlWidget(QWidget* parent = nullptr);
	virtual ~GlWidget();

public slots:
	void cleanup();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

private:
	void setup_shaders();
	void setup_mvp();
	void create_drawbuffer();
	void create_drawbuffer_quad();
	void render_drawbuffer();
	// Framebuffer for painting hair
	QOpenGLFramebufferObject* m_drawbuffer_{};

	// Contains the a quad for rendering the drawBuffer content
	QOpenGLVertexArrayObject m_drawbuffer_quad_vao_;
	QOpenGLBuffer m_drawbuffer_quad_vbo_;

	// is core profile active?
	bool m_core_;

	// default shader uniform locations
	int m_view_loc_{};
	int m_model_loc_{};
	int m_proj_loc_{};

	mat4 m_model_matrix_;
	mat4 m_view_matrix_;
	mat4 m_projection_matrix_;

	QOpenGLShaderProgram* m_default_shader_{};
	QOpenGLShaderProgram* m_uv_map_shader_{};
	QOpenGLShaderProgram* m_drawbuffer_shader_{};
	QOpenGLShaderProgram* m_hair_shader_{};

	// Test model
	GlModel m_test_model_;

	// MVP design pattern reference of view
	MainWindow* m_view_;
};

#endif
