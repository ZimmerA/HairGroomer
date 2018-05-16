#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLShaderProgram>
#include <vector>
#include <meshData.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

using namespace std;

class ModelData
{
public:
    ModelData(const char *path);
	// Meshdata for every submodel
	vector<MeshData> meshes;
private:
	string directory;


    void loadModel(string path);
	// Process a node in the model tree
    void processNode(aiNode *node, const aiScene *scene);
	// Create a meshData object out of the data
    MeshData processMesh(aiMesh * mesh, const aiScene * scene);
};

#endif // MODEL_H
