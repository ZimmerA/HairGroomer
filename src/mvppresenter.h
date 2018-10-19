#ifndef MVPPRESENTER_H
#define MVPPRESENTER_H

#include "mvpmodel.h"
// Forward declare MainWindow
class MainWindow;

/**
 * \brief Serves as the connection between MvpModel and MainWindow(the view)
 */
class MvpPresenter
{
public:
	MvpPresenter();
	void load_default_values() const;
	void export_hair(const QString& filename) const;
	void export_hairstyle(const QString& filename) const;
	void load_hairstyle(const QString& filename) const;

	// Getters and Setters
	void set_model(MvpModel* model) { this->m_model_ = model; }
	void set_view(MainWindow* view) { this->m_view_ = view; }
	MvpModel* get_model() const { return m_model_; }
	MainWindow* get_view() const { return m_view_; }

private:
	MvpModel* m_model_;
	MainWindow* m_view_;
};

#endif // MVPPRESENTER_H
