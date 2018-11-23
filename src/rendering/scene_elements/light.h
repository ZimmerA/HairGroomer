#ifndef LIGHT_H
#define LIGHT_H

/**
 * \brief stores general light information
 */
class Light
{
public:

	glm::vec3 m_position{0.f,550.0f,0.f};
	glm::vec3 m_color{1,1,1};
};

#endif