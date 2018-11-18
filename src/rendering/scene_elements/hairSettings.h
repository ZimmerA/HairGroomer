#ifndef HAIR_H
#define HAIR_H

/**
 * \brief Stores general hair information used for the rendering
 */
class HairSettings
{
public:
	glm::vec3 m_root_color{0};
	glm::vec3 m_hair_color{0};
	float m_length{1};
	int m_num_segments{1};
};

#endif