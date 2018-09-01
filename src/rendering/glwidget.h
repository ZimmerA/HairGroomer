#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "Paintbrush.h"
#include "./rendering/orbitcamera.h"

#include <linalg.hpp>
#include <mainwindow.h>
#include "glModel.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFramebufferObject>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

/**
 * \brief stores general light information
 */
struct Light
{
	vec3 m_position = vec3(0.f,550.0f,0.f);
	vec3 m_color;
};

/**
 * \brief Stores general hair information
 */
struct Hair
{
	vec3 m_root_color;
	vec3 m_hair_color;
	float m_length{};
	int m_num_segments{};
};

/**
 * \brief The widget that renders the OpenGL context
 */
class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
Q_OBJECT

public:
	explicit GlWidget(QWidget* parent = nullptr);
	virtual ~GlWidget();

	// Getters/Setters
	void set_uv_overlay_visible(bool visible);
	void set_should_render_growthmesh(bool visible);
	void set_should_render_referencemodel(bool visible);
	void set_hair_length(float length);
	void set_hair_num_segments(int segment_count);
	void set_hair_color(int r, int g, int b);
	void set_hair_root_color(int r, int g, int b);
	void set_should_light_hair(bool enabled);
	void set_should_light_mesh(bool enabled);
	void set_light_color(int r, int g, int b);
	void set_light_position(float x, float y, float z);
	void set_brush_size(float size);
	void set_brush_intensity(float intensity);
	void set_brush_mode(Paintbrush::paintmode mode);
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

	// Shader programs
	QOpenGLShaderProgram* m_default_shader_{};
	QOpenGLShaderProgram* m_uv_map_shader_{};
	QOpenGLShaderProgram* m_drawbuffer_shader_{};
	QOpenGLShaderProgram* m_hair_shader_{};
	QOpenGLShaderProgram* m_paintbrush_shader_{};

	// Textures
	QOpenGLTexture * m_paint_brush_texture_{};
	
	// Quad for rendering textures
	QOpenGLVertexArrayObject m_quad_vao_;
	QOpenGLBuffer m_quad_vbo_;

	// Framebuffer for painting hair
	QOpenGLFramebufferObject* m_drawbuffer_{};
	void reset_drawbuffer();

	/* Input related*/
	QPoint m_last_mouse_pos_;
	bool m_has_mouse_started_in_viewport_{};	// tells us if the mouse is in viewport
	bool m_is_leftmouse_pressed_ = false;
	QMap<int, bool> m_keys_;
	void process_input();

	/* Scene Elements */
	Orbitcamera m_camera_;
	Light m_light_;
	Hair m_hair_;
	Paintbrush m_brush_;
	GlModel m_growth_mesh_;
	GlModel m_reference_model_;

	bool m_should_light_hair_{};
	bool m_should_light_mesh_{};
	bool m_should_draw_uv_map_{};
	bool m_should_render_refrencemodel_{};
	bool m_should_render_growthmesh_{};

	// mvp matrices used by hair and meshes
	mat4 m_defaultmodel_matrix_;
	mat4 m_defaultview_matrix_;
	mat4 m_defaultprojection_matrix_;

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
	void keyReleaseEvent(QKeyEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
};
#endif
