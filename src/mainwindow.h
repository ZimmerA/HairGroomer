#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QColorDialog>

#include "mvppresenter.h"
namespace Ui
{
	class MainWindow;
}

/**
 * \brief Handles the Ui and its control elements, also holds the reference to the opengl rendering widget
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void display_messagebox(const char* title, const char* content);
	// Getters/Setters
	void set_presenter(MvpPresenter* presenter) { this->m_presenter_ = presenter; }
	MvpPresenter* get_presenter() const { return m_presenter_; }
	Ui::MainWindow* get_ui() const { return m_ui_;}

	// Setters for UI elements
	void set_uv_grid_visibility(bool visible) const;
	void set_hair_length(double length) const;
	void set_hair_segment_count(int segments) const;
	void set_hair_color(const QColor& color) const;
	void set_hair_root_color(const QColor& color) const;
	void set_brush_mode(Paintbrush::paintmode mode) const;
	void set_brush_size(double size) const;
	void set_brush_intensity(double intensity) const;
	void set_light_hair(bool active) const;
	void set_light_mesh(bool active) const;
	void set_light_color(const QColor& color) const;
	void set_growthmesh_show(bool active) const;
	void set_referencemodel_show(bool active) const;

private slots:
	void on_actionExport_triggered() const;
	void on_actionExport_Hairstyle_triggered();
	void on_actionLoad_Hairstyle_triggered();
	// Listeners for ui control elements
	// general
	void uv_visibility_changed_listener(const bool b) const;
	// hair listeners
	void hair_segment_count_changed_listener(int segments) const;
	void hair_length_changed_listener(double length) const;
	void hair_color_clicked_listener();
	void hair_root_color_clicked_listener();
	void hair_color_selected_listener(const QColor& color) const;
	void hair_root_color_selected_listener(const QColor& color) const;

	// brush listeners
	void brush_intensity_changed_listener(double intensity) const;
	void brush_size_changed_listener(double size) const;
	void brush_mode_changed_listener(int mode, bool checked) const;
	void growthmesh_show_changed_listener(bool enabled) const;
	void referencemodel_show_changed_listener(bool enabled) const;

	//light listeners
	void light_hair_changed_listener(bool enabled) const;
	void light_mesh_changed_listener(bool enabled) const;
	void light_color_clicked_listener();
	void light_color_selected_listener(const QColor& color) const;

private:
	Ui::MainWindow* m_ui_;
	MvpPresenter* m_presenter_{};
	QColorDialog m_color_picker_dialog_;
	void connect_signals_and_slots();
	QString save_file_dialog(const char* title, const char* types);
	QString open_file_dialog(const char* title, const char* types);

	// Shortcuts for Ui controls
	QShortcut *m_hotkey_toggle_uv_{};
	QShortcut *m_hotkey_brushsize_reduce_{};
	QShortcut *m_hotkey_brushsize_increase_{};
	QShortcut *m_hotkey_brushintensity_reduce_{};
	QShortcut *m_hotkey_brushintensity_increase_{};
	QShortcut *m_hotkey_brushmode_hair_{};
	QShortcut *m_hotkey_brushmode_curl_{};
	QShortcut *m_hotkey_brushmode_twist_{};
};

#endif // MAINWINDOW_H
