#include "mvppresenter.h"
#include "mainwindow.h"
#include "hairData.h"

#include <ui_mainwindow.h>
#include <QString>

MvpPresenter::MvpPresenter() noexcept: m_model_(nullptr), m_view_(nullptr)
{
}

/**
 * \brief Loads the default values stored in the model into the UI elements
 */
void MvpPresenter::load_ui_values(const UiSettings& settings) const
{
	get_view()->set_uv_grid_visibility(settings.m_grid_visibility);

	get_view()->set_hair_length(settings.m_hairlength);
	get_view()->set_hair_segment_count(settings.m_hairsegment_count);
	get_view()->set_hair_color(settings.m_hair_color);
	get_view()->set_hair_root_color(settings.m_hair_root_color);

	get_view()->set_brush_mode(settings.m_brushmode);
	get_view()->set_brush_size(settings.m_brush_size);
	get_view()->set_brush_intensity(settings.m_brush_intensity);

	get_view()->set_light_hair(settings.m_light_hair);
	get_view()->set_light_mesh(settings.m_light_mesh);
	get_view()->set_light_color(settings.m_light_color);

	get_view()->set_growthmesh_show(settings.m_growthmesh_show);
	get_view()->set_referencemodel_show(settings.m_referencemodel_show);
}


/**
 * \brief Queries the mvp model to store the hairworks hair file to disk
 * \param filename The filename of the stored hair
 */
void MvpPresenter::export_hair(const QString& filename) const
{
	if (filename.isEmpty())
		return;

	get_view()->set_statusbar_text("Exporting Hair...");

	try
	{
		const HairData& d = m_view_->get_ui()->widget_gl->m_renderer.m_hairdata_cache;
		get_model()->export_hair_to_disk(filename, d);
		get_view()->display_messagebox("Export successful!", "The hair file has successfully been exported");
	}
	catch (std::runtime_error& e)
	{
		get_view()->display_errorbox("Error exporting hairfile", e.what());
	}catch (std::exception& e)
	{
		qDebug() << e.what();
		get_view()->display_errorbox("Error exporting hairfile", "Unknown error when exporting hairfile.");
	}

	get_view()->set_statusbar_text("Ready");
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
	get_view()->get_ui()->widget_gl->m_scene.m_drawbuffer.grab_drawbuffer_content_to_image(image);

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

	get_view()->get_ui()->widget_gl->m_scene.m_drawbuffer.set_content(hairstyle);
}

void MvpPresenter::load_fbx_model(const QString& filename) const
{
	get_view()->set_growthmesh_index(0);

	get_view()->set_statusbar_text("Loading Fbx...");
	try
	{
		// Try Loading the Model data
		get_model()->load_fbx_model_from_disk(filename);

		// Try Creating the opengl buffers
		get_view()->get_ui()->widget_gl->load_glmodel_data();

		get_view()->set_growthmesh_index_content(get_model()->get_fbx_model()->m_name_list);
	}
	catch (std::runtime_error& e)
	{
		get_view()->display_errorbox("Error loading Model.", e.what());
	}catch (std::exception& e)
	{
		qDebug() << e.what();
		get_view()->display_errorbox("Error loading Model.", "Unknown error when loading model.");
	}
	get_view()->set_statusbar_text("Ready");
}

void MvpPresenter::load_project_file(const QString& filename) const
{
	Project project = get_model()->load_project_file_from_disk(filename);

	// Concat the path of the project file with the name of the growthmesh
	const QString path = filename.left(filename.lastIndexOf("/")+1).append(project.m_growthmesh_path.data());
	load_fbx_model(path);

	load_ui_values(project.m_ui_settings);
}

void MvpPresenter::save_project_file(const QString& filename) const
{
	Project project;
	project.m_ui_settings.m_grid_visibility = get_view()->get_ui()->cb_show_uv->isChecked();
	project.m_ui_settings.m_hairlength = get_view()->get_ui()->sb_hair_length->value();
	project.m_ui_settings.m_hairsegment_count = get_view()->get_ui()->sb_hair_num_segments->value();
	project.m_ui_settings.m_hair_color = get_view()->m_hair_color;
	project.m_ui_settings.m_hair_root_color = get_view()->m_hair_root_color;
	project.m_ui_settings.m_brushmode = get_view()->m_paintmode;
	project.m_ui_settings.m_brush_size = get_view()->get_ui()->sb_size->value();
	project.m_ui_settings.m_brush_intensity = get_view()->get_ui()->sb_intensity->value();
	project.m_ui_settings.m_light_hair = get_view()->get_ui()->cb_light_hair->isChecked();
	project.m_ui_settings.m_light_mesh = get_view()->get_ui()->cb_light_mesh->isChecked();
	project.m_ui_settings.m_light_color = get_view()->m_light_color;
	project.m_ui_settings.m_growthmesh_show = get_view()->get_ui()->cb_growthmesh_show->isChecked();
	project.m_ui_settings.m_referencemodel_show = get_view()->get_ui()->cb_referencemodel_show->isChecked();
	get_model()->save_project_file_to_disk(filename, project);
}
