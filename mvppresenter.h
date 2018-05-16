#ifndef MVPPRESENTER_H
#define MVPPRESENTER_H
#include <mvpmodel.h>
#include <mainwindow.h>
#include <modelData.h>

class MvpPresenter
{
public:
	MvpPresenter();

	// Getters and Setters
	void setModel(MvpModel * model) { this->model = model; }
	void setView(MainWindow * view) { this->view = view; }
	MvpModel * getModel() const { return model; }
	MainWindow * getView() const { return view; }

private:

	MvpModel * model;
	MainWindow * view;
};

#endif // MVPPRESENTER_H