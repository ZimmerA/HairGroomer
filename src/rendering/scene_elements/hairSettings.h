#ifndef HAIR_H
#define HAIR_H
#include <linalg.hpp>

/**
 * \brief Stores general hair information used for the rendering
 */
class HairSettings
{
public:
	vec3 m_root_color;
	vec3 m_hair_color;
	float m_length;
	int m_num_segments = 4;
};

#endif