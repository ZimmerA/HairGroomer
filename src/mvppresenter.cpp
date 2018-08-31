#include "mvppresenter.h"
#include "mainwindow.h"
#include <ui_mainwindow.h>

MvpPresenter::MvpPresenter(): m_model_(nullptr), m_view_(nullptr)
{
}

/**
 * \brief Loads the default values stored in the model into the UI elements
 */
void MvpPresenter::load_default_values() const
{
	get_view()->set_uv_grid_visibility(get_model()->m_grid_visibility_default);

	get_view()->set_hair_length(get_model()->m_hairlength_default);
	get_view()->set_hair_segment_count(get_model()->m_hairsegment_count_default);
	get_view()->set_hair_color(get_model()->m_hair_color_default);
	get_view()->set_hair_root_color(get_model()->m_hair_root_color_default);

	get_view()->set_brush_mode(get_model()->m_brushmode_default);
	get_view()->set_brush_size(get_model()->m_brush_size_default);
	get_view()->set_brush_intensity(get_model()->m_brush_intensity_default);

	get_view()->set_light_hair(get_model()->m_light_hair_default);
	get_view()->set_light_mesh(get_model()->m_light_mesh_default);
	get_view()->set_light_color(get_model()->m_light_color_default);


	get_view()->set_growthmesh_show(get_model()->m_growthmesh_show_default);
	get_view()->set_referencemodel_show(get_model()->m_referencemodel_show_default);
}


/**
 * \brief Queries the model to store the hairworks hair file (not working yet)
 * \param filename The filename of the stored hair
 */
void MvpPresenter::export_hair(const QString& filename) const
{
	if (filename.isEmpty())
		return;

	const bool success = get_model()->export_hair_to_disk(filename);

	if (!success)
		get_view()->display_messagebox("Unable to open file", "The selected file could not be opened");
}

/**
 * \brief Queries the model to store the current hairstyle to disk
 * \param filename The filename of the exported hairstyle
 */
void MvpPresenter::export_hairstyle(const QString& filename) const
{
	if (filename.isEmpty())
		return;

	QImage image(10, 10, QImage::Format_RGBA8888);
	get_view()->get_ui()->widget_gl->grab_drawbuffer_content_to_image(image);

	const bool sucess = get_model()->export_hairstyle_to_disk(image, filename);

	if (!sucess)
		get_view()->display_messagebox("Hairstyle could not be saved.", "");
}

/**
 * \brief Queries the model to load a hairstyle from the disk and applies it to the hair draw framebuffer
 * \param filename The hairstyle image to be loaded
 */
void MvpPresenter::load_hairstyle(const QString& filename) const
{
	if (filename.isEmpty())
		return;

	QImage hairstyle = get_model()->load_hairstyle_from_disk(filename);

	if (hairstyle.isNull())
		get_view()->display_messagebox("Couldn't load file", "");

	get_view()->get_ui()->widget_gl->set_drawbuffer_content(hairstyle);
}
