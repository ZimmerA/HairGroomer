#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>
#include "opengl/Paintbrush.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_ui_(new Ui::MainWindow)
{
	m_ui_->setupUi(this);
	connect_signals_and_slots();
}

MainWindow::~MainWindow()
{
	delete m_ui_;
}

/**
 * \brief Reserved for future versions of the software
 */
void MainWindow::on_actionExport_triggered() const
{
	/*
	 *	const QString file_name = save_file_dialog("Export Hair","Nvidia APX File (*.apx);;All Files (*)" );
	 *	get_presenter()->export_hair(file_name);
	 *
	 */
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
 * \brief Opens a file dialog and tells the presenter to load the selected file(picture) as the current hairstyle
 */
void MainWindow::on_actionLoad_Hairstyle_triggered()
{
	const QString file_name = open_file_dialog("Load Hairstyle", "PNG (*.png)");
	get_presenter()->load_hairstyle(file_name);
}


/**
 * \brief Displays a info mesagebox
 * \param title The title of the messagebox
 * \param content The text in the body of the messagebox
 */
void MainWindow::display_messagebox(const char* title, const char* content)
{
		QMessageBox::information(this, tr(title),
		                         content);
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
	connect(m_hotkey_toggle_uv_, SIGNAL(activated()), m_ui_->cb_show_uv, SLOT(toggle()));
	connect(m_ui_->cb_show_uv,SIGNAL(toggled(bool)), this, SLOT(uv_visibility_changed_listener(bool)));

	/*BRUSH SETTINGS*/
	// Brush intensity
	m_hotkey_brushintensity_reduce_ = new QShortcut(this);
	m_hotkey_brushintensity_reduce_->setKey(Qt::Key_M);
	connect(m_hotkey_brushintensity_reduce_, SIGNAL(activated()), m_ui_->sb_intensity, SLOT(stepDown()));

	m_hotkey_brushintensity_increase_ = new QShortcut(this);
	m_hotkey_brushintensity_increase_->setKey(Qt::Key_P);
	connect(m_hotkey_brushintensity_increase_, SIGNAL(activated()), m_ui_->sb_intensity, SLOT(stepUp()));

	connect(m_ui_->sb_intensity, SIGNAL(valueChanged(double)), this, SLOT(brush_intensity_changed_listener(double)));

	// Brush size
	m_hotkey_brushsize_reduce_ = new QShortcut(this);
	m_hotkey_brushsize_reduce_->setKey(Qt::Key_Minus);
	connect(m_hotkey_brushsize_reduce_, SIGNAL(activated()), m_ui_->sb_size, SLOT(stepDown()));

	m_hotkey_brushsize_increase_ = new QShortcut(this);
	m_hotkey_brushsize_increase_->setKey(Qt::Key_Plus);
	connect(m_hotkey_brushsize_increase_, SIGNAL(activated()), m_ui_->sb_size, SLOT(stepUp()));
	connect(m_ui_->sb_size, SIGNAL(valueChanged(double)), this, SLOT(brush_size_changed_listener(double)));

	// Brush mode
	m_hotkey_brushmode_hair_ = new QShortcut(this);
	m_hotkey_brushmode_hair_->setKey(Qt::Key_L);
	connect(m_hotkey_brushmode_hair_, SIGNAL(activated()), m_ui_->rb_brushmode_hair, SLOT(toggle()));

	m_hotkey_brushmode_curl_ = new QShortcut(this);
	m_hotkey_brushmode_curl_->setKey(Qt::Key_C);
	connect(m_hotkey_brushmode_curl_, SIGNAL(activated()), m_ui_->rb_brushmode_curl, SLOT(toggle()));

	m_hotkey_brushmode_twist_ = new QShortcut(this);
	m_hotkey_brushmode_twist_->setKey(Qt::Key_T);
	connect(m_hotkey_brushmode_twist_, SIGNAL(activated()), m_ui_->rb_brushmode_twist, SLOT(toggle()));
	m_ui_->brushmodeGroup->setId(m_ui_->rb_brushmode_hair, 0);
	m_ui_->brushmodeGroup->setId(m_ui_->rb_brushmode_curl, 1);
	m_ui_->brushmodeGroup->setId(m_ui_->rb_brushmode_twist, 2);
	connect(m_ui_->brushmodeGroup, SIGNAL(buttonToggled(int, bool)), this,SLOT(brush_mode_changed_listener(int, bool)));

	/* HAIR LISTENERS */
	// Segment count
	connect(m_ui_->sb_hair_num_segments,SIGNAL(valueChanged(int)), this,
	        SLOT(hair_segment_count_changed_listener(int)));
	// Hair length
	connect(m_ui_->sb_hair_length,SIGNAL(valueChanged(double)), this, SLOT(hair_length_changed_listener(double)));

	// Color dialog
	connect(m_ui_->b_hair_color,SIGNAL(clicked(bool)), this, SLOT(hair_color_clicked_listener()));
	connect(m_ui_->b_hair_root_color, SIGNAL(clicked(bool)), this, SLOT(hair_root_color_clicked_listener()));

	/* Light Settings */
	// Light Hair
	connect(m_ui_->cb_light_hair,SIGNAL(toggled(bool)), this, SLOT(light_hair_changed_listener(bool)));
	// Light color
	connect(m_ui_->b_light_color,SIGNAL(clicked(bool)), this, SLOT(light_color_clicked_listener()));
}

/**
 * \brief Listener that gets called when the uv grid is turned on or off
 * \param b Determines whether the uv grid should be visible
 */
void MainWindow::uv_visibility_changed_listener(const bool b) const
{
	m_ui_->widget_gl->set_uv_overlay_visible(b);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the hair lighting is turned on or off
 * \param enabled The state of the hair lighting
 */
void MainWindow::light_hair_changed_listener(const bool enabled) const
{
	m_ui_->widget_gl->set_light_hair_uniform(enabled);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that opens a dialog for setting the light color
 */
void MainWindow::light_color_clicked_listener()
{
	// Connect the listener of the select color button
	connect(&m_color_picker_dialog_, SIGNAL(colorSelected(QColor)), this, SLOT(light_color_selected_listener(QColor)));
	m_color_picker_dialog_.show();
}

/**
 * \brief Called when the user selects a color after opening the color dialog via the light color button
 * \param color The selected color
 */
void MainWindow::light_color_selected_listener(const QColor& color) const
{
	// Disconnect the listener of the select color button
	disconnect(&m_color_picker_dialog_, nullptr, nullptr, nullptr);

	const QString s("background-color: " + color.name() + ";");
	m_ui_->b_light_color->setStyleSheet(s);
	m_ui_->widget_gl->set_light_color_uniform(color.red(), color.green(), color.blue());
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the amount of hair segments has been changed
 * \param segments The amount of segments per hair
 */
void MainWindow::hair_segment_count_changed_listener(const int segments) const
{
	m_ui_->widget_gl->set_hair_num_segments_uniform(segments);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the hair length has been changed
 * \param length The length of the hair
 */
void MainWindow::hair_length_changed_listener(const double length) const
{
	m_ui_->widget_gl->set_hair_length_uniform(length);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that opens a dialog for setting the hair color
 */
void MainWindow::hair_color_clicked_listener()
{
	// Connect the listener of the select color button
	connect(&m_color_picker_dialog_, SIGNAL(colorSelected(QColor)), this, SLOT(hair_color_selected_listener(QColor)));
	m_color_picker_dialog_.show();
}

/**
 * \brief Listener that opens a dialog for setting the hair root color
 */
void MainWindow::hair_root_color_clicked_listener()
{
	// Connect the listener of the select color button
	connect(&m_color_picker_dialog_, SIGNAL(colorSelected(QColor)), this,
	        SLOT(hair_root_color_selected_listener(QColor)));
	m_color_picker_dialog_.show();
}

/**
 * \brief Called when the user selects a color after opening the color dialog via the hair color button
 * \param color The selected color
 */
void MainWindow::hair_color_selected_listener(const QColor& color) const
{
	// Disconnect the listener of the select color button
	disconnect(&m_color_picker_dialog_, nullptr, nullptr, nullptr);

	const QString s("background-color: " + color.name() + ";");
	m_ui_->b_hair_color->setStyleSheet(s);
	m_ui_->widget_gl->set_hair_color_unfirom(color.red(), color.green(), color.blue());
	m_ui_->widget_gl->update();
}

/**
 * \brief Called when the user selects a color after opening the color dialog via the root color button
 * \param color The selected color
 */
void MainWindow::hair_root_color_selected_listener(const QColor& color) const
{
	// Disconnect the listener of the select color button
	disconnect(&m_color_picker_dialog_, nullptr, nullptr, nullptr);

	const QString s("background-color: " + color.name() + ";");
	m_ui_->b_hair_root_color->setStyleSheet(s);
	m_ui_->widget_gl->set_hair_root_color_uniform(color.red(), color.green(), color.blue());
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the brush intensity has been changed
 * \param intensity Intensity of the brush
 */
void MainWindow::brush_intensity_changed_listener(const double intensity) const
{
	m_ui_->widget_gl->m_brush.set_brush_intensity(intensity);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the brush size has been changed
 * \param size Size of the brush
 */
void MainWindow::brush_size_changed_listener(const double size) const
{
	m_ui_->widget_gl->m_brush.set_brush_size(size);
	m_ui_->widget_gl->update();
}

/**
 * \brief Listener that gets called when the brush mode has been changed
 * \param mode The id of the radiobutton which determines the brush mode
 * \param checked True if the button is checked
 */
void MainWindow::brush_mode_changed_listener(int mode, const bool checked) const
{
	if (checked)
	{
		m_ui_->widget_gl->m_brush.set_paintmode(static_cast<Paintbrush::paintmode>(mode));
		m_ui_->widget_gl->update();
	}
}

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

void MainWindow::set_hair_color(const QColor& color) const
{
	hair_color_selected_listener(color);
}

void MainWindow::set_hair_root_color(const QColor& color) const
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
		case Paintbrush::paintmode::tangent:
			m_ui_->rb_brushmode_curl->click();
			break;
		case Paintbrush::paintmode::bitangent:
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

void MainWindow::set_light_color(const QColor& color) const
{
	light_color_selected_listener(color);
}
