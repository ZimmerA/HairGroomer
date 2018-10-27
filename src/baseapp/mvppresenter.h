#ifndef MVPPRESENTER_H
#define MVPPRESENTER_H

#include "mvpmodel.h"

class QString;
class MainWindow;

/**
 * \brief Serves as the connection between MvpModel and MainWindow(the view)
 */
class MvpPresenter
{
public:
	MvpPresenter() noexcept;
	void load_ui_values(const UiSettings& settings) const;
	void export_hair(const QString& filename) const;
	void export_hairstyle(const QString& filename) const;
	void load_hairstyle(const QString& filename) const;
	void load_fbx_model(const QString& filename) const;
	void load_project_file(const QString& filename) const;
	void save_project_file(const QString& filename) const;

	// Getters and Setters
	void set_model(MvpModel* model) noexcept{ this->m_model_ = model; }
	void set_view(MainWindow* view) noexcept{ this->m_view_ = view; }
	MvpModel* get_model() const noexcept { return m_model_; }
	MainWindow* get_view() const noexcept{ return m_view_; }

private:
	MvpModel* m_model_;
	MainWindow* m_view_;
};

#endif // MVPPRESENTER_H
