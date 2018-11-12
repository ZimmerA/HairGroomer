#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rendering/scene_elements/paintbrush.h"

#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>
#include <sstream>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_ui_(new Ui::MainWindow)
{
	m_ui_->setupUi(this);
	connect_signals_and_slots();
	m_ui_->statusBar->showMessage("");
}

MainWindow::~MainWindow()
{
	delete m_ui_;
}

void MainWindow::on_actionExport_triggered()
{
	m_ui_->widget_gl->m_renderer.m_should_write_out_hair = true;
	const QString file_name = save_file_dialog("Export Hair", "Nvidia APX File (*.apx);;All Files (*)");
	get_presenter()->export_hairworks(file_name);
}

void MainWindow::on_actionOpen_Fbx_triggered()
{
	const QString file_name = open_file_dialog("Import FBX model", "Filmbox (*.FBX)");
	get_presenter()->load_fbx_model(file_name);
}

/**
 * \brief Opens a file dialog and tells the presenter to save the current hairstyle to the selected file
 */
void MainWindow::on_actionExport_Hairstyle_triggered()
{
	const QString file_name = save_file_dialog("Export Hairstyle", "PNG (*.png)");

	get_presenter()->export_hairstyle(file_name);
}

/**
 * \brief Resets the current project, first asks if the user wants to save the current project
 */
void MainWindow::on_actionNew_Project_triggered()
{
	if (display_questionbox("Save?", "Do you want to save your current project?"))
	{
		on_actionSave_Project_triggered();
	}

	get_presenter()->new_project();
}

/**
 * \brief Opens a file dialog and tells the presenter to open the project at the selected path
 */
void MainWindow::on_actionOpen_Project_triggered()
{
	const QString file_name = open_file_dialog("Open Project", "JSON (*.JSON)");
	if (!file_name.isEmpty())
	{
		if (display_questionbox("Save?", "Do you want to save your current project?"))
		{
			on_actionSave_Project_triggered();
		}
		get_presenter()->load_project_file(file_name);
	}
}

/**
 * \brief Tries to save the current project to the last opened project, otherwise opens a file dialog to select a location
 */
void MainWindow::on_actionSave_Project_triggered()
{
	if (!get_presenter()->save_project_file())
		on_actionSave_Project_as_triggered();
}

/**
 * \brief Opens a file dialog and tells the presenter to save the current project to the selected path
 */
void MainWindow::on_actionSave_Project_as_triggered()
{
	const QString file_name = save_file_dialog("Save Project", "JSON (*.JSON)");
	get_presenter()->save_project_file_as(file_name);
}

/**
 * \brief Opens a file dialog and tells the presenter to load the selected file (picture) as the current hairstyle
 */
void MainWindow::on_actionLoad_Hairstyle_triggered()
{
	const QString file_name = open_file_dialog("Load Hairstyle", "PNG (*.png)");
	get_presenter()->load_hairstyle(file_name);
}

/**
 * \brief Displays an info messagebox
 * \param title The title of the messagebox
 * \param content The text in the body of the messagebox
 */
void MainWindow::display_messagebox(const char* title, const char* content)
{
	QMessageBox box;
	std::stringstream ss;
	ss << "<p>" << content << "</p>";
	box.information(this, tr(title),
	                ss.str().data());
	box.setFixedSize(500, 400);
}

/**
 * \brief Displays an error messagebox
 * \param title The title of the messagebox
 * \param content The text in the body of the messagebox
 */
void MainWindow::display_errorbox(const char* title, const char* content)
{
	QMessageBox box;
	std::stringstream ss;
	ss << "<p>" << content << "</p>";
	box.critical(this, tr(title), ss.str().data());
	box.setFixedSize(1000, 400);
}

/**
 * \brief Displays a questionbox with the given title and message and the buttons yes and no
 * \param title The title of the messagebox
 * \param question The question inside the mesagebox
 * \return True if yes was clicked false otherwise
 */
bool MainWindow::display_questionbox(const char* title, const char* question)
{
	const QMessageBox::StandardButton reply = QMessageBox::question(this, title, question,
	                                                                QMessageBox::Yes | QMessageBox::No);
	return reply == QMessageBox::Yes;
}

void MainWindow::set_statusbar_text(const std::string& text) const
{
	m_ui_->statusBar->showMessage(text.data());
}

/**
 * \brief Opens a file dialog to select a file to save to
 * \param title The title of the dialog
 * \param types the file types that can be selected using this dialog
 * \return The path to the selected file
 */
