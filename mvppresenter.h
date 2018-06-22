#ifndef MVPPRESENTER_H
#define MVPPRESENTER_H
#include <mvpmodel.h>
#include <qfile.h>
class MainWindow;

class MvpPresenter
{
public:
	MvpPresenter();

	// Getters and Setters
	void set_model(MvpModel* model) { this->m_model_ = model; }
	void set_view(MainWindow* view) { this->m_view_ = view; }
	MvpModel* get_model() const { return m_model_; }
	MainWindow* get_view() const { return m_view_; }
	static void export_hair(QFile* file);
private:

	MvpModel* m_model_;
	MainWindow* m_view_;
};

#endif // MVPPRESENTER_H
