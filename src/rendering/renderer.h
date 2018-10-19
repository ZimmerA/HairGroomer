#ifndef RENDERER_H
#define RENDERER_H

#include "scene.h"
#include "rendering/GLWidget2.h"
class Renderer
{
public:
	void render(Scene * scene, GLWidget *widget);

private:
	int m_width_;
	int m_height_;

};

#endif

