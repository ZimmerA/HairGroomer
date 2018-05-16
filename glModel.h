#ifndef GLMODEL_H
#define GLMODEL_H
#include <vector>
#include <glMesh.h>
#include <modelData.h>
#include <qopenglshaderprogram.h>
#include <memory> // unique pointer

class GlModel
{
public:
    GlModel();
	void draw(QOpenGLShaderProgram * shader);
	// Takes modeldata and creates the according glMeshes
	void setupModel(ModelData * model);
	void cleanupModel();

private:
    vector<std::unique_ptr<GlMesh>> meshes;


};

#endif // GLMODEL_H
