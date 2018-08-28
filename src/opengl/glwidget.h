#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <linalg.hpp>
#include <mainwindow.h>
#include "glModel.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include "Paintbrush.h"
#include "./opengl/orbitcamera.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

/**
 * \brief The widget that renders the opengl context
 */
class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
Q_OBJECT

public:
	explicit GlWidget(QWidget* parent = nullptr);
	virtual ~GlWidget();
	
	Paintbrush m_brush;
	void set_uv_overlay_visible(bool visible);
	void set_hair_length_uniform(float length);
	void set_hair_num_segments_uniform(int segment_count);
	void set_hair_color_unfirom(int r, int g, int b);
	void set_hair_root_color_uniform(int r, int g, int b);
	void set_light_hair_uniform(bool enabled);
	void set_light_color_uniform(int r, int g, int b);
	void grab_drawbuffer_content_to_image(QImage& image);
	void set_drawbuffer_content(QImage& image);

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
	void render_paintbrush(bool painting_to_framebuffer);
	void render_uv_map();
	void paint_to_frame_buffer();

	bool m_should_draw_uv_map_{};

	// is core profile active?
	bool m_core_;

	// Shader programs
	QOpenGLShaderProgram* m_default_shader_{};
	QOpenGLShaderProgram* m_uv_map_shader_{};
	QOpenGLShaderProgram* m_drawbuffer_shader_{};
	QOpenGLShaderProgram* m_hair_shader_{};
	QOpenGLShaderProgram* m_paintbrush_shader_{};

	// mvp matrices used by hair and meshes
	mat4 m_defaultmodel_matrix_;
	mat4 m_defaultview_matrix_;
	mat4 m_defaultprojection_matrix_;

	// Textures
	QOpenGLTexture * m_paint_brush_texture_{};
	
	// Quad vao for rendering textures
	QOpenGLVertexArrayObject m_quad_vao_;
	QOpenGLBuffer m_quad_vbo_;

	// Framebuffer for painting hair
	QOpenGLFramebufferObject* m_drawbuffer_{};
	bool m_is_leftmouse_pressed_ = false;
	void reset_drawbuffer();

	//Viewport camera
	QPoint m_last_mouse_pos_; // Last mouse position
	bool m_has_mouse_started_in_viewport_{};	// tells us if the mouse is in viewport
	Orbitcamera m_camera_;
	
	// MVP design pattern reference of view
	MainWindow* m_view_;

	// Growth mesh for hair grooming
	GlModel m_growth_mesh_;
	GlModel m_reference_model_;

	QMap<int, bool> m_keys_;
	void process_input();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
};
#endif