QString MainWindow::save_file_dialog(const char* title, const char* types)
{
	return QFileDialog::getSaveFileName(this, tr(title), "",
	                                    tr(types));
}

/**
 * \brief Opens a file dialog to select a file to open
 * \param title The title of the dialog
 * \param types the file types that can be selected using this dialog
 * \return The path to the selected file
 */
QString MainWindow::open_file_dialog(const char* title, const char* types)
{
	return QFileDialog::getOpenFileName(this, tr(title), "",
	                                    tr(types));
}

/**
 * \brief Connects the signals of the shortcuts to the slots of the according ui, connects the Signals of the ui Elements to their listeners
 */
void MainWindow::connect_signals_and_slots()
{
	m_hotkey_toggle_uv_ = new QShortcut(this);
	m_hotkey_toggle_uv_->setKey(Qt::Key_D);
#pragma warning(suppress: 26444)
	connect(m_hotkey_toggle_uv_, SIGNAL(activated()), m_ui_->cb_show_uv, SLOT(toggle()));
#pragma warning(suppress: 26444)
	connect(m_ui_->cb_show_uv,SIGNAL(toggled(bool)), this, SLOT(uv_visibility_changed_listener(bool)));

#pragma warning(suppress: 26444)
	connect(m_ui_->cmb_growthmesh_index,SIGNAL(currentIndexChanged(int)), this,
	        SLOT(growthmesh_index_changed_listener(int)));
#pragma warning(suppress: 26444)
	connect(m_ui_->cmb_up_axis,SIGNAL(currentIndexChanged(int)), this,
	        SLOT(up_axis_changed_listener(int)));
	/*BRUSH SETTINGS*/
	// Brush intensity
	m_hotkey_brushintensity_reduce_ = new QShortcut(this);
	m_hotkey_brushintensity_reduce_->setKey(Qt::Key_M);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushintensity_reduce_, SIGNAL(activated()), m_ui_->sb_intensity, SLOT(stepDown()));

	m_hotkey_brushintensity_increase_ = new QShortcut(this);
	m_hotkey_brushintensity_increase_->setKey(Qt::Key_P);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushintensity_increase_, SIGNAL(activated()), m_ui_->sb_intensity, SLOT(stepUp()));

#pragma warning(suppress: 26444)
	connect(m_ui_->sb_intensity, SIGNAL(valueChanged(double)), this, SLOT(brush_intensity_changed_listener(double)));

	// Brush size
	m_hotkey_brushsize_reduce_ = new QShortcut(this);
	m_hotkey_brushsize_reduce_->setKey(Qt::Key_Minus);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushsize_reduce_, SIGNAL(activated()), m_ui_->sb_size, SLOT(stepDown()));

	m_hotkey_brushsize_increase_ = new QShortcut(this);
	m_hotkey_brushsize_increase_->setKey(Qt::Key_Plus);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushsize_increase_, SIGNAL(activated()), m_ui_->sb_size, SLOT(stepUp()));
#pragma warning(suppress: 26444)
	connect(m_ui_->sb_size, SIGNAL(valueChanged(double)), this, SLOT(brush_size_changed_listener(double)));

	// Brush mode
	m_hotkey_brushmode_hair_ = new QShortcut(this);
	m_hotkey_brushmode_hair_->setKey(Qt::Key_L);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushmode_hair_, SIGNAL(activated()), m_ui_->rb_brushmode_hair, SLOT(toggle()));

	m_hotkey_brushmode_curl_ = new QShortcut(this);
	m_hotkey_brushmode_curl_->setKey(Qt::Key_C);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushmode_curl_, SIGNAL(activated()), m_ui_->rb_brushmode_curl, SLOT(toggle()));

	m_hotkey_brushmode_twist_ = new QShortcut(this);
	m_hotkey_brushmode_twist_->setKey(Qt::Key_T);
#pragma warning(suppress: 26444)
	connect(m_hotkey_brushmode_twist_, SIGNAL(activated()), m_ui_->rb_brushmode_twist, SLOT(toggle()));
	m_ui_->brushmodeGroup->setId(m_ui_->rb_brushmode_hair, 0);
	m_ui_->brushmodeGroup->setId(m_ui_->rb_brushmode_curl, 1);
	m_ui_->brushmodeGroup->setId(m_ui_->rb_brushmode_twist, 2);
#pragma warning(suppress: 26444)
	connect(m_ui_->brushmodeGroup, SIGNAL(buttonToggled(int, bool)), this,SLOT(brush_mode_changed_listener(int, bool)));

	/* HAIR LISTENERS */
	// Segment count
