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
	void export_hair(const QString& file_path) const;
	void export_hairstyle(const QString& file_path) const;
	void load_hairstyle(const QString& file_path) const;
	void load_fbx_model(const QString& file_path) const;
	void load_project_file(const QString& file_path) const;
	void new_project() const;
	bool save_project_file() const;
	void save_project_file_as(const QString& file_path) const;

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
