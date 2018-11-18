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
 * \brief Handles the UI, its control elements and their respective listeners. Acts as the bridge between the core application and the OpenGL Rendering (GlWidget),
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void display_messagebox(const char* title, const char* content);
	void display_errorbox(const char* title, const char* content);
	bool display_questionbox(const char* title, const char* question);
	void set_statusbar_text(const std::string& text) const;
	void set_window_title_prefix(const QString& title_prefix);

	QColor m_hair_color;
	QColor m_hair_root_color;
	QColor m_light_color;
	Paintbrush::paintmode m_paintmode;

	// Getters/Setters
	void set_presenter(MvpPresenter* presenter) noexcept { this->m_presenter_ = presenter; }
	MvpPresenter* get_presenter() const noexcept{ return m_presenter_; }
	Ui::MainWindow* get_ui() const noexcept{ return m_ui_;}
	UiSettings get_ui_settings() const;

	// Setters for UI elements
	void set_uv_grid_visibility(bool visible) const;
	void set_hair_length(double length) const;
	void set_hair_segment_count(int segments) const;
	void set_hair_color(const QColor& color);
	void set_hair_root_color(const QColor& color);
	void set_brush_mode(Paintbrush::paintmode mode) const;
	void set_brush_size(double size) const;
	void set_brush_intensity(double intensity) const;
	void set_light_hair(bool active) const;
	void set_light_mesh(bool active) const;
	void set_light_color(const QColor& color);
	void set_growthmesh_show(bool active) const;
	void set_referencemodel_show(bool active) const;
	void set_up_axis_index(int index) const;

	void set_growthmesh_index(int index) const;
	void set_growthmesh_index_content(std::vector<std::string> content) const;
	void clear_growthmesh_index_content() const;
	void set_ui_settings(const UiSettings& settings);
public slots:
		void on_actionSave_Project_triggered();
private slots:

	void on_actionExport_triggered();
	void on_actionOpen_Fbx_triggered();
	void on_actionExport_Hairstyle_triggered();
	void on_actionOpen_Project_triggered();
	void on_actionSave_Project_as_triggered();
	void on_actionNew_Project_triggered();
	void on_actionLoad_Hairstyle_triggered();

	// Listeners for ui control elements
	// general
	void uv_visibility_changed_listener(bool enabled) const;
	void growthmesh_index_changed_listener(int index) const;
	void up_axis_changed_listener(int index) const;

	// hair listeners
	void hair_segment_count_changed_listener(int segments) const;
	void hair_length_changed_listener(double length) const;
	void hair_color_clicked_listener();
	void hair_root_color_clicked_listener();
	void hair_color_selected_listener(const QColor& color);
	void hair_root_color_selected_listener(const QColor& color);

	// brush listeners
	void brush_intensity_changed_listener(double intensity) const;
	void brush_size_changed_listener(double size) const;
	void brush_mode_changed_listener(int mode, bool checked);
	void growthmesh_show_changed_listener(bool enabled) const;
	void referencemodel_show_changed_listener(bool enabled) const;

	//light listeners
	void light_hair_changed_listener(bool enabled) const;
	void light_mesh_changed_listener(bool enabled) const;
	void light_color_clicked_listener();
	void light_color_selected_listener(const QColor& color);

private:
	// Reference to the ui generated by QT
	Ui::MainWindow* m_ui_;

	MvpPresenter* m_presenter_{};

	// Used when picking a color
	QColorDialog m_color_picker_dialog_;

	// Helper methods for opening file dialogs
	QString save_file_dialog(const char* title, const char* types);
	QString open_file_dialog(const char* title, const char* types);

	// Connect the listeners to the UI Elements and their shortcuts
	void connect_signals_and_slots();

protected:
	void dropEvent(QDropEvent* e) override;
	void dragEnterEvent(QDragEnterEvent* e) override;

private:
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
