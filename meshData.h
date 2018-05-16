#ifndef MESHDATA_H
#define MESHDATA_H

#include <linalg.hpp>
#include <vector>

using namespace std;

struct Vertex{
    vec3 Position;
    vec3 Normal;
    vec2 UV;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

/*
*	Holds the data of loaded Meshes to be used across multiple contexts
*	The data is later loaded by glMeshes
*/
class MeshData
{
public:
    MeshData(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
};

#endif // MESHDATA_H
