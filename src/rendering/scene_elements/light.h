#ifndef LIGHT_H
#define LIGHT_H
#include <linalg.hpp>

/**
 * \brief stores general light information
 */
class Light
{
public:
	vec3 m_position = vec3(0.f,550.0f,0.f);
	vec3 m_color = vec3(1,1,1);
};

#endif