#pragma warning(suppress: 26444)
	connect(m_ui_->sb_hair_num_segments,SIGNAL(valueChanged(int)), this,
	        SLOT(hair_segment_count_changed_listener(int)));
	// Hair length
#pragma warning(suppress: 26444)
	connect(m_ui_->sb_hair_length,SIGNAL(valueChanged(double)), this, SLOT(hair_length_changed_listener(double)));

	// Hair + Hair root color
#pragma warning(suppress: 26444)
	connect(m_ui_->b_hair_color,SIGNAL(clicked(bool)), this, SLOT(hair_color_clicked_listener()));
#pragma warning(suppress: 26444)
	connect(m_ui_->b_hair_root_color, SIGNAL(clicked(bool)), this, SLOT(hair_root_color_clicked_listener()));

	/* Light Settings */
	// Should light Hair
#pragma warning(suppress: 26444)
	connect(m_ui_->cb_light_hair,SIGNAL(toggled(bool)), this, SLOT(light_hair_changed_listener(bool)));
	// Should light mesh
#pragma warning(suppress: 26444)
	connect(m_ui_->cb_light_mesh,SIGNAL(toggled(bool)), this, SLOT(light_mesh_changed_listener(bool)));
	// Light color
#pragma warning(suppress: 26444)
	connect(m_ui_->b_light_color,SIGNAL(clicked(bool)), this, SLOT(light_color_clicked_listener()));

	// Mesh settings
	// Should show grwothmesh
#pragma warning(suppress: 26444)
	connect(m_ui_->cb_growthmesh_show,SIGNAL(toggled(bool)), this, SLOT(growthmesh_show_changed_listener(bool)));
	// Should show referencemodel
#pragma warning(suppress: 26444)
	connect(m_ui_->cb_referencemodel_show,SIGNAL(toggled(bool)), this,
	        SLOT(referencemodel_show_changed_listener(bool)));
}

/**
 * \brief Listener that gets called when the uv grid is turned on or off
 * \param enabled Determines whether the uv grid should be visible
 */
