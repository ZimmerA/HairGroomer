#ifndef GLMESH_H
#define GLMESH_H

#include "baseapp/meshData.h"

#include <qopenglvertexarrayobject.h>
#include <qopenglbuffer.h>


/*
* Used by the GLWidget to draw meshes
*/
class GlMesh
{
public:
	GlMesh();
	void draw();
	void draw_points();
	// Creates buffers and fills them with the mesh data
	void setup_buffers(MeshData* mesh_data);
	// Calls destroy on all buffers of the mesh
	void destroy_buffers();
	
	unsigned int m_indicie_amount;
	unsigned int m_vertex_count;
private:
	QOpenGLVertexArrayObject m_vao_;
	QOpenGLBuffer m_vbo_;
	QOpenGLBuffer m_ibo_;

};

#endif // GLMESH_H
