#include "renderer.h"


void Renderer::render(Scene* scene, GLWidget* widget)
{
	QOpenGLFunctions_3_3_Core * gl = widget->context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	gl->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	// Render to left half of the screen
	glViewport(0, 0, m_width_ / 2, m_height_);
	glScissor(0, 0,m_width_  / 2, m_height_);

	// We don't want the mesh to be a wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup the relevant matrices
	const mat4 view_matrix = scene->m_camera.get_view_matrix();
	mat4 model_matrix = scene->m_model_matrix;
	const mat4 projection_matrix = scene->m_projection_matrix;

	mat4 mvp = projection_matrix * view_matrix * model_matrix;
	mat4 vp = projection_matrix * view_matrix;

	// Draw the meshes
	scene->m_mesh_shader.bind();

	// Setup uniforms
	glUniform4fv(scene->m_mesh_shader.uniformLocation("lightPos"),1, reinterpret_cast<GLfloat *>(&scene->m_light.m_position));
	glUniform4fv(scene->m_mesh_shader.uniformLocation("lightColor"),1, reinterpret_cast<GLfloat *>(&scene->m_light.m_color));
	glUniform1i(scene->m_mesh_shader.uniformLocation("lighting"),scene->m_should_light_mesh);
	glUniformMatrix4fv(scene->m_mesh_shader.uniformLocation("model"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&model_matrix));
	glUniformMatrix4fv(scene->m_mesh_shader.uniformLocation("mvp"), 1, GL_FALSE,
	                   reinterpret_cast<GLfloat *>(&mvp));

	if(scene->m_should_render_grwothmesh)
	{
		scene->m_growth_mesh.draw();
	}

	if(scene->m_should_render_referencemodel)
	{
		scene->m_reference_model.draw();
	}
	*/

}