void MainWindow::uv_visibility_changed_listener(const bool enabled) const
{
	m_ui_->widget_gl->m_renderer.m_should_render_uv_overlay = enabled;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the selected growthmesh is changed
 * \param index The growthmesh index
 */
void MainWindow::growthmesh_index_changed_listener(const int index) const
{
	m_ui_->widget_gl->m_scene.m_growth_mesh_index = index;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the selected up axis is changed
 * \param index The selected index (0 = y, 1 = z)
 */
void MainWindow::up_axis_changed_listener(const int index) const
{
	m_ui_->widget_gl->m_scene.set_up_axis(index);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the hair lighting is turned on or off
 * \param enabled The state of the hair lighting
 */
void MainWindow::light_hair_changed_listener(const bool enabled) const
{
	m_ui_->widget_gl->m_renderer.m_should_light_hair = enabled;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the mesh lighting is turned on or off
 * \param enabled The state of the mesh lighting
 */
void MainWindow::light_mesh_changed_listener(const bool enabled) const
{
	m_ui_->widget_gl->m_renderer.m_should_light_meshes = enabled;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the light color button is clicked. Opens a color dialog
 */
void MainWindow::light_color_clicked_listener()
{
	// Connect the listener of the select color button
#pragma warning(suppress: 26444)
	connect(&m_color_picker_dialog_, SIGNAL(colorSelected(QColor)), this, SLOT(light_color_selected_listener(QColor)));
	m_color_picker_dialog_.show();
}

/**
 * \brief Listener that gets called when the user selects a color after opening the color dialog via the light color button
 * \param color The selected color
 */
void MainWindow::light_color_selected_listener(const QColor& color)
{
	// Disconnect the listener of the select color button
	disconnect(&m_color_picker_dialog_, nullptr, nullptr, nullptr);
	m_light_color = QColor(color);
	// Set the new color of the button
	const QString s("background-color: " + color.name() + ";");
	m_ui_->b_light_color->setStyleSheet(s);
	m_ui_->widget_gl->m_scene.m_light.m_color = glm::vec3(color.red() / 255.0f, color.green() / 255.0f,
	                                                      color.blue() / 255.0f);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the amount of hair segments has been changed
 * \param segments The amount of segments per hair
 */
void MainWindow::hair_segment_count_changed_listener(const int segments) const
{
	m_ui_->widget_gl->m_scene.m_hair.m_num_segments = segments;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the hair length has been changed
 * \param length The length of the hair
 */
void MainWindow::hair_length_changed_listener(const double length) const
{
	m_ui_->widget_gl->m_scene.m_hair.m_length = length;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the hair color button is clicked. Opens a color dialog
 */
void MainWindow::hair_color_clicked_listener()
{
	// Connect the listener of the select color button
#pragma warning(suppress: 26444)
	connect(&m_color_picker_dialog_, SIGNAL(colorSelected(QColor)), this, SLOT(hair_color_selected_listener(QColor)));
	m_color_picker_dialog_.show();
}

/**
 * \brief Listener that gets called when the hair root color button is clicked. Opens a color dialog
 */
void MainWindow::hair_root_color_clicked_listener()
{
	// Connect the listener of the select color button
#pragma warning(suppress: 26444)
	connect(&m_color_picker_dialog_, SIGNAL(colorSelected(QColor)), this,
	        SLOT(hair_root_color_selected_listener(QColor)));
	m_color_picker_dialog_.show();
}

/**
 * \brief Listener that gets called when the user selects a color after opening the color dialog via the hair color button
 * \param color The selected color
 */
void MainWindow::hair_color_selected_listener(const QColor& color)
{
	// Disconnect the listener of the select color button
	disconnect(&m_color_picker_dialog_, nullptr, nullptr, nullptr);
	m_hair_color = QColor(color);
	const QString s("background-color: " + color.name() + ";");
	m_ui_->b_hair_color->setStyleSheet(s);
	m_ui_->widget_gl->m_scene.m_hair.m_hair_color = glm::vec3(color.red() / 255.0f, color.green() / 255.0f,
	                                                          color.blue() / 255.0f);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the user selects a color after opening the color dialog via the root color button
 * \param color The selected color
 */
void MainWindow::hair_root_color_selected_listener(const QColor& color)
{
	// Disconnect the listener of the select color button
	disconnect(&m_color_picker_dialog_, nullptr, nullptr, nullptr);
	m_hair_root_color = QColor(color);
	const QString s("background-color: " + color.name() + ";");
	m_ui_->b_hair_root_color->setStyleSheet(s);
	m_ui_->widget_gl->m_scene.m_hair.m_root_color = glm::vec3(color.red() / 255.0f, color.green() / 255.0f,
	                                                          color.blue() / 255.0f);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the brush intensity has been changed
 * \param intensity Intensity of the brush
 */
void MainWindow::brush_intensity_changed_listener(const double intensity) const
{
	m_ui_->widget_gl->m_scene.m_brush.set_brush_intensity(intensity);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the brush size has been changed
 * \param size Size of the brush
 */
void MainWindow::brush_size_changed_listener(const double size) const
{
	m_ui_->widget_gl->m_scene.m_brush.set_brush_size(size);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the brush mode has been changed
 * \param mode The id of the radiobutton which determines the brush mode
 * \param checked True if the button is checked
 */
void MainWindow::brush_mode_changed_listener(int mode, const bool checked)
{
	if (checked)
	{
		m_paintmode = static_cast<Paintbrush::paintmode>(mode);
		m_ui_->widget_gl->m_scene.m_brush.set_paintmode(m_paintmode);
		m_ui_->widget_gl->update();
	}
}

/**
 * \brief Listener that gets called when the growthmesh rendering gets turned on or off
 * \param enabled The state of the growthmesh rendering
 */
void MainWindow::growthmesh_show_changed_listener(const bool enabled) const
{
	m_ui_->widget_gl->m_renderer.m_should_render_growthmesh = enabled;
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the reference model rendering gets turned on or off
 * \param enabled The state of the reference model rendering
 */
void MainWindow::referencemodel_show_changed_listener(const bool enabled) const
{
	m_ui_->widget_gl->m_renderer.m_should_render_refrencemodel = enabled;
	m_ui_->widget_gl->update();
}

/* Setters for the UI elements*/
void MainWindow::set_uv_grid_visibility(const bool visible) const
{
	m_ui_->cb_show_uv->setChecked(visible);
}

void MainWindow::set_hair_length(const double length) const
{
	m_ui_->sb_hair_length->setValue(length);
}

void MainWindow::set_hair_segment_count(const int segments) const
{
	m_ui_->sb_hair_num_segments->setValue(segments);
}

void MainWindow::set_hair_color(const QColor& color)
{
	hair_color_selected_listener(color);
}

void MainWindow::set_hair_root_color(const QColor& color)
{
	hair_root_color_selected_listener(color);
}

void MainWindow::set_brush_mode(const Paintbrush::paintmode mode) const
{
	switch (mode)
	{
	case Paintbrush::paintmode::length:
		m_ui_->rb_brushmode_hair->click();
		break;
	case Paintbrush::paintmode::curl:
		m_ui_->rb_brushmode_curl->click();
		break;
	case Paintbrush::paintmode::twist:
		m_ui_->rb_brushmode_twist->click();
		break;
	default:
		break;
	}
}

void MainWindow::set_brush_size(const double size) const
{
	m_ui_->sb_size->setValue(size);
}

void MainWindow::set_brush_intensity(const double intensity) const
{
	m_ui_->sb_intensity->setValue(intensity);
}

void MainWindow::set_light_hair(const bool active) const
{
	m_ui_->cb_light_hair->setChecked(active);
}

void MainWindow::set_light_mesh(const bool active) const
{
	m_ui_->cb_light_mesh->setChecked(active);
}

void MainWindow::set_light_color(const QColor& color)
{
	light_color_selected_listener(color);
}

void MainWindow::set_growthmesh_show(const bool active) const
{
	m_ui_->cb_growthmesh_show->setChecked(active);
}

void MainWindow::set_referencemodel_show(const bool active) const
{
	m_ui_->cb_referencemodel_show->setChecked(active);
}

void MainWindow::set_up_axis_index(const int index) const
{
	m_ui_->cmb_up_axis->setCurrentIndex(index);
}

void MainWindow::set_growthmesh_index(const int index) const
{
	m_ui_->cmb_growthmesh_index->setCurrentIndex(index);
}

void MainWindow::set_growthmesh_index_content(std::vector<std::string> content) const
{
	m_ui_->cmb_growthmesh_index->clear();

	for (auto& item : content)
	{
		m_ui_->cmb_growthmesh_index->addItem(QString(item.data()));
	}
}

void MainWindow::clear_growthmesh_index_content() const
{
	m_ui_->cmb_growthmesh_index->clear();
}

/**
 * \brief Returns a UiSettings object that holds the current settings
 * \return The current UiSettings
 */
UiSettings MainWindow::get_ui_settings() const
{
	UiSettings settings;

	// General settings
	settings.m_grid_visibility = get_ui()->cb_show_uv->isChecked();
	settings.m_growth_mesh_index = get_ui()->cmb_growthmesh_index->currentIndex();
	settings.m_up_axis_index = get_ui()->cmb_up_axis->currentIndex();
	// Hair settings
	settings.m_hairlength = get_ui()->sb_hair_length->value();
	settings.m_hairsegment_count = get_ui()->sb_hair_num_segments->value();
	settings.m_hair_color = m_hair_color;
	settings.m_hair_root_color = m_hair_root_color;
	// Brush settings
	settings.m_brushmode = m_paintmode;
	settings.m_brush_size = get_ui()->sb_size->value();
	settings.m_brush_intensity = get_ui()->sb_intensity->value();
	// Light settings
	settings.m_light_hair = get_ui()->cb_light_hair->isChecked();
	settings.m_light_mesh = get_ui()->cb_light_mesh->isChecked();
	settings.m_light_color = m_light_color;
	// Mesh settings
	settings.m_growthmesh_show = get_ui()->cb_growthmesh_show->isChecked();
	settings.m_referencemodel_show = get_ui()->cb_referencemodel_show->isChecked();

	return settings;
}

/**
 * \brief Loads the given UiSettings into the UI
 * \param settings The UiSettings to be loaded
 */
void MainWindow::set_ui_settings(const UiSettings& settings)
{
	// General settings
	set_uv_grid_visibility(settings.m_grid_visibility);
	set_growthmesh_index(settings.m_growth_mesh_index);
	set_up_axis_index(settings.m_up_axis_index);
	// Hair settings
	set_hair_length(settings.m_hairlength);
	set_hair_segment_count(settings.m_hairsegment_count);
	set_hair_color(settings.m_hair_color);
	set_hair_root_color(settings.m_hair_root_color);
	// Brush settings
	set_brush_mode(settings.m_brushmode);
	set_brush_size(settings.m_brush_size);
	set_brush_intensity(settings.m_brush_intensity);
	// Light settings
	set_light_hair(settings.m_light_hair);
	set_light_mesh(settings.m_light_mesh);
	set_light_color(settings.m_light_color);
	// Mesh settings
	set_growthmesh_show(settings.m_growthmesh_show);
	set_referencemodel_show(settings.m_referencemodel_show);
}
