#include "renderer.h"

#include <linalg.hpp>
#include <QDebug>

void Renderer::init(const int width, const int height)
{
	initializeOpenGLFunctions();
	// Enable Scissor test for viewport splitting
	glEnable(GL_SCISSOR_TEST);

	m_width_ = width;
	m_height_ = height;
}

void Renderer::render_scene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Render left half of the context (viewport)
	glViewport(0, 0, m_width_ / 2, m_height_);
	glScissor(0, 0, m_width_ / 2, m_height_);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 view_matrix = m_current_scene_->m_camera.get_view_matrix();
	mat4 mvp = m_current_scene_->m_defaultprojection_matrix * view_matrix * m_current_scene_->m_defaultmodel_matrix;
	mat4 vp = m_current_scene_->m_defaultprojection_matrix * view_matrix;

	m_current_scene_->m_default_shader->bind();
	m_current_scene_->m_default_shader->setUniformValue("lightPos", m_current_scene_->m_light.m_position.x,
	                                                    m_current_scene_->m_light.m_position.y,
	                                                    m_current_scene_->m_light.m_position.z);
	m_current_scene_->m_default_shader->setUniformValue("lightColor", m_current_scene_->m_light.m_color.x,
	                                                    m_current_scene_->m_light.m_color.y,
	                                                    m_current_scene_->m_light.m_color.z);
	m_current_scene_->m_default_shader->setUniformValue("lighting", m_should_light_meshes);
	glUniformMatrix4fv(m_current_scene_->m_default_shader->uniformLocation("model"), 1,GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_current_scene_->m_defaultmodel_matrix));
	glUniformMatrix4fv(m_current_scene_->m_default_shader->uniformLocation("mvp"), 1,GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&mvp));

	if (m_should_render_growthmesh)
	{
		// draw the growth mesh
		m_current_scene_->m_growth_mesh.draw();
	}

	if (m_should_render_refrencemodel)
	{
		// draw the reference mesh
		m_current_scene_->m_reference_model.draw();
	}

	m_current_scene_->m_default_shader->release();

	// draw the hair
	m_current_scene_->m_hair_shader->bind();
	m_current_scene_->m_hair_shader->setUniformValue("cameraPos", m_current_scene_->m_camera.m_position.x,
	                                                 m_current_scene_->m_camera.m_position.y,
	                                                 m_current_scene_->m_camera.m_position.z);
	m_current_scene_->m_hair_shader->setUniformValue("lightPos", m_current_scene_->m_light.m_position.x,
	                                                 m_current_scene_->m_light.m_position.y,
	                                                 m_current_scene_->m_light.m_position.z);
	m_current_scene_->m_hair_shader->setUniformValue("lightColor", m_current_scene_->m_light.m_color.x,
	                                                 m_current_scene_->m_light.m_color.y,
	                                                 m_current_scene_->m_light.m_color.z);
	m_current_scene_->m_hair_shader->setUniformValue("lighting", m_should_light_hair);
	m_current_scene_->m_hair_shader->setUniformValue("numSegments", m_current_scene_->m_hair.m_num_segments);
	m_current_scene_->m_hair_shader->setUniformValue("maxHairLength", m_current_scene_->m_hair.m_length);
	m_current_scene_->m_hair_shader->setUniformValue("hairColor", m_current_scene_->m_hair.m_hair_color.x,
	                                                 m_current_scene_->m_hair.m_hair_color.y,
	                                                 m_current_scene_->m_hair.m_hair_color.z);
	m_current_scene_->m_hair_shader->setUniformValue("rootColor", m_current_scene_->m_hair.m_root_color.x,
	                                                 m_current_scene_->m_hair.m_root_color.y,
	                                                 m_current_scene_->m_hair.m_root_color.z);
	glUniformMatrix4fv(m_current_scene_->m_hair_shader->uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&m_current_scene_->m_defaultmodel_matrix));
	glUniformMatrix4fv(m_current_scene_->m_hair_shader->uniformLocation("view"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&view_matrix));
	glUniformMatrix4fv(m_current_scene_->m_hair_shader->uniformLocation("mvp"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&mvp));
	glUniformMatrix4fv(m_current_scene_->m_hair_shader->uniformLocation("vp"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&vp));

	// Bind the draw framebuffer texture to texunit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_current_scene_->m_drawbuffer.get_texture_handle());


	// Transform feedback
	GLuint geom_shader_query;
	if (m_should_write_out_hair)
	{
		// Generate query to get number of generated primitves
		glGenQueries(1, &geom_shader_query);
		// 1 hair per drawn vertex of the mesh, number of segments + 1 points per hair ( 1 segment = 2 points, 2 segments = 3 points etc.), 3 dimensions per hair(x, y z)
		m_current_scene_->resize_hair_feedback_buffer(m_current_scene_->m_growth_mesh.get_vertice_amount() * (m_current_scene_->m_hair.m_num_segments *2) * 3 * sizeof(GLfloat));
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_current_scene_->m_hair_output_vbo.bufferId());
		glBeginQuery(GL_PRIMITIVES_GENERATED, geom_shader_query);
		glBeginTransformFeedback(GL_LINES);
	}

	// Draw the hair using the growthmesh + the drawbuffer texture
	m_current_scene_->m_growth_mesh.draw_points();

	// Transform feedback
	if (m_should_write_out_hair)
	{
		glEndTransformFeedback();
		glEndQuery(GL_PRIMITIVES_GENERATED);
		GLuint primitives;
		glGetQueryObjectuiv(geom_shader_query, GL_QUERY_RESULT, &primitives);

		m_hairdata_cache.m_num_hair = primitives / m_current_scene_->m_hair.m_num_segments;
		m_hairdata_cache.m_num_segments = m_current_scene_->m_hair.m_num_segments;
		//qDebug() << "Primitives generated: " << primitives;
		// Read data from the buffer
		// 2 points per line segment, 3 points per position
		auto* feedback = new GLfloat[primitives * 2 * 3];
		int size =  primitives * 2 * 3 * sizeof(GLfloat);
		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, size, feedback);
		m_hairdata_cache.m_vertices.clear();
		unsigned int i = 0;
		int counter = 0;
		
		while (i < primitives * 2 * 3)
		{
			vec3 position( feedback[i], feedback[i+1], feedback[i+2]);
			m_hairdata_cache.m_vertices.push_back(position);
			//qDebug() << "value: " << i << ": " << feedback[i] << " " << feedback[i+1] << " " << feedback[i+2];
			if (counter < m_current_scene_->m_hair.m_num_segments-1)
			{
				i += 2 * 3;
				counter++;
			}
			else
			{
				i += 1 * 3;
				counter++;
				if (counter == m_current_scene_->m_hair.m_num_segments+1)
				{
					counter = 0;
				}
			}
		}
		
		delete [] feedback;
		m_should_write_out_hair = false;
	}
	m_current_scene_->m_hair_shader->release();

	// Render right half of the context (drawing window)
	glViewport(m_width_ / 2, 0, m_width_ / 2, m_height_);
	glScissor(m_width_ / 2, 0, m_width_, m_height_);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// Disable depth testing cause of alpha blending (were only drawing textures)
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// Render drawbuffer texture content to screen
	m_current_scene_->m_drawbuffer_shader->bind();
	m_current_scene_->m_quad_vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_current_scene_->m_drawbuffer_shader->release();

	// Render UV Map in wireframe mode
	if (m_should_render_uv_overlay)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_current_scene_->m_uv_map_shader->bind();
		m_current_scene_->m_growth_mesh.draw();
		m_current_scene_->m_uv_map_shader->release();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Render the paintbrush to default framebuffer
	m_current_scene_->m_paint_brush_texture->bind();
	m_current_scene_->m_paintbrush_shader->bind();
	//m_current_scene_->m_brush.begin(m_current_scene_->m_paintbrush_shader, false);
	glUniformMatrix4fv(m_current_scene_->m_paintbrush_shader->uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<const GLfloat *>(&m_current_scene_->m_brush.get_transform()));
	glUniform1f(m_current_scene_->m_paintbrush_shader->uniformLocation("intensity"),
	            m_current_scene_->m_brush.get_intensity());
	m_current_scene_->m_quad_vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Render the paintbrush to the drawbuffer if left mouse is pressed
	if (m_is_drawing)
	{
		m_current_scene_->m_drawbuffer.bind();
		glViewport(0, 0, 800, 600);
		glScissor(0, 0, 800, 600);
		//m_current_scene_->m_brush.begin(m_current_scene_->m_paintbrush_shader, true);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glColorMask(m_current_scene_->m_brush.get_colormask()[0], m_current_scene_->m_brush.get_colormask()[1],
		            m_current_scene_->m_brush.get_colormask()[2], GL_TRUE);
		if (m_current_scene_->m_brush.get_opposite_mode())
		{
			glUniform1f(m_current_scene_->m_paintbrush_shader->uniformLocation("intensity"),
			            1.0f - m_current_scene_->m_brush.get_intensity());
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glColorMask(GL_TRUE,GL_TRUE, GL_TRUE, GL_TRUE);
		m_current_scene_->m_drawbuffer.release();
	}

	m_current_scene_->m_paintbrush_shader->release();

	// Reset opengl to default state
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
