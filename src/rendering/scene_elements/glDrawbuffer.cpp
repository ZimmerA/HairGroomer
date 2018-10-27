#include "glDrawbuffer.h"

#include <qopenglfunctions.h>
#include <QImage>
#include <QOpenGLFunctions_3_3_Core>


void GlDrawbuffer::create()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	m_drawbuffer_ = std::make_unique<QOpenGLFramebufferObject>(800, 600);
	m_drawbuffer_->bind();
	f->glClearColor(0.f, 0.5f, 0.5f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT);
	m_drawbuffer_->release();
}

void GlDrawbuffer::bind() const
{
	m_drawbuffer_->bind();
}

void GlDrawbuffer::release() const
{
	m_drawbuffer_->release();
}

/**
 * \brief Load a given QImage into the painting framebuffer
 * \param image The image
 */
void GlDrawbuffer::set_content(QImage& image) const
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGBA,GL_UNSIGNED_BYTE, image.bits());
}

/**
 * \brief Takes the content of the painting framebuffer and stores it in an image
 * \param image the returned image
 */
void GlDrawbuffer::grab_drawbuffer_content_to_image(QImage& image) const
{
	GLint width = 0, height = 0;
	auto* f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	f->glBindTexture(GL_TEXTURE_2D, m_drawbuffer_->texture());
	f->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	f->glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	auto resized = image.scaled(width, height).convertToFormat(QImage::Format_RGBA8888);

	f->glPixelStorei(GL_PACK_ALIGNMENT, 1);
	f->glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, resized.bits());
	image.swap(resized);
}

/**
 * \brief Clears the content of the drawbuffer
 */
void GlDrawbuffer::reset() const
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	m_drawbuffer_->bind();
	f->glViewport(0, 0, 800, 600);
	f->glScissor(0, 0, 800, 600);
	f->glClearColor(0.f, 0.5f, 0.5f, 1.0f);
	f->glClear(GL_COLOR_BUFFER_BIT);
	m_drawbuffer_->release();
}

int GlDrawbuffer::get_texture_handle() const
{
	return m_drawbuffer_->texture();
}
