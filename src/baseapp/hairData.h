#ifndef HAIRDATA_H
#define HAIRDATA_H

#include <vector>

/**
 * \brief Stores the data of the generated hair. Used to write to the HairWorks file, the data is retrieved using transform feedback on the geometry shader
 */
struct HairData
{
	int m_num_hair{-1};
	int m_num_segments{-1};
	int m_growthmesh_index{0};
	std::vector<glm::vec3> m_vertices;
};

#endif