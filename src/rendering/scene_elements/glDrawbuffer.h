#ifndef DRAWBUFFER_H
#define DRAWBUFFER_H

#include <memory>

#include <QOpenGLFramebufferObject>

class GLDrawbuffer
{
public:

	void create();
	void bind() const;
	void release() const;
	void grab_drawbuffer_content_to_image(QImage &image) const;
	void reset() const;

	// Getters/Setters
	int get_texture_handle() const;
	void set_content(QImage &image) const;

private:

	std::unique_ptr<QOpenGLFramebufferObject> m_drawbuffer_;
};

#endif