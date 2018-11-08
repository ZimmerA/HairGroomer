#include "mvppresenter.h"
#include "mainwindow.h"
#include "hairData.h"

#include <ui_mainwindow.h>
#include <QString>

QString get_filename_from_path(const QString& path)
{
	QString filename = path.mid(path.lastIndexOf('/')+1);
	return filename;
}

QString get_directory_from_path(const QString& path)
{
	QString directory = path.left(path.lastIndexOf("/") + 1);
	return directory;
}

MvpPresenter::MvpPresenter() noexcept: m_model_(nullptr), m_view_(nullptr)
{
}

/**
 * \brief Tries to store the hairworks file to disk
 * \param file_path The path to the resulting hairworks file
 */
void MvpPresenter::export_hair(const QString& file_path) const
{
	if (file_path.isEmpty())
		return;

	get_view()->set_statusbar_text("Exporting Hair...");

	try
	{
		const HairData& d = m_view_->get_ui()->widget_gl->m_renderer.m_hairdata_cache;
		get_model()->export_hair_to_disk(file_path, d, m_view_->get_ui()->cmb_up_axis->currentIndex());
		get_view()->display_messagebox("Export successful!", "The hair file has successfully been exported");
	}
	catch (std::runtime_error& e)
	{
		get_view()->display_errorbox("Error exporting hairfile", e.what());
	}catch (...)
	{
		get_view()->display_errorbox("Error exporting hairfile", "Unknown error when exporting hairfile.");
	}

	get_view()->set_statusbar_text("Ready");
}

/**
 * \brief Tries to store the current hairstyle to disk
 * \param file_path The path to the exported hairstyle image
 */
void MvpPresenter::export_hairstyle(const QString& file_path) const
{
	if (file_path.isEmpty())
		return;

	QImage image(10, 10, QImage::Format_RGBA8888);
	get_view()->get_ui()->widget_gl->m_scene.m_drawbuffer.grab_drawbuffer_content_to_image(image);

	const bool sucess = get_model()->export_hairstyle_to_disk(image, file_path);

	if (!sucess)
		get_view()->display_messagebox("Hairstyle could not be saved.", "");
	else
		get_model()->m_loaded_hairstyle_name = get_filename_from_path(file_path).toLocal8Bit().data();
}

/**
 * \brief Tries to load a hairstyle from disk and puts it into the drawbuffer
 * \param file_path The path to the hairstyle image
 */
void MvpPresenter::load_hairstyle(const QString& file_path) const
{
	if (file_path.isEmpty())
		return;

	QImage hairstyle = get_model()->load_hairstyle_from_disk(file_path);

	if (hairstyle.isNull())
		get_view()->display_messagebox("Couldn't load file", "");

	get_model()->m_loaded_hairstyle_name = get_filename_from_path(file_path).toLocal8Bit().data();
	get_view()->get_ui()->widget_gl->m_scene.m_drawbuffer.set_content(hairstyle);
}

/**
 * \brief Tries to load the specified fbx model data and create buffers for it in opengl
 * \param file_path The path to the fbx file
 */
void MvpPresenter::load_fbx_model(const QString& file_path) const
{
	if (file_path.isEmpty())
		return;

	get_view()->set_growthmesh_index(0);

	get_view()->set_statusbar_text("Loading Fbx...");
	try
	{
		// Try Loading the Model data
		get_model()->load_fbx_model_from_disk(file_path);

		// Try Creating the opengl buffers
		get_view()->get_ui()->widget_gl->load_glmodel_data();

		get_view()->set_growthmesh_index_content(get_model()->get_fbx_model()->m_name_list);
	}
	catch (std::runtime_error& e)
	{
		get_view()->display_errorbox("Error loading Model.", e.what());
	}catch (...)
	{
		get_view()->display_errorbox("Error loading Model.", "Unknown error when loading model.");
	}

	get_view()->set_statusbar_text("Ready");
}

/**
 * \brief Resets the current project, ui elements and rendering data
 */
void MvpPresenter::new_project() const
{
	get_model()->m_loaded_project_path = "";
	get_model()->m_loaded_hairstyle_name = "";
	// Reset the opengl scene
	get_view()->get_ui()->widget_gl->m_scene.reset();
	// Reset the modeldata
	get_model()->reset_fbx_model();
	// Reset the UI to standard settings
	get_view()->clear_growthmesh_index_content();
	get_view()->set_ui_settings(UiSettings{});
}

/**
 * \brief Tries to open the project file at the specified path
 * \param file_path The path to the file to open
 */
void MvpPresenter::load_project_file(const QString& file_path) const
{
	if (file_path.isEmpty())
		return;

	ProjectSettings project;

	try
	{
		project = get_model()->load_project_file_from_disk(file_path);
	}catch(...)
	{
		get_view()->display_errorbox("Error", "The project file you were trying to load is corrupted.");
		return;
	}

	// if the project includes a fbx model name look for it in the same path as the project file
	if (!project.m_growthmesh_name.empty())
	{

		const QString path = get_directory_from_path(file_path).append(project.m_growthmesh_name.data());
		load_fbx_model(path);
	}

	// if the project includes a hairstyle name look for it in the same path as the project file
	if (!project.m_hair_style_name.empty())
	{
		const QString path = get_directory_from_path(file_path).append(project.m_hair_style_name.data());
		load_hairstyle(path);
	}

	get_view()->set_ui_settings(project.m_ui_settings);
	get_model()->m_loaded_project_path = file_path.toLocal8Bit().constData();
}

/**
 * \brief Tries to save the current project to the file it was last saved to or opened with
 * \return True if the file could be saved, false if not
 */
bool MvpPresenter::save_project_file() const
{
	if(!get_model()->m_loaded_project_path.empty())
	{
		save_project_file_as(get_model()->m_loaded_project_path.data());

		return true;
	}

	return false;
}

/**
 * \brief Saves the current project to the specified file
 * \param file_path The path to the file to save to
 */
void MvpPresenter::save_project_file_as(const QString& file_path) const
{
	if(file_path.isEmpty())
		return;

	ProjectSettings project;
	project.m_ui_settings = get_view()->get_ui_settings();

	// If an fbx model is currently loaded, store it's name in the project
	if(get_model()->get_fbx_model())
	{
		project.m_growthmesh_name = get_model()->get_fbx_model()->m_name.toLocal8Bit().constData();
	}

	QString hair_file_path;
	// If no hairstyle is currently loaded export the hairstyle as Hairstyle.png into the project directory
	if(get_model()->m_loaded_hairstyle_name.empty())
	{
		hair_file_path = get_directory_from_path(file_path).append("Hairstyle.png");
		project.m_hair_style_name = "Hairstyle.png";
	}else // Export it to the project directory using the name of the previously opened Hairstyle
	{
		hair_file_path = get_directory_from_path(file_path).append(get_model()->m_loaded_hairstyle_name.data());
		project.m_hair_style_name = get_model()->m_loaded_hairstyle_name;
	}
	
	export_hairstyle(hair_file_path);
	get_model()->save_project_file_to_disk(file_path, project);
	get_model()->m_loaded_project_path = file_path.toLocal8Bit().constData();
}
