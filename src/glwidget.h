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
#include <QOpenGLTexture>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
Q_OBJECT

public:
	explicit GlWidget(QWidget* parent = nullptr);
	virtual ~GlWidget();

public slots:
	void cleanup();

private:
	// setup methods
	void setup_shaders();
	void setup_mvp();
	void create_drawbuffer();
	void load_textures();
	void create_quad_vao();

	// Render methods
	void render_left_half();
	void render_right_half();
	void render_drawbuffer();
	void render_paintbrush();
	void render_uv_map();
	void paint_to_frame_buffer();

	// is core profile active?
	bool m_core_;

	// Framebuffer for painting hair
	QOpenGLFramebufferObject* m_drawbuffer_{};
	bool m_is_leftmouse_pressed_ = false;

	// Contains the a quad for rendering the drawBuffer content
	QOpenGLVertexArrayObject m_quad_vao_;
	QOpenGLBuffer m_quad_vbo_;

	// default shader uniform locations
	int m_defaultview_loc_{};
	int m_defaultmodel_loc_{};
	int m_defaultproj_loc_{};
	mat4 m_defaultmodel_matrix_;
	mat4 m_defaultview_matrix_;
	mat4 m_defaultprojection_matrix_;

	// Shader programs
	QOpenGLShaderProgram* m_default_shader_{};
	QOpenGLShaderProgram* m_uv_map_shader_{};
	QOpenGLShaderProgram* m_drawbuffer_shader_{};
	QOpenGLShaderProgram* m_hair_shader_{};
	QOpenGLShaderProgram* m_paintbrush_shader_{};
	// Textures
	QOpenGLTexture * m_paint_brush_texture_{};

	// Paintbrush
	mat4 m_paintbrush_model_matrix_;
	int m_paintbrushmodel_loc_{};
	bool m_colormask_[3];
	// Test model
	GlModel m_test_model_;

	// MVP design pattern reference of view
	MainWindow* m_view_;

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
};

#endif
