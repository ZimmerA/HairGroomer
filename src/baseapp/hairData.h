#ifndef HAIRDATA_H
#define HAIRDATA_H
#include <vector>
#include <linalg.hpp>

/**
 * \brief Stores the data of the generated hair to write to the Hairworks file, the data is retrieved using transform feedback on the geometry shader
 */
struct HairData
{
	int m_num_hair;
	int m_num_segments;
	std::vector<vec3> m_vertices;
};

#endif

